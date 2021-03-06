#include <string>
#include <iostream>
#include <fstream>
#include <stack>
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

int windowId = 0;

// projection matrix - perspective projection
glm::mat4 projectionMatrix;

// view matrix - orient everything around our preferred view
glm::mat4 viewMatrix;

GLuint programId;
GLuint vertexBuffer;
GLuint indexBuffer;
GLenum positionBufferId;

float xRotation = 0.0f;
float yRotation = 0.0f;
float zRotation = 0.0f;

float yRotationSpeed = 0.1f;

float minArmRotation = 45.0f;
float maxArmRotation = 160.0f;
float upperArmRotation = 45.0f;
float upperArmRotationSpeed = 1.0f;

void drawLetter(glm::vec3 position, glm::vec3 scale, float angle) ;
void drawElongatedCube(glm::mat4 modelMatrix, glm::vec4 colour);
static GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
static GLuint createShader(const GLenum shaderType, const std::string shaderSource);

static const GLfloat vertexPositionData[] = {
    -1.0f, -1.0f,  1.0f,  // front
     1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,  // back
     1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f
};
static const GLushort indexData[] = {
  0, 1, 2,   // front
  3, 2, 1,
  5, 6, 7,   // back
  5, 4, 6,
  1, 7, 3,   // right
  1, 5, 7,
  4, 0, 2,   // left
  4, 2, 6,
  2, 7, 6,   // top
  2, 3, 7,
  0, 4, 5,   // bottom
  0, 5, 1
};
int numVertices = 36;

static void createGeometry(void) {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionData), vertexPositionData, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
}

static void update(void) {
    int milliseconds = glutGet(GLUT_ELAPSED_TIME);

    // rotate the entire model, to that we can examine it
    yRotation += yRotationSpeed;

    glutPostRedisplay();
}

