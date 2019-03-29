#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#pragma once

class UVTorus {
  private:
    unsigned int numVerticalSegments;
    unsigned int numHorizontalSegments;
    float radius;
	float cValue;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textureCoords;
    std::vector<glm::vec3> normals;

    unsigned int numVertices;
    unsigned int numTriangles;
    unsigned int* triangleIndices;

  public:

    UVTorus(float radius, float cValue, unsigned int vertRes, unsigned int horizRes) : radius(radius), cValue(cValue), numVerticalSegments(vertRes), numHorizontalSegments(horizRes) {}

    glm::vec3* getPositions();
    glm::vec2* getTextureCoords();
    glm::vec3* getNormals();

    unsigned int getNumVertices();
    unsigned int getNumTriangles();

    unsigned int* getTriangleIndices();

    float getRadius();
	float getcValue();
    unsigned int getNumVerticalSegments();
    unsigned int getNumHorizontalSegments();

    void save(const std::string filename);
};
