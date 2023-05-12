#pragma once

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "noise.h"
#include "hashes.h"
#include "block.h"
#include "cubes.h"

#undef far

constexpr static int chunk_size = 64;
struct Chunk {
    Chunk() : data(nullptr) {}
    void create() {
        data = new Block[chunk_size * chunk_size * chunk_size];
    }
    void destroy() {
        delete[] data;
    }

    Block& operator()(const glm::ivec3& pos) {
        if (!in_bounds(pos)) {
            std::stringstream msg;
            msg << "Position out of bounds: " << glm::to_string(pos);
            throw std::runtime_error(msg.str());
        }
        return data[pos.x * chunk_size * chunk_size + pos.y * chunk_size + pos.z];
    }

    bool in_bounds(glm::ivec3 pos) const {
        return pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
               pos.x < chunk_size && pos.y < chunk_size && pos.z < chunk_size;
    }

    Block* data;
};

struct World {
    World(Cubes& cubes, Cubes& water_cubes) : cubes(cubes), water_cubes(water_cubes) {}

    bool is_solid(glm::ivec3 block_pos) {
        return get_block(block_pos).solid;
    }

    Block get_block(glm::vec2 pos) {
        return get_block(glm::floor(pos)); 
    }

    Block get_block(glm::ivec3 block_pos) {
        glm::ivec2 chunk_pos = glm::floor(glm::vec2(block_pos.x, block_pos.z) / (float)chunk_size);
        Chunk& chunk = get_chunk(chunk_pos);
        glm::ivec3 local_pos = glm::ivec3(glm::mod((float)block_pos.x, (float)chunk_size), block_pos.y, glm::mod((float)block_pos.z, (float)chunk_size));
        if (!chunk.in_bounds(local_pos)) {
            return Block();
        }
        return chunk(local_pos);
    }

    Chunk& get_chunk(glm::ivec2 chunk_pos) {
        load_chunk(chunk_pos);
        return chunk_store[chunk_pos];
    }

    void load_nearby(glm::vec3 pos, int range = 1) {
        int x0 = (int)(pos.x / chunk_size) - range;
        int x1 = x0 + range * 2;
        int z0 = (int)(pos.z / chunk_size) - range;
        int z1 = z0 + range * 2;

        for (int x = x0; x <= x1; ++x) {
            for (int z = z0; z <= z1; ++z) {
                load_chunk({x,z});
            }
        }
    }

    void unload_far(glm::vec3 pos, int far = 2) {
        int x0 = (int)(pos.x / chunk_size) - far;
        int x1 = x0 + far * 2;
        int z0 = (int)(pos.z / chunk_size) - far;
        int z1 = z0 + far * 2;

        auto it = chunk_store.begin();
        while (it != chunk_store.end()) {
            const glm::ivec2& chunk_pos = it->first;
            if (chunk_pos.x < x0 || chunk_pos.y < z0 || chunk_pos.x > x1 || chunk_pos.y > z1) {
                it->second.destroy();
                it = chunk_store.erase(it);
            } else {
                ++it;
            }
        }
    }

    void load_chunk(glm::ivec2 chunk_pos) {
        if (!has_chunk(chunk_pos)) {
            chunk_store[chunk_pos] = gen_chunk(chunk_pos);
            cubes_dirty = true;
        }
    }

    void free_chunk(glm::ivec2 chunk_pos) {
        std::cout << "freeing chunk " << glm::to_string(chunk_pos) << std::endl;
        chunk_store[chunk_pos].destroy();
        chunk_store.erase(chunk_pos);
        cubes_dirty = true;
    }

    bool has_chunk(glm::ivec2 chunk_pos) {
        return chunk_store.count(chunk_pos) == 1;
    }

    Chunk gen_chunk(glm::ivec2 chunk_pos) {
        std::cout << "generating chunk " << glm::to_string(chunk_pos) << std::endl;
        Chunk chunk;
        chunk.create();
        for (int x = 0; x < chunk_size; ++x) {
            for (int z = 0; z < chunk_size; ++z) {
                glm::vec3 perlin_pos = glm::vec3(x, 0, z);
                perlin_pos.x += chunk_pos.x * chunk_size;
                perlin_pos.z += chunk_pos.y * chunk_size;
                perlin_pos *= 0.02f;
                float f = noise::perlin3d(perlin_pos, 3, 0.5);
                int h = (int)(f * 20) + 20;
                
                for (int y = h - 1; y <= h; ++y) { 
                    chunk({x,y,z}) = Block(true, (f < 0 ? 1 : f < 0.1 ? 2 : 3));;
                }
                
                for (int y = h+1; y <= 12; ++y) {
                    chunk({x,y,z}) = Block(false, 4);
                }
            }
        }
        return chunk;
    }

    void update_cubes_instances() {
        if (!cubes_dirty) { return; }
        std::vector<Cubes::Instance> cube_instances;
        std::vector<Cubes::Instance> water_instances;

        for (auto& it : chunk_store) {
            const glm::ivec2& chunk_pos = it.first;
            Chunk& chunk = it.second;
            for (int x = 0; x < chunk_size; ++x) {
                for (int y = 0; y < chunk_size; ++y) {
                    for (int z = 0; z < chunk_size; ++z) {
                        Block b = chunk({x,y,z});
                        Cubes::Instance inst(
                                glm::vec3(x + chunk_pos.x * chunk_size, y, z + chunk_pos.y * chunk_size),
                                b.type
                        );
                            
                        if (b.type == 4) {
                            water_instances.emplace_back(inst);
                        } else if (b.type != 0) {
                            cube_instances.emplace_back(inst);
                        }
                    }
                }
            }
        }
        
        cubes.vao.instances.set_data(cube_instances);
        water_cubes.vao.instances.set_data(water_instances);
        cubes_dirty = false;
    }

    bool cubes_dirty = true;
    Cubes& cubes;
    Cubes& water_cubes;
    std::unordered_map<glm::ivec2, Chunk> chunk_store;
};