#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "apis/stb_image.h"

#include "ShaderProgram.h"
#include "ObjMesh.h"

int width, height;

GLuint programId;
GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;
GLuint positions_vbo = 0;
GLuint textureCoords_vbo = 0;
GLuint normals_vbo = 0;
GLuint colours_vbo = 0;
GLuint textureId;

GLfloat xRotated, yRotated, zRotated;
GLdouble innerRadius = 0.30;
GLdouble outerRadius = 0.8;
GLint sides = 50;
GLint rings = 50;


unsigned int numVertices;
bool rotating = true;
unsigned int loadTexture(char const * path);

static void createTexture(std::string filename) {
   int imageWidth, imageHeight;
   int numComponents;
   // load the image data into a bitmap
   unsigned char *bitmap = stbi_load(filename.c_str(), &imageWidth, &imageHeight, &numComponents, 4);
   // generate a texture name
   glGenTextures(1, &textureId);
   // make the texture active
   glBindTexture(GL_TEXTURE_2D, textureId);
   // make a texture mip map
   glGenerateTextureMipmap(textureId);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
   // specify the functions to use when shrinking/enlarging the texture image
   // recommended to use this glTexParamterics (GL_LINEAR make it much smoother than NEAREST)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   // send the data to OpenGL
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
   // free the bitmap data
   stbi_image_free(bitmap);
}

static void createGeometry(void) {
  ObjMesh mesh;
  mesh.load("meshes/sphere.obj", true, true);

  numVertices = mesh.getNumIndexedVertices();
  Vector3* vertexPositions = mesh.getIndexedPositions();
  Vector2* vertexTextureCoords = mesh.getIndexedTextureCoords();
  Vector3* vertexNormals = mesh.getIndexedNormals();

  //position buffer
  glGenBuffers(1, &positions_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertexPositions, GL_STATIC_DRAW);

  //texture buffer
  glGenBuffers(1, &textureCoords_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector2), vertexTextureCoords, GL_STATIC_DRAW);

  //normals buffer
  glGenBuffers(1, &normals_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vector3), vertexNormals, GL_STATIC_DRAW);

  unsigned int* indexData = mesh.getTriangleIndices();
  int numTriangles = mesh.getNumTriangles();

  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numTriangles * 3, indexData, GL_STATIC_DRAW);
}

float angle = 0.0f;

//method for updating the model
static void update(void) {
   int milliseconds = glutGet(GLUT_ELAPSED_TIME);
   //torus
   xRotated = xRotated + 0.01;
   yRotated += 0.01;
   zRotated += 0.01;
   //used to rotate the model
   if (rotating) {
      float degrees = (float)milliseconds / 10.0f;
      angle = degrees;
   }
   glutPostRedisplay();
}

//torus
void myinit()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { -0.4, 1.0, -0.5, 0.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

static void render(void) {
	glClearColor(0.95f, 0.9f, 0.6f, 0.0f) ;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// clear the drawing buffer.
	glClear(GL_COLOR_BUFFER_BIT);
	// clear the identity matrix.
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	glColor3f(0.0, 0.0, 0.0);
	glRotatef(zRotated, 0.0, 0.0, 1.0);
	// scaling transfomation 
	glScalef(1.0, 1.0, 1.0);
	// built-in (glut library) function , draw you a sphere.
	glUseProgram(0);
	glutSolidTorus(innerRadius, outerRadius, sides, rings);
	glUseProgram(programId);
   // turn on depth buffering
   glEnable(GL_DEPTH_TEST);

   float aspectRatio = (float)width / (float)height;
   glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
   
   // view matrix - orient everything around our preferred view
   glm::mat4 view = glm::lookAt(
      glm::vec3(40,30,30), // eye/camera location
      glm::vec3(0,0,0),    // where to look
      glm::vec3(0,1,0)     // up
   );

   // model matrix: translate, scale, and rotate the model
   glm::vec3 rotationAxis(0,1,0);
   glm::mat4 model = glm::mat4(1.0f);
   model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0)); // rotate about the y-axis
   model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));

   // model-view-projection matrix
   glm::mat4 mvp = projection * view * model;
   GLuint mvpMatrixId = glGetUniformLocation(programId, "MVP");
   glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

   // texture sampler - a reference to the texture we've previously created
   // send the texture id to the texture sampler
   GLuint textureUniformId = glGetUniformLocation(programId, "textureSampler");
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glUniform1i(textureUniformId, 0);

   // find the names (ids) of each vertex attribute
   GLint positionAttribId = glGetAttribLocation(programId, "position");
   GLint colourAttribId = glGetAttribLocation(programId, "colour");
   GLint textureCoordsAttribId = glGetAttribLocation(programId, "textureCoords");
   GLint normalAttribId = glGetAttribLocation(programId, "normal");

   // provide the vertex positions to the shaders
   glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
   glEnableVertexAttribArray(positionAttribId);
   glVertexAttribPointer(positionAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

   // provide the vertex texture coordinates to the shaders
   glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vbo);
   glEnableVertexAttribArray(textureCoordsAttribId);
   glVertexAttribPointer(textureCoordsAttribId, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

   // provide the vertex normals to the shaders
   glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
   glEnableVertexAttribArray(normalAttribId);
   glVertexAttribPointer(normalAttribId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// draw the triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, (void*)0);

	// disable the attribute arrays
   glDisableVertexAttribArray(positionAttribId);
   glDisableVertexAttribArray(textureCoordsAttribId);
   glDisableVertexAttribArray(normalAttribId);
   glDisableVertexAttribArray(colourAttribId);

	// make the draw buffer to display buffer (i.e. display what we have drawn)
   //flush out the buffers
   glFlush();
	glutSwapBuffers(); 
}

static void reshape(int w, int h) {
  glViewport(0, 0, w, h);

  width = w;
  height = h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-1.5, 1.5, -1.5*(GLfloat)480 / (GLfloat)640,
	  1.5*(GLfloat)480 / (GLfloat)640, -10.0, 10.0);
  glMatrixMode(GL_MODELVIEW);
  //glViewport(0,0,w,h);  //Use the whole window for rendering
  glLoadIdentity();
}

static void drag(int x, int y) {
}

static void mouse(int button, int state, int x, int y) {
}

static void keyboard(unsigned char key, int x, int y) {
   if (key == 'r') {
      rotating = !rotating;
   }
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(800, 600);
   glutCreateWindow("CSCI 3090u Graphics Project - ANIMATED RAPTORS LOGO");
   glClearColor(1,1,1,1);
   glutIdleFunc(&update);
   glutDisplayFunc(&render);
   glutReshapeFunc(&reshape);
   glutMotionFunc(&drag);
   glutMouseFunc(&mouse);
   glutKeyboardFunc(&keyboard);
   myinit();
   glewInit();
   if (!GLEW_VERSION_2_0) {
      std::cerr << "OpenGL 2.0 not available" << std::endl;
      return 1;
   }
   std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
   std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

   createGeometry();

   // load and prepare the texture
   createTexture("textures/basket_ball1.jpg");

   ShaderProgram program;
  	program.loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
  	programId = program.getProgramId();

   glutMainLoop();

   return 0;
}
