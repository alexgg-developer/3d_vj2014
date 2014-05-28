#pragma once
#include <vector>
#include <glm/glm.hpp>

struct cMesh {
	int textureIndex;
	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> triangles;
};
