#include "UVTorus.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
using namespace std;
//nmake /F Nmakefile.Windows
void UVTorus::save(const std::string filename) {
  std::cout << "Generating geometry and saving to " << filename << std::endl;

  std::vector<glm::vec3> positions;
  std::vector<unsigned int> vertexIndices;
  std::vector<glm::vec2> textureCoords;
  std::vector<glm::vec3> normals;
  glm::vec3 centreT(0.0, 0.0, 1.0);
  glm::vec3 centreB(0.0, 0.0, 0.0);
  positions.push_back(centreT);
  positions.push_back(centreB);
  
  float deltaTheta = (M_PI * 2.0) / this->numVerticalSegments;
  float deltaPhi = (M_PI * 2.0) / this->numHorizontalSegments;
  float phi = 0.0f;
  float theta = 0.0f;
  int i = 0;
  // generate one circle at this longitude
  for (float theta = 0.0f; theta < 2*M_PI+deltaTheta; theta += deltaTheta) {
	  phi = 0.0f;
	  for(float phi = 0.0f; phi < 2*M_PI+deltaPhi; phi += deltaPhi){
		  // generate two vertices
		  //vertices
		  float x = (this->cValue + this->radius*cos(theta))*cos(phi);
		  float y = (this->cValue + this->radius * cos(theta))*sin(phi);
		  float z = this->radius*sin(theta);
		  //cout << "X:" << x << "Y:" << y << "Z:" << z << endl;
		  glm::vec3 posO(x, y, z);
		  positions.push_back(posO);
		  float xI = (this->cValue + this->radius*cos(theta + deltaTheta))*cos(phi +deltaPhi);
		  float yI = (this->cValue + this->radius * cos(theta+deltaTheta))*sin(phi+deltaPhi);
		  float zI = this->radius*sin(theta); 
		  glm::vec3 posI(x, y, z);
		  positions.push_back(posI);
		  
		  // texture coords, unnecessary
		  float uB = theta / (2.0 * M_PI);
		  float vB = (1.0 + M_PI) / (2.0 * M_PI);
		  
		  glm::vec2 uvB(uB,vB);
		  textureCoords.push_back(uvB);
		  // calculate the surface normals for top
		  glm::vec3 normalT = glm::normalize(posO - centreT);
		  normals.push_back(normalT);
		  
		  // calculate the surface normals for bot
		  glm::vec3 normalB = glm::normalize(posI - centreB);
		  normals.push_back(normalB);
		  
		  /*vertexIndices.push_back(i+(i*2));
		  vertexIndices.push_back(i+(i*2) + 1);
		  vertexIndices.push_back(i+(i*2) + 2);*/
		   if (theta> 0) {
			// now we figure out the patterns
			unsigned int a = positions.size() - 1;
			unsigned int b = positions.size() - 2;
			unsigned int c = positions.size() - 3;
			unsigned int d = positions.size() - 4;
			vertexIndices.push_back(b);
			vertexIndices.push_back(0);
			vertexIndices.push_back(d);			
		  }
		  //i++;
	  }	  
  }
  std::ofstream fileOut(filename.c_str());

  if (!fileOut.is_open()) {
    return;
  }

  for (unsigned int i = 0; i < positions.size(); i++) {
    fileOut << "v " << positions[i].x << " "  << positions[i].y << " "  << positions[i].z << std::endl;
  }

  for (unsigned int i = 0; i < textureCoords.size(); i++) {
    fileOut << "vt " << textureCoords[i].s << " "  << textureCoords[i].t << std::endl;
  }

  for (unsigned int i = 0; i < normals.size(); i++) {
    fileOut << "vn " << normals[i].x << " "  << normals[i].y << " "  << normals[i].z << std::endl;
  }

  for (unsigned int i = 0; i < vertexIndices.size(); i += 3) {
    fileOut << "f " << vertexIndices[i] << "/" << vertexIndices[i] << "/" << vertexIndices[i] << " ";
    fileOut << vertexIndices[i+1] << "/" << vertexIndices[i+1] << "/" << vertexIndices[i+1] << " ";
    fileOut << vertexIndices[i+2] << "/" << vertexIndices[i+2] << "/" << vertexIndices[i+2] << std::endl;
  }

  fileOut.close();
}



glm::vec3* UVTorus::getPositions() { return this->positions.data(); }
glm::vec2* UVTorus::getTextureCoords() { return this->textureCoords.data(); }
glm::vec3* UVTorus::getNormals() { return this->normals.data(); }
unsigned int UVTorus::getNumVertices() { return this->numVertices; }
unsigned int UVTorus::getNumTriangles() { return this->numTriangles; }
unsigned int* UVTorus::getTriangleIndices() { return this->triangleIndices; }
float UVTorus::getRadius() { return this->radius; }
float UVTorus::getcValue() { return this->cValue; }
unsigned int UVTorus::getNumVerticalSegments() { return this->numVerticalSegments; }
unsigned int UVTorus::getNumHorizontalSegments() { return this->numHorizontalSegments; }
