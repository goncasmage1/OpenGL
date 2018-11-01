///////////////////////////////////////////////////////////////////////
//
//  Loading OBJ mesh from external file
//
//	Final individual assignment:
//	1.	Create classes: Scene, Camera and Mesh (that loads a mesh from
//		a Wavefront OBJ file to an indexed format) and build a small
//		scenegraph of your tangram scene (you may create more 
//		classes if needed).
//	2.	Create a ground object and couple the tangram figure to the
//		ground. Press keys to move the ground about: the tangram
//		figure must follow the ground.
//	3.	Animate between closed puzzle (initial square) and tangram
//		figure by pressing a key.
//	4.	Spherical camera interaction through mouse. It should be
//		possible to interact while animation is playing.
//
//	Team assignment:
//	Pick your team (2 elements from one same lab) for the team
//	assignment you will be working until the end of the semester,
//	and register it online.
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
#include <cassert>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Shader/ShaderProgram.h"
#include "Input.h"
#include "Camera.h"
#include "MeshLoader.h"
#include "Mesh.h"

#define CAPTION "Hello Modern 2D World"

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

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
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { base, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { base + offset, height, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { base + offset, height, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { offset, height, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },
#define PARALELOGRAM_BACK(base, height, offset, color_r, color_g, color_b) \
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { base + offset, height, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { base, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { 0.0f, 0.0f, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { offset, height, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } }, \
	{ { base + offset, height, 0.0f, 1.0f }, { color_r, color_g, color_b, 1.0f } },

int WinX = 800, WinY = 800;
int WindowHandle = 0;
unsigned int FrameCount = 0;

GLuint VaoId, VboId[2];

std::shared_ptr<ShaderProgram> ShaderProg = nullptr;
std::shared_ptr<Input> input = std::make_shared<Input>();
std::shared_ptr<Camera> camera = std::make_shared<Camera>();
std::shared_ptr<MeshLoader> meshLoader = std::make_shared<MeshLoader>();

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
		std::cout << std::endl << "Press <return>.";
		std::cin.ignore(1);
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram()
{
	std::vector<ShaderAttribute> Attributes = { {VERTICES, "in_Position"},
												{TEXCOORDS, "inTexcoord"},
												{NORMALS, "inNormal"}
												};
	std::vector<std::string> ShaderPaths = { "src/Shader/VertexShader.vert",
											 "src/Shader/FragmentShader.frag"
											};

	ShaderProg = std::make_shared<ShaderProgram>(Attributes, ShaderPaths);

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	ShaderProg->Destroy();
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

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
		Mat4::ScaleMat(1.08),
		Mat4::TranslationMat(Vec3(-0.375, 0.75, 0)),
	},
	//Triangle 1 back
	{
		Mat4::RotationMat(Vec3::Z(), 180),
		Mat4::TranslationMat(Vec3(0.25, 0.5, 0)),
		Mat4::ScaleMat(1.5),
	},
	//Triangle 2 back
	{
		Mat4::RotationMat(Vec3::Z(), 90),
		Mat4::TranslationMat(Vec3(0.25, 0, 0)),
		Mat4::ScaleMat(1.5),
	},
	//Triangle 3 back
	{
		Mat4::RotationMat(Vec3::Z(), 135),
		Mat4::ScaleMat(1.06)
	},
	//Triangle 4 back
	{
		Mat4::RotationMat(Vec3::Z(), -90),
		Mat4::ScaleMat(0.75),
		Mat4::TranslationMat(Vec3(-0.25*0.75, 0.562, 0)),
	},
	//Triangle 5 back
	{
		Mat4::ScaleMat(0.75),
	},
	//Square 1.1 back
	{
		Mat4::RotationMat(Vec3::Z(), 45),
		Mat4::ScaleMat(1.06),
	},
	//Square 1.2 back
	{
		Mat4::RotationMat(Vec3::Z(), 45),
		Mat4::ScaleMat(1.06),
	},
	//Paralelogram 1.1 back
	{
		Mat4::RotationMat(Vec3::Z(), -90),
		Mat4::ScaleMat(1.07),
		Mat4::TranslationMat(Vec3(-0.375, 0.75, 0)),
	},
	//Paralelogram 1.2 back
	{
		Mat4::RotationMat(Vec3::Z(), -90),
		Mat4::ScaleMat(1.08),
		Mat4::TranslationMat(Vec3(-0.375, 0.75, 0)),
	},
};

void createBufferObjects()
{
	GLuint VboVertices, VboTexcoords, VboNormals;

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glGenBuffers(1, &VboVertices);
			glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
			glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vec3), &Vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);

			if (TexcoordsLoaded)
			{
				glGenBuffers(1, &VboTexcoords);
				glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
				glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Vec2), &Texcoords[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(TEXCOORDS);
				glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), 0);
			}
			if (NormalsLoaded)
			{
				glGenBuffers(1, &VboNormals);
				glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
				glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Vec3), &Normals[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(NORMALS);
				glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), 0);
			}
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboVertices);
	glDeleteBuffers(1, &VboTexcoords);
	glDeleteBuffers(1, &VboNormals);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{
	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Mat4), camera->GetViewMatrix().GetData());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Mat4), sizeof(Mat4), camera->GetProjectionMatrix(input->IsPDown()).GetData());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(VaoId);
	glUseProgram(ShaderProg->GetProgramId());

	GLint UniformId = ShaderProg->GetUniformId("Transformation");

	//TODO: Draw all vertices at once
	uint32_t counter = 0;
	for (std::shared_ptr<Mesh> mesh : meshLoader->Meshes)
	{
		for (int i = 0; i < (mesh->Vertices.size() / 3); i++)
		{
			glUniformMatrix4fv(UniformId, 1, GL_FALSE, Mat4::IdentityMat().GetData());
			glUniformMatrix4fv(ShaderProg->GetUniformId("ModelMatrix"), 1, GL_FALSE, camera->GetModelMatrix().GetData());
			glDrawArrays(GL_TRIANGLES, counter, 3);
			counter += 3;
		}
	}

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

