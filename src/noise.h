#pragma once

#include <random>
#include <algorithm>

#include <glm/glm.hpp>

namespace noise {
uint hash(uint a) {
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
}

static glm::vec3 directions[12] = {
    {1, 1, 0}, {1, -1, 0}, {-1, -1, 0}, {-1, 1, 0},
    {1, 0, 1}, {1, 0, -1}, {-1, 0, -1}, {-1, 0, 1},
    {0, 1, 1}, {0, 1, -1}, {0, -1, -1}, {0, -1, 1}
};

static glm::vec3 gradient_direction(glm::ivec3 grid_pos) {
    uint coord = hash(hash(hash(grid_pos.x) + grid_pos.y) + grid_pos.z);
    return directions[coord % 12]; 
}

static float dot_grid_gradient(glm::ivec3 grid_pos, glm::vec3 pos) {
    const glm::vec3 f = pos - glm::vec3(grid_pos);
    const glm::vec3 gradients = gradient_direction(grid_pos);
    return glm::dot(f, gradients);
}

static glm::vec3 fade(glm::vec3 t) {
    return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
}

static float perlin3d(glm::vec3 pos) {
    const glm::ivec3 cell_pos = glm::floor(pos);
    const glm::vec3 weights = fade(pos - glm::vec3(cell_pos));

    const float n0 = dot_grid_gradient(cell_pos + glm::ivec3(0, 0, 0), pos);
    const float n1 = dot_grid_gradient(cell_pos + glm::ivec3(1, 0, 0), pos);
    const float ix0 = glm::mix(n0, n1, weights.x);

    const float n2 = dot_grid_gradient(cell_pos + glm::ivec3(0, 1, 0), pos);
    const float n3 = dot_grid_gradient(cell_pos + glm::ivec3(1, 1, 0), pos);
    const float ix1 = glm::mix(n2, n3, weights.x);

    const float n4 = dot_grid_gradient(cell_pos + glm::ivec3(0, 0, 1), pos);
    const float n5 = dot_grid_gradient(cell_pos + glm::ivec3(1, 0, 1), pos);
    const float ix2 = glm::mix(n4, n5, weights.x);

    const float n6 = dot_grid_gradient(cell_pos + glm::ivec3(0, 1, 1), pos);
    const float n7 = dot_grid_gradient(cell_pos + glm::ivec3(1, 1, 1), pos);
    const float ix3 = glm::mix(n6, n7, weights.x);

    // interpolate the 4 interpolated values by y
    const float ixy0 = glm::mix(ix0, ix1, weights.y);
    const float ixy1 = glm::mix(ix2, ix3, weights.y);

    // interpolate the 2 remaining values by z
    return glm::mix(ixy0, ixy1, weights.z);
}

static float perlin3d(glm::vec3 pos, int octaves, float persistence) {
    float sum = 0;
    float amplitude = 1;
    float frequency = 1;
    for (int i = 0; i < octaves; ++i) {
        sum += perlin3d(pos * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    return sum;
}
}