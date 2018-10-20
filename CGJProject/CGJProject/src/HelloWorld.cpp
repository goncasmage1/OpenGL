///////////////////////////////////////////////////////////////////////
//
// Assignment consists in the following:
//
// - Create the following changes to your scene:
//   - Make your TANs double-faced, so they can be seen from both sides.
//   - The new face of each TAN should share the same hue as the
//     original top face color but have a different level of saturation 
//     and brightness.
//
// - Add the following functionality:
//   - Create a View Matrix from (eye, center, up) parameters.
//   - Create an Orthographic Projection Matrix from (left-right, 
//     bottom-top, near-far) parameters.
//   - Create a Perspective Projection Matrix from (fovy, aspect,
//     nearZ, farZ) parameters.
//
// - Add the following dynamics to the application:
//   - Create a free 3D camera controlled by the mouse allowing to 
//     visualize the scene through all its angles.
//   - Change perspective from orthographic to perspective and back as
//     a response to pressing the key 'p'.
//
// (c) 2013-18 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Math/Public/Vector.h"
#include "Math/Public/Matrix.h"
#include "Shader/ShaderProgram.h"
#include "Input.h"

#define CAPTION "Hello Modern 2D World"

#define VERTICES 0
#define COLORS 1

#define CENTERED_RIGHT_TRIANGLE(base, height, color_r, color_g, color_b) \
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { base, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { base/2, height, 0.0f, 1.0f }, {color_r, color_g, color_b, 1.0f } },
#define CENTERED_RIGHT_TRIANGLE_BACK(base, height, color_r, color_g, color_b) \
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { base/2, height, 0.0f, 1.0f }, {color_r, color_g, color_b, 1.0f } },\
	{ { base, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },

#define LEFT_LEAN_RIGHT_TRIANGLE(base, height, color_r, color_g, color_b) \
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { base, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { 0.f, height, 0.0f, 1.0f }, {color_r, color_g, color_b, 1.0f } },
#define LEFT_LEAN_RIGHT_TRIANGLE_BACK(base, height, color_r, color_g, color_b) \
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { 0.f, height, 0.0f, 1.0f }, {color_r, color_g, color_b, 1.0f } },\
	{ { base, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },

#define RIGHT_LEAN_RIGHT_TRIANGLE(base, height, color_r, color_g, color_b) \
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { base, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { base, height, 0.0f, 1.0f }, {color_r, color_g, color_b, 1.0f } },
#define RIGHT_LEAN_RIGHT_TRIANGLE_BACK(base, height, color_r, color_g, color_b) \
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { base, height, 0.0f, 1.0f }, {color_r, color_g, color_b, 1.0f } },\
	{ { base, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },

#define SQUARE(side, color_r, color_g, color_b) \
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f }},\
	{ { side, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { 0.00f, side, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { side, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { side, side, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { 0.00f, side, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },
#define SQUARE_BACK(side, color_r, color_g, color_b) \
	{{ 0.00f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f }},\
	{ { 0.00f, side, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { side, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { side, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { 0.00f, side, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },\
	{ { side, side, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },

#define PARALELOGRAM(base, height, offset, color_r, color_g, color_b) \
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { base, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { base + offset, height, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { base + offset, height, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { offset, height, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
#define PARALELOGRAM_BACK(base, height, offset, color_r, color_g, color_b) \
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { base + offset, height, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { base, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { offset, height, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, \
	{ { base + offset, height, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

int WinX = 800, WinY = 800;
int WindowHandle = 0;
unsigned int FrameCount = 0;

GLuint VaoId, VboId[2];

std::shared_ptr<ShaderProgram> ShaderProg = nullptr;
std::shared_ptr<Input> input = std::make_shared<Input>();

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
	std::vector<ShaderAttribute> Attributes = { {VERTICES, "in_Position"},
												{COLORS, "in_Color"} };
	ShaderProg = std::make_shared<ShaderProgram>(Attributes);

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	ShaderProg->Destroy();
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
	CENTERED_RIGHT_TRIANGLE(0.5f, 0.25f, 1.0f, 0.0f, 0.0f)
	//Triangle 2
	CENTERED_RIGHT_TRIANGLE(0.5f, 0.25f, 0.0f, 1.0f, 0.0f)
	//Triangle 3
	CENTERED_RIGHT_TRIANGLE(0.5f, 0.25f, 0.0f, 0.0f, 1.0f)
	//Triangle 4
	CENTERED_RIGHT_TRIANGLE(0.5f, 0.25f, 1.0f, 1.0f, 0.0f)
	//Triangle 5
	CENTERED_RIGHT_TRIANGLE(0.5f, 0.25f, 1.0f, 0.0f, 1.0f)
	//Square
	SQUARE(0.25f, 0.f, 1.f, 1.f)
	//Paralelogram
	PARALELOGRAM(0.35f, 0.175f, 0.175f, 1.f, 1.f, 1.f)
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
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	{
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, ShaderProg->GetUBO_BP(), VboId[1]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs, VBOs and UBOs.");
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

Mat4 ModelMat1 = Mat4::Model(Vec3(0.5, 0.5, 0.5));
Mat4 ModelMat2 = Mat4::Model(Vec3(2.5, 2.5, 2.5));

Mat4 ViewMat1 = Mat4::View(Vec3(5 ,5 ,5), Vec3(0, 0, 0), Vec3(0, 1, 0));
Mat4 ViewMat2 = Mat4::View(Vec3(-5, -5, -5), Vec3(0, 0, 0), Vec3(0, 1, 0));

Mat4 ProjMat1 = Mat4::Orthographic(1, 10, -2, 2, -2, 2);
Mat4 ProjMat2 = Mat4::Projection(30, 640/480, 1, 10);

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{
	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), ViewMat2.GetData());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mat4), sizeof(Mat4), ProjMat1.GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(VaoId);
	glUseProgram(ShaderProg->GetProgramId());

	/*for (int i = 0; i < ((sizeof(Vertices) / sizeof(*Vertices) / 3)); i++)
	{
		Mat4 Result = Mat4::IdentityMat();
		for (size_t j = 0; j < Mats[i].size(); j++)
		{
			Result = Mats[i][j] * Result;
		}

		glUniformMatrix4fv(UniformId, 1, GL_FALSE, Result.GetData());
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)counter);
		counter += 3;
	}*/

	glUniformMatrix4fv(ShaderProg->GetUniformId("ModelMatrix"), 1, GL_FALSE, ModelMat1.GetData());
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS



void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
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

void keyboardInputDown(unsigned char Key, int x, int y)
{
	input->keyboardInputDown(Key, x, y);
}

void keyboardInputUp(unsigned char Key, int x, int y)
{
	input->keyboardInputUp(Key, x, y);
}

void mouseInput(int button, int state, int x, int y)
{
	input->mouseInput(button, state, x, y);
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
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(keyboardInputDown);
	glutKeyboardUpFunc(keyboardInputUp);
	glutMouseFunc(mouseInput);
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