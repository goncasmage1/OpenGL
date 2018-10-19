///////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Modern OpenGL.
// A "hello world" of Modern OpenGL.
//
// Assignment : Create Shader Abstraction
//					(e.g. check compilation/linkage for errors, read from file) 
//			  : Manage Multiple Drawable Entities (using your vertex and matrix classes)
//              Draw a set of 7 TANs (i.e. TANGRAM shapes) of different colors: 
//              (1) 3 different TAN geometric shapes at the origin:
//					- right triangle
//					- square
//					- parallelogram
//              (2) 7 TANs of different colors put together to form a shape of your choice:
//					- 2 big right triangles
//					- 1 medium right triangle
//					- 2 small right triangles
//					- 1 square
//					- 1 parallelogram;
//
// (c) 2013-18 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Math/Public/Vector.h"
#include "Math/Public/Matrix.h"
#include "ShaderProgram.h"

#define CAPTION "Hello Modern 2D World"

int WinX = 800, WinY = 800;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTICES 0
#define COLORS 1

GLuint VaoId, VboId[2];

ShaderProgram ShaderProg;
//std::shared_ptr<ShaderProgram> ShaderProg;

/////////////////////////////////////////////////////////////////////// ERRORS

static std::string errorType(GLenum type)
{
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:				return "error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
		case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
		case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
		case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
		case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
		default:								exit(EXIT_FAILURE);
	}
}

static std::string errorSource(GLenum source)
{
	switch (source)
	{
		case GL_DEBUG_SOURCE_API:				return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
		case GL_DEBUG_SOURCE_APPLICATION:		return "application";
		case GL_DEBUG_SOURCE_OTHER:				return "other";
		default:								exit(EXIT_FAILURE);
	}
}

static std::string errorSeverity(GLenum severity)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:			return "high";
		case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
		case GL_DEBUG_SEVERITY_LOW:				return "low";
		case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
		default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
				  const GLchar *message, const void *userParam)
{
	std::cerr << "ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
}

void setupErrors()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	// params: source, type, severity, count, ids, enabled
}

static bool isOpenGLError()
{
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR)
	{
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError())
	{
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram()
{
	ShaderProg = ShaderProgram();
	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	ShaderProg.Destroy();
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct
{
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] =
{
	//Triangle 1
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	{{ 0.50f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	//Triangle 2
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{ 0.50f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	//Triangle 3
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
	{{ 0.50f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
	//Triangle 4
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }},
	{{ 0.50f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }},
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }},
	//Triangle 5
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }},
	{{ 0.50f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }},
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }},
	//Square 1.1
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }},
	{{ 0.25f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }},
	{{ 0.00f, 0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }},
	//Square 1.2
	{{ 0.25f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }},
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }},
	{{ 0.00f, 0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }},
	//Paralelogram 1.1
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }},
	{{ 0.35f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }},
	{{ 0.525f, 0.175f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }},
	//Paralelogram 1.2
	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }},
	{{ 0.525f, 0.175f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }},
	{{ 0.175f, 0.175f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }},
};

const std::vector<Mat4> Mats[] = {
	//Triangle 1
	{
		Mat4::RotationMat(Vec3::Z(), 180),
		Mat4::TranslationMat(Vec3(0.25, 0.5, 0)),
		Mat4::ScaleMat(1.5),
	},
	//Triangle 2
	{
		Mat4::RotationMat(Vec3::Z(), 90),
		Mat4::TranslationMat(Vec3(0.25, 0, 0)),
		Mat4::ScaleMat(1.5),
	},
	//Triangle 3
	{
		Mat4::RotationMat(Vec3::Z(), 135),
		Mat4::ScaleMat(1.06)
	},
	//Triangle 4
	{
		Mat4::RotationMat(Vec3::Z(), -90),
		Mat4::ScaleMat(0.75),
		Mat4::TranslationMat(Vec3(-0.25*0.75, 0.562, 0)),
	},
	//Triangle 5
	{
		Mat4::ScaleMat(0.75),
	},
	//Square 1.1
	{
		Mat4::RotationMat(Vec3::Z(), 45),
		Mat4::ScaleMat(1.06),
	},
	//Square 1.2
	{
		Mat4::RotationMat(Vec3::Z(), 45),
		Mat4::ScaleMat(1.06),
	},
	//Paralelogram 1.1
	{
		Mat4::RotationMat(Vec3::Z(), -90),
		Mat4::ScaleMat(1.07),
		Mat4::TranslationMat(Vec3(-0.375, 0.75, 0)),
	},
	//Paralelogram 1.2
	{
		Mat4::RotationMat(Vec3::Z(), -90),
		Mat4::ScaleMat(1.07),
		Mat4::TranslationMat(Vec3(-0.375, 0.75, 0)),
	}
};

const GLubyte* BuildIndices()
{
	int size = (sizeof(Vertices) / sizeof(*Vertices));
	GLubyte* arr = new GLubyte[size];

	for (int i = 0; i < size; i++) arr[i] = i;

	return arr;
}

const GLubyte* Indices = BuildIndices();

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * (sizeof(Vertices) / sizeof(*Vertices)), Indices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{
	glBindVertexArray(VaoId);
	glUseProgram(ShaderProg.GetProgramId());

	uint64_t counter = 0;

	for (int i = 0; i < ((sizeof(Vertices) / sizeof(*Vertices) / 3)); i++)
	{
		Mat4 Result = Mat4::IdentityMat();
		for (size_t j = 0; j < Mats[i].size(); j++)
		{
			Result = Mats[i][j] * Result;
		}

		glUniformMatrix4fv(ShaderProg.GetUniformId("Matrix"), 1, GL_FALSE, Result.GetData());
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)counter);
		counter += 3;
	}

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
	delete Indices;
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	setupErrors();
}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextFlags(GLUT_DEBUG);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1)
	{
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
	createShaderProgram();
	createBufferObjects();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////