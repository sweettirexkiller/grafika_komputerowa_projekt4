#pragma once
#include "glm/ext/vector_float3.hpp"
struct LightCreateInfo {
	glm::vec3 position, color;
	float strength;
};

class Light {
public:
	glm::vec3 position, color;
	float strength;
	Light(LightCreateInfo* createInfo);
};