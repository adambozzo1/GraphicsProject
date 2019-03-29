#include <string>
#include <iostream>
#include <cmath>
#include <fstream>
#include <GL/glew.h>
#ifdef __APPLE__
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "apis/stb_image.h"

#include "ObjMesh.h"
#include "ShaderProgram.h"

#define SCALE_FACTOR 2.0f

using namespace std;

int width, height;

GLuint programId;
GLuint vertexBuffer;
GLuint indexBufferO;
GLuint indexBufferT;
GLuint positionBufferId;

//GLuint positions_vbo[];

GLuint positions_vboO = 0;
GLuint textureCoords_vboO = 0;
GLuint normals_vboO = 0;
GLenum colors_vboO = 0;

GLuint positions_vboT = 0;
GLuint textureCoords_vboT = 0;
GLuint normals_vboT = 0;
//GLenum colors_vboT = 0;
GLuint textureId;

unsigned int numVerticesS;
unsigned int numVerticesD;

bool rotating = true;
float yAngle = 0.0f;
glm::vec3 eyePosition(100, 30, 30);
bool rotateObject = true;
float scaleFactor = 1.0f;
GLdouble innerRadius = 0.25;
GLdouble outerRadius = 0.8;
GLint sides = 50;
GLint rings = 50;

unsigned int loadTexture(char const * path);

static void createTexture(std::string filename) {
   int imageWidth, imageHeight;
   int numComponents;

   // load the image data into a bitmap
   unsigned char *bitmap = stbi_load(filename.c_str(),
                                     &imageWidth,
                                     &imageHeight,
                                     &numComponents, 4);
     // generate a texture name
     glGenTextures(1, &textureId);

     // make the texture active
     glBindTexture(GL_TEXTURE_2D, textureId);

     // make a texture mip map
     //glGenerateMipmap(textureId);
     //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (imageWidth), (imageHeight),
               0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    std::cout << "Image width: " << imageWidth << "\nImage hieght: " << imageHeight << std::endl;

     // bind the texture to unit 0
    glBindTexture(GL_TEXTURE_2D, textureId);
    glActiveTexture(GL_TEXTURE0);

    // free the bitmap data
    stbi_image_free(bitmap);
}

static void createGeometry() {

  ObjMesh sphere;
  sphere.load("meshes/sphere.obj", true, true);

  ObjMesh dragon;
  dragon.load("meshes/dragon.obj", true, true);

  //ObjMesh objects[] = {mesh, dragon};


  cout << "Finished loading obj files." << endl;
  // use glGenBuffer, bindbuffer and buffer data to play around with assignment of gl data

  // need to figure out a way to get this to work with each thing.. sphere and dragon for each vbo for each of these obj files..

    numVerticesS = sphere.getNumIndexedVertices();
    Vector3* vertexPositionsS = sphere.getIndexedPositions();
    Vector2* vertexTextureCoordsS = sphere.getIndexedTextureCoords();
    Vector3* vertexNormalsS = sphere.getIndexedNormals();

    numVerticesD = dragon.getNumIndexedVertices();
    Vector3* vertexPositionsD = dragon.getIndexedPositions();
    Vector2* vertexTextureCoordsD = dragon.getIndexedTextureCoords();
    Vector3* vertexNormalsD = dragon.getIndexedNormals();

    // now we do the actual passing of information to the vertex shader

    // pos data for sphere
    positions_vboO;
    glGenBuffers(1, &positions_vboO);
    glBindBuffer(GL_ARRAY_BUFFER, positions_vboO);
    glBufferData(GL_ARRAY_BUFFER, numVerticesS * sizeof(Vector3), vertexPositionsS, GL_STATIC_DRAW);

    // pos data for dragon
    positions_vboT;
    glGenBuffers(1, &positions_vboT);
    glBindBuffer(GL_ARRAY_BUFFER, positions_vboT);
    glBufferData(GL_ARRAY_BUFFER, numVerticesD * sizeof(Vector3), vertexPositionsD, GL_STATIC_DRAW);

    // texture data for sphere
    textureCoords_vboO;
    glGenBuffers(1, &textureCoords_vboO);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vboO);
    glBufferData(GL_ARRAY_BUFFER, numVerticesS * sizeof(Vector2), vertexTextureCoordsS, GL_STATIC_DRAW);

    // texture data for dragon
    textureCoords_vboT;
    glGenBuffers(1, &textureCoords_vboT);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vboT);
    glBufferData(GL_ARRAY_BUFFER, numVerticesD * sizeof(Vector2), vertexTextureCoordsD, GL_STATIC_DRAW);

    // normal data for sphere
    normals_vboO;
    glGenBuffers(1, &normals_vboO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vboO);
    glBufferData(GL_ARRAY_BUFFER, numVerticesS * sizeof(Vector3), vertexNormalsS, GL_STATIC_DRAW);

    // normal data for dragon
    normals_vboT;
    glGenBuffers(1, &normals_vboT);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vboT);
    glBufferData(GL_ARRAY_BUFFER, numVerticesD * sizeof(Vector3), vertexNormalsD, GL_STATIC_DRAW);

    unsigned int* indexDataS = sphere.getTriangleIndices();
    unsigned int* indexDataD = dragon.getTriangleIndices();

    int numTrianglesS = sphere.getNumTriangles();
    int numTrianglesD = dragon.getNumTriangles();

    // drawing for sphere
    indexBufferO;
    glGenBuffers(1, &indexBufferO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numTrianglesS * 3, indexDataS, GL_STATIC_DRAW);

    // drawing for dragon
    indexBufferT;
    glGenBuffers(1, &indexBufferT);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferT);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numTrianglesD * 3, indexDataD, GL_STATIC_DRAW);
}


