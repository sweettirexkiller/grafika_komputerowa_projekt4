#pragma once
#include <vector>
#include "glm/ext/vector_float3.hpp"

class DynamicSurface {
public:
	unsigned int VBO, VAO, vertexCount;
	std::vector<float> vertices;

	DynamicSurface();
	void build(const std::vector<glm::vec3>& data);
	~DynamicSurface();
};
