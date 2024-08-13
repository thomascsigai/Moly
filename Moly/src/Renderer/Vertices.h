#pragma once

// In this file are defined vertices data to draw simple objects

#define MOLY_DEFINE_TRIANGLE_VERTICES(vec)            \
    vec.push_back({ glm::vec3(-0.5f, -0.5f, 0.0f) }); \
    vec.push_back({ glm::vec3( 0.5f, -0.5f, 0.0f) }); \
    vec.push_back({ glm::vec3( 0.0f,  0.5f, 0.0f) });