// has lighting and cool visual afffect done here
static void update(void) {
    int milliseconds = glutGet(GLUT_ELAPSED_TIME);

    // rotate the shape about the y-axis so that we can see the shading
    if (rotating) {
      float degrees = (float)milliseconds / 80.0f;
      yAngle = degrees;
    }
    glutPostRedisplay();
}

static void render(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // activate our shader program
	glUseProgram(programId);

   // turn on depth buffering
   glEnable(GL_DEPTH_TEST);

   float aspectRatio = (float)width / (float)height;
   glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

   // view matrix - orient everything around our preferred view
   glm::mat4 view = glm::lookAt(
      eyePosition, // eye/camera location
      glm::vec3(0,0,0),    // where to look
      glm::vec3(0,1,0)     // up
   );

   // model matrix: translate, scale, and rotate the model
   glm::vec3 objectPos[] = {
     glm::vec3(-5.0f, 0.0f, 0.0f),
     glm::vec3(10.0f,  5.0f, -15.0f),
     //glm::vec3(-50.0f, -2.2f, -2.5f)
   };

   //for (unsigned int i = 0; i < 2; i++) {

        glm::vec3 rotationAxis(0,1,0);
        glm::mat4 model = glm::mat4(1.0f);
    //    model = glm::translate(model, objectPos[i]);
        model = glm::rotate(model, glm::radians(yAngle), glm::vec3(0, 1, 0)); // rotate about the y-axis
        model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));

        // model-view-projection matrix
        glm::mat4 mvp = projection * view * model;
        GLuint mvpMatrixId = glGetUniformLocation(programId, "MVP");
        glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

        GLuint textureUniformId = glGetUniformLocation(programId, "textureSampler");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(textureUniformId, 0);

        // find the names (ids) of each vertex attribute
        GLint positionAttribIdO = glGetAttribLocation(programId, "position");
        GLint colourAttribIdO = glGetAttribLocation(programId, "colour");
        GLint textureCoordsAttribIdO = glGetAttribLocation(programId, "textureCoords");
        GLint normalAttribIdO = glGetAttribLocation(programId, "normal");

        // provide the vertex positions to the shaders for sphere
        glBindBuffer(GL_ARRAY_BUFFER, positions_vboO);
        glEnableVertexAttribArray(positionAttribIdO);
        glVertexAttribPointer(positionAttribIdO, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // provide the vertex texture coordinates to the shaders for sphere
        glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vboO);
        glEnableVertexAttribArray(textureCoordsAttribIdO);
        glVertexAttribPointer(textureCoordsAttribIdO, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // provide the vertex normals to the shaders for sphere
        glBindBuffer(GL_ARRAY_BUFFER, normals_vboO);
        glEnableVertexAttribArray(normalAttribIdO);
        glVertexAttribPointer(normalAttribIdO, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferO);
       glDrawElements(GL_TRIANGLES, numVerticesS, GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(positionAttribIdO);
        glDisableVertexAttribArray(textureCoordsAttribIdO);
        glDisableVertexAttribArray(normalAttribIdO);
        glDisableVertexAttribArray(colourAttribIdO);


       // draogn object
       GLint positionAttribIdT = glGetAttribLocation(programId, "position");
       GLint colourAttribIdT = glGetAttribLocation(programId, "colour");
       GLint textureCoordsAttribIdT = glGetAttribLocation(programId, "textureCoords");
       GLint normalAttribIdT = glGetAttribLocation(programId, "normal");


       // provide the vertex positions to the shaders for dragon
       glBindBuffer(GL_ARRAY_BUFFER, positions_vboT);
       glEnableVertexAttribArray(positionAttribIdT);
       glVertexAttribPointer(positionAttribIdT, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

       // provide the vertex texture coordinates to the shaders for dragon
       glBindBuffer(GL_ARRAY_BUFFER, textureCoords_vboT);
       glEnableVertexAttribArray(textureCoordsAttribIdT);
       glVertexAttribPointer(textureCoordsAttribIdT, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

       // provide the vertex normals to the shaders for dragon
       glBindBuffer(GL_ARRAY_BUFFER, normals_vboT);
       glEnableVertexAttribArray(normalAttribIdT);
       glVertexAttribPointer(normalAttribIdT, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

       // draw dragon
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferT);
       glDrawElements(GL_TRIANGLES, numVerticesD, GL_UNSIGNED_INT, (void*)0);

       // disable the attribute arrays
        glDisableVertexAttribArray(positionAttribIdO);
        glDisableVertexAttribArray(positionAttribIdT);
        glDisableVertexAttribArray(textureCoordsAttribIdT);
        glDisableVertexAttribArray(normalAttribIdT);
        glDisableVertexAttribArray(colourAttribIdT);



	// make the draw buffer to display buffer (i.e. display what we have drawn)
	glutSwapBuffers();
  //glutSolidTorus(innerRadius,outerRadius,sides,rings);
}

static void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    width = w;
    height = h;
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
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Raptors LOGO");
  glClearColor(1,1,1,1);

  glutIdleFunc(&update);
  glutDisplayFunc(&render);
  glutReshapeFunc(&reshape);
  glutMotionFunc(&drag);
  glutMouseFunc(&mouse);
  glutKeyboardFunc(&keyboard);

  //glutSolidTorus(innerRadius,outerRadius,sides,rings);


  glewInit();
    if (!GLEW_VERSION_2_0) {
        std::cerr << "OpenGL 2.0 not available" << std::endl;
        return 1;
    }
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

    createGeometry();

    createTexture("textures/basketball.jpg");

    ShaderProgram program;
    program.loadShaders("shaders/vertex.glsl", "shaders/fragment.glsl");
  	programId = program.getProgramId();

    glutMainLoop();

    return 0;
}