static void render(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // turn on depth buffering
  glEnable(GL_DEPTH_TEST);

  // activate our shader program
  glUseProgram(programId);

  // colours
  glm::vec4 red(0.8, 0.0, 0.0, 1.0);

  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(2.4f, 2.4f, 0.5f));
  modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -1.5f, 0.0f));

  // (x, y, z)

  glm::vec3 bp = glm::vec3(glm::vec3(0.0f, 0.0f, 0.0f));
  // this code does the scaling
  glm::vec3 bs = glm::vec3(glm::vec3(0.0f, 0.0f, 0.25f));
  glm::vec3 br = glm::vec3(glm::vec3(0.0f, 0.0f, 0.0f));
  float angle = 0.0f;

  // basic
  //drawLetter(glm::vec3(bp.x, bp.y, bp.z), glm::vec3(bs.x + 0.5, bs.y + 0.5, bs.z), 0);  //

  // -------------------------- TORONTO -------------------------------------------------------

  // T
  drawLetter(glm::vec3(bp.x - 10, bp.y + 6, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 10, bp.y + 4.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |

  // O
  drawLetter(glm::vec3(bp.x - 8, bp.y + 4.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 6.4, bp.y + 4.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 7.2, bp.y + 6, bp.z), glm::vec3(bs.x + 1.1, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 7.2, bp.y + 3, bp.z), glm::vec3(bs.x + 1.1, bs.y + 0.3, bs.z), angle);  // -

  // R
  drawLetter(glm::vec3(bp.x - 4.8, bp.y + 4.5, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.8, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 4.1, bp.y + 6, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 4.1, bp.y + 4.5, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 3.4, bp.y + 5.3, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 3.7, bp.y + 3.7, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1, bs.z), angle + 25);  // |

  // O
  drawLetter(glm::vec3(bp.x - 1.8, bp.y + 4.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 0.2, bp.y + 4.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 1.0, bp.y + 6, bp.z), glm::vec3(bs.x + 1.1, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 1.0, bp.y + 3, bp.z), glm::vec3(bs.x + 1.1, bs.y + 0.3, bs.z), angle);  // -

  // N
  drawLetter(glm::vec3(bp.x + 1.4, bp.y + 4.5, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.8, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 3.2, bp.y + 4.5, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.8, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 2.3, bp.y + 4.5, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.9, bs.z), angle + 28);  // |

  // T
  drawLetter(glm::vec3(bp.x + 5.2, bp.y + 6, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x + 5.2, bp.y + 4.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |

  // O
  drawLetter(glm::vec3(bp.x + 7.2, bp.y + 4.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 8.8, bp.y + 4.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 8, bp.y + 6, bp.z), glm::vec3(bs.x + 1.1, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x + 8, bp.y + 3, bp.z), glm::vec3(bs.x + 1.1, bs.y + 0.3, bs.z), angle);  // -



  // ------------------------------RAPTORS--------------------------------------
  // R
  drawLetter(glm::vec3(bp.x - 10.6, bp.y  -1, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.8, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 9.9, bp.y + 0.5, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 9.9, bp.y  -1, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 9.2, bp.y - 0.2, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 9.5, bp.y - 1.8, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1, bs.z), angle + 25);  // |

  // A
  drawLetter(glm::vec3(bp.x - 6, bp.y - 1, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.9, bs.z), angle + 18);  // |
  drawLetter(glm::vec3(bp.x - 7.3, bp.y - 1, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.9, bs.z), angle + 162);  // |
  drawLetter(glm::vec3(bp.x -6.6 , bp.y + 0.5 , bp.z), glm::vec3(bs.x + 0.3, bs.y + 0.4, bs.z), 0);  //
  drawLetter(glm::vec3(bp.x - 6.6, bp.y - 1.2, bp.z), glm::vec3(bs.x + 0.75, bs.y + 0.3, bs.z), angle);  // -

  // P
  drawLetter(glm::vec3(bp.x - 4, bp.y  -1, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.8, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x - 3.3, bp.y + 0.5, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 3.3, bp.y  -1, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 2.6, bp.y - 0.2, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1, bs.z), angle);  // |

  // T
  drawLetter(glm::vec3(bp.x - 0.6, bp.y + 0.5, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x - 0.6, bp.y + -1.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |

  // O
  drawLetter(glm::vec3(bp.x + 1.4, bp.y + -1.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 3, bp.y + -1.25, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.5, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 2.2, bp.y + 0.5, bp.z), glm::vec3(bs.x + 1.1, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x + 2.2, bp.y + -2.5, bp.z), glm::vec3(bs.x + 1.1, bs.y + 0.3, bs.z), angle);  // -

  // R
  drawLetter(glm::vec3(bp.x + 4.6, bp.y + -1, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.8, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 5.3, bp.y + 0.5, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x + 5.3, bp.y + -1, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x + 6, bp.y + -0.2, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 5.7, bp.y + -1.8, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1, bs.z), angle + 25);  // |

  // S
  drawLetter(glm::vec3(bp.x + 7.6, bp.y + -0.3, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1.0, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 8.3, bp.y + 0.5, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x + 8.3, bp.y + -1, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -
  drawLetter(glm::vec3(bp.x + 9, bp.y + -1.8, bp.z), glm::vec3(bs.x + 0.3, bs.y + 1, bs.z), angle);  // |
  drawLetter(glm::vec3(bp.x + 8.3, bp.y + -2.5, bp.z), glm::vec3(bs.x + 1.0, bs.y + 0.3, bs.z), angle);  // -



  // make the draw buffer to display buffer (i.e. display what we have drawn)
  glutSwapBuffers();
}

void drawLetter(glm::vec3 position, glm::vec3 scale, float angle)
{
  // colours
  glm::vec4 colour1(1.0, 1.0, 1.0, 1.0);

  // TODO:  Draw three transformed cubes to make the finger
  //Fingers
  glm::mat4 mat1 = glm::mat4(1.0f);
  mat1 = glm::translate(mat1, position);
  //glm::vec3(0.0f, 0.0f, 1.0f) is rotate in z axis!
  mat1 = glm::rotate(mat1, glm::radians(angle),  glm::vec3(0.0f, 0.0f, 1.0f));
  mat1 = glm::scale(mat1, scale);

  drawElongatedCube(mat1, colour1);

}

void drawElongatedCube(glm::mat4 modelMatrix, glm::vec4 colour) {
  // model-view-projection matrix
  glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
  GLuint mvpMatrixId = glGetUniformLocation(programId, "u_MVP");
  glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &mvp[0][0]);

  // cube colour
  GLuint colourId = glGetUniformLocation(programId, "u_colour");
  glUniform4fv(colourId, 1, (GLfloat*)&colour[0]);

  // enable the vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

  // configure the attribute array (the layout of the vertex buffer)
  glVertexAttribPointer(positionBufferId, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *)0);
  glEnableVertexAttribArray(positionBufferId);

  // draw the triangle strip
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_SHORT, (void*)0);

  // disable the attribute array
  glDisableVertexAttribArray(positionBufferId);
}

static void reshape(int width, int height) {
    float aspectRatio = (float)width / (float)height;
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);

    // if using perpsective projection, update projection matrix
    glViewport(0, 0, width, height);
}

static void drag(int x, int y) {
}

static void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    }
}

static void keyboard(unsigned char key, int x, int y) {
    if (key == 'r') {
      if (yRotationSpeed > 0.0) {
        yRotationSpeed = 0.0;
      } else {
        yRotationSpeed = 0.1;
      }
    } else if (key == 27) {
      glutDestroyWindow(windowId);
      exit(0);
    }
    std::cout << "Key pressed: " << key << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    windowId = glutCreateWindow("TORONTO-RAPTORS 3D TEXT!");
    glutIdleFunc(&update);
    glutDisplayFunc(&render);
    glutReshapeFunc(&reshape);
    glutMotionFunc(&drag);
    glutMouseFunc(&mouse);
    glutKeyboardFunc(&keyboard);

    glewInit();
    if (!GLEW_VERSION_2_0) {
        std::cerr << "OpenGL 2.0 not available" << std::endl;
        return 1;
    }
    std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

    createGeometry();
    programId = createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    // create the view matrix (position and orient the camera)
    viewMatrix = glm::lookAt(
        glm::vec3(0,0,25), // eye/camera location
        glm::vec3(0,0,0),    // where to look
        glm::vec3(0,1,0)     // up
    );

    glutMainLoop();

    return 0;
}

static GLuint createShader(const GLenum shaderType, const std::string shaderFilename) {
  // load the shader source code
  std::ifstream fileIn(shaderFilename.c_str());

	if (!fileIn.is_open()) {
		return -1;
	}

	std::string shaderSource;
	std::string line;
	while (getline(fileIn, line)) {
		shaderSource.append(line);
		shaderSource.append("\n");
	}

	const char* sourceCode = shaderSource.c_str();

	// create a shader with the specified source code
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &sourceCode, nullptr);

	// compile the shader
	glCompileShader(shaderId);

	// check if there were any compilation errors
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int errorLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLength);
		char *errorMessage = new char[errorLength];

		glGetShaderInfoLog(shaderId, errorLength, &errorLength, errorMessage);
		std::cout << "Shader compilation failed: " << errorMessage << std::endl;

		delete[] errorMessage;

		glDeleteShader(shaderId);

		return 0;
	}

	return shaderId;
}

static GLuint createShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
	// create and compile a shader for each
	GLuint vShaderId = createShader(GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	// create and link the shaders into a program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	// delete the shaders
	glDetachShader(programId, vShaderId);
	glDetachShader(programId, fShaderId);
	glDeleteShader(vShaderId);
	glDeleteShader(fShaderId);

	return programId;
}