void processInput()
{
	if (input->IsMiddleMouseButtonDown() != camera->IsOrbiting()) camera->ToggleOrbiting();
	if (input->IsGDown() != camera->UsingQuaternion()) camera->ToggleQuaternion();

	camera->RotateCamera(input->GetMouseDelta());
	//camera->MoveCamera(input->GetMovement());
	camera->Zoom(input->GetWheelDelta());
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
	processInput();
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

void keyboardButtontDown(unsigned char Key, int x, int y)
{
	input->keyboardButtontDown(Key);
}

void keyboardButtonUp(unsigned char Key, int x, int y)
{
	input->keyboardButtonUp(Key);
}

void mouseButton(int button, int state, int x, int y)
{
	input->mouseButton(button, state);
}

void mouseMove(int x, int y)
{
	input->mouseMove(x, y);
}

void mouseWheel(int wheel, int direction, int x, int y)
{
	input->mouseWheel(direction);
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
	glutKeyboardFunc(keyboardButtontDown);
	glutKeyboardUpFunc(keyboardButtonUp);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutMouseWheelFunc(mouseWheel);
	glutPassiveMotionFunc(mouseMove);
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

	meshLoader->CreateMesh(std::string("../../assets/models/Solid Snake.obj"));

	/*createShaderProgram(std::string("../../projects/ModernOpenGL/src/Introduction/shaders/cube_vs.glsl"),
						std::string("../../projects/ModernOpenGL/src/Introduction/shaders/cube_fs.glsl"));*/
	createShaderProgram();
	createBufferObjects();
}

void q1test()
{
	Quat q = FromAngleAxis(Vec4(0.0f, 1.0f, 0.0f, 1.0f), 90.0f);
	std::cout << "q " << q << std::endl;

	Quat vi = { 0.0f, 7.0f, 0.0f, 0.0f };
	std::cout << "vi " << vi << std::endl;

	Quat qe = { 0.0f, 0.0f, 0.0f, -7.0f };
	std::cout << "qe " << qe << std::endl;

	Quat qinv = Inversed(q);
	qinv.Clean();
	std::cout << "qinv " << qinv << std::endl;

	Quat qf = (q * vi) * qinv;
	std::cout << "qf " << qf << std::endl;

	assert(qf == qe);
}

void q2test()
{
	Quat q = FromAngleAxis(Vec4(0.0f, 1.0f, 0.0f, 1.0f), 90.0f);
	std::cout << "q " << q << std::endl;

	Vec4 vi = { 7.0f, 0.0f, 0.0f, 0.0f };
	std::cout << "vi " << vi << std::endl;

	//Vec4 ve = { 0.0f, 0.0f, -7.0f, 1.0f };
	Vec4 ve = { 0.0f, 0.0f, -7.0f, 0.0f };
	std::cout << "ve " << ve << std::endl;

	Mat4 m = q.GetMatrix();
	std::cout << "m " << m << std::endl;

	Vec4 vf = m * vi;
	std::cout << "vf " << vf << std::endl;

	assert(vf == ve);
}

void q3test()
{
	Vec4 axis_x = { 1.0f, 0.0f, 0.0f, 1.0f };
	Vec4 axis_y = { 0.0f, 1.0f, 0.0f, 1.0f };
	Vec4 axis_z = { 0.0f, 0.0f, 1.0f, 1.0f };

	Quat qyaw900 = FromAngleAxis(axis_y, 900.0f);
	std::cout << "qyaw900 " << qyaw900 << std::endl;

	Quat qroll180 = FromAngleAxis(axis_x, 180.0f);
	std::cout << "qroll180 " << qroll180 << std::endl;
	Quat qpitch180 = FromAngleAxis(axis_z, 180.0f);
	std::cout << "qpitch180 " << qpitch180 << std::endl;
	Quat qrp = qpitch180 * qroll180;
	std::cout << "qrp " << qrp << std::endl;
	Quat qpr = qroll180 * qpitch180;
	std::cout << "qpr " << qpr << std::endl;

	Quat qi = { 0.0f, 1.0f, 0.0f, 0.0f }; // x-axis
	std::cout << "qi " << qi << std::endl;
	Quat qe = { 0.0f, -1.0f, 0.0f, 0.0f };
	std::cout << "qe " << qe << std::endl;

	Quat qyaw900inv = Inversed(qyaw900);
	Quat qfy = (qyaw900 * qi) * qyaw900inv;
	std::cout << "qfy " << qfy << std::endl;
	assert(qe == qfy);

	Quat qrpinv = Inversed(qrp);
	Quat qfrp = (qrp * qi) * qrpinv;
	std::cout << "qfrp " << qfrp << std::endl;
	assert(qe == qfrp);

	Quat qprinv = Inversed(qpr);
	Quat qfpr = (qpr * qi) * qprinv;
	std::cout << "qfpr " << qfpr << std::endl;
	assert(qe == qfpr);
}

void q4test()
{
	float thetai = 45.0f;
	Vec4 axis_i = { 0.0f, 1.0f, 0.0f, 1.0f };
	Quat q = FromAngleAxis(axis_i, thetai);
	std::cout << thetai << " ";
	std::cout << "axis_i " << axis_i << std::endl;

	float thetaf;
	Vec4 axis_f;
	ToAngleAxis(q, thetaf, axis_f);
	std::cout << thetaf << " ";
	std::cout << "thetaf " << thetaf << std::endl;

	bool b1 = (std::abs(thetai - thetaf) < 0.00001f);
	bool b2 = axis_i == axis_f;
	assert(b1 && b2);
}

void q5test()
{
	Vec4 axis = { 0.0f, 1.0f, 0.0f, 1.0f };
	Quat q0 = FromAngleAxis(axis, 0.0f);
	std::cout << "q0 " << q0 << std::endl;
	Quat q1 = FromAngleAxis(axis, 90.0f);
	std::cout << "q1 " << q1 << std::endl;
	Quat qe = FromAngleAxis(axis, 30.0f);
	std::cout << "qe " << qe << std::endl;

	Quat qLerp0 = Lerp(q0, q1, 0.0f);
	std::cout << "lerp(0) " << qLerp0 << std::endl;
	assert(qLerp0 == q0);

	Quat qLerp1 = Lerp(q0, q1, 1.0f);
	std::cout << "lerp(1) " << qLerp1 << std::endl;
	assert(qLerp1 == q1);

	Quat qlerp = Lerp(q0, q1, 1 / 3.0f);
	std::cout << "lerp(1/3) " << qlerp << std::endl;

	assert(qlerp != qe);
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();

	//q1test();
	//q2test();
	//q3test();
	//q4test();
	//q5test();

	std::cin.ignore(1);
	exit(EXIT_SUCCESS);
}