#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <cassert>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Math/Vector.h"
//#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Shader/ShaderProgram.h"
#include "Input.h"
#include "Camera.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"

#define CAPTION "Hello Modern 2D World"

int WinX = 800, WinY = 800;
int WindowHandle = 0;
unsigned int FrameCount = 0;
float animationProgress = 0.f;

auto begin = std::chrono::steady_clock::now();

enum AnimationState
{
	Start,
	Lift,
	End
};

struct Animation
{
	Transform From;
	Transform To;
};

std::shared_ptr<ShaderProgram> ShaderProg = nullptr;
std::shared_ptr<Input> input = std::make_shared<Input>();
std::shared_ptr<Camera> camera = std::make_shared<Camera>(WinX, WinY, 90);
std::shared_ptr<MeshLoader> meshLoader = std::make_shared<MeshLoader>();
std::shared_ptr<Scene> scene = nullptr;
std::shared_ptr<SceneNode> table = nullptr;

AnimationState animationState = AnimationState::Start;

const std::vector<Animation> animations = {
	{
		//From Triangle 1
		Transform(
				Vec3(0.25f, 0.25f, 0.05f),
				FromAngleAxis(Vec4::Z(), 180.f),
				Vec3(1.5f)
				),
		//To Triangle 1
		Transform(
				Vec3(0.25f, 0.f, 0.05f),
				FromAngleAxis(Vec4::Z(), 180.f),
				Vec3(1.5f)
				)
	},
	{
		//From Triangle 2
		Transform(
				Vec3(0.25f, -0.25f, 0.05f),
				FromAngleAxis(Vec4::Z(), 90.f),
				Vec3(1.5f)
				),
		//To Triangle 2
		Transform(
				Vec3(0.25f, 0.f, 0.05f),
				FromAngleAxis(Vec4::Z(), 45.f),
				Vec3(1.5f)
				)
	},
	{
		//From Triangle 3
		Transform(
				Vec3(0.0f, -0.35f, 0.05f),
				FromAngleAxis(Vec4::Z(), 135.f),
				Vec3(1.06f)
				),
		//To Triangle 3
		Transform(
				Vec3(-0.25f, -0.5f*1.2f, 0.05f),
				Quat(),
				Vec3(1.06f)
				)
	},
	{
		//From Triangle 4
		Transform(
				Vec3(-0.25f, 0.25f, 0.05f),
				FromAngleAxis(Vec4::Z(), -90.f),
				Vec3(0.75f)
				),
		//To Triangle 4
		Transform(
				Vec3(-0.7f, -0.5f, 0.05f),
				FromAngleAxis(Vec4::Z(), -45.f),
				Vec3(0.75f)
				)
	},
	{
		//From Triangle 5
		Transform(
				Vec3(0.f, -0.5f, 0.05f),
				Quat(),
				Vec3(0.75f)
				),
		//To Triangle 5
		Transform(
				Vec3(0.35f, -0.85f, 0.05f),
				FromAngleAxis(Vec4::Z(), 45.f),
				Vec3(0.75f)
				)
	},
	{
		//From Square
		Transform(
				Vec3(0.f, -0.35f, 0.05f),
				FromAngleAxis(Vec4::Z(), 45.f),
				Vec3(1.06f)
				),
		//To Square
		Transform(
				Vec3(0.f, 0.f, 0.05f),
				FromAngleAxis(Vec4::Z(), 45.f),
				Vec3(1.06f)
				)
	},
	{
		//From Paralelogram
		Transform(
				Vec3(-0.35f, 0.35f, 0.05f),
				FromAngleAxis(Vec4::Z(), -90.f),
				Vec3(1.07f)
				),
		//To Paralelogram
		Transform(
				Vec3(-0.5f, 0.56f, 0.05f),
				FromAngleAxis(Vec4::Z(), -90.f),
				Vec3(1.07f)
				)
	},
};

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
	//TODO: Fix!
	std::vector<ShaderAttribute> Attributes = { {0, "in_Position"},
												{1, "in_Coordinates"},
												{2, "in_Normal"}
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

void createBufferObjects()
{
	meshLoader->CreateBufferObjects();
	camera->CreateBufferObjects();

	checkOpenGLError("ERROR: Could not create VAOs, VBOs and UBOs.");
}

void destroyBufferObjects()
{
	meshLoader->DestroyBufferObjects();
	camera->DestroyBufferObjects();

	checkOpenGLError("ERROR: Could not destroy VAOs, VBOs and UBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{
	scene->Draw();

	checkOpenGLError("ERROR: Could not draw scene.");
}

void subProcessAnimation(float progress)
{
	int i = 0;
	std::vector<std::shared_ptr<SceneNode>> children = table->childNodes;
	switch (animationState)
	{
		case Start:
			for (int i = 0; i < children.size(); i++)
			{
				children[i]->transform.Position.z = (i+1)*progress*0.1f + 0.05f;
				children[i]->UpdateTransformationMatrix();
			}
			break;
		case Lift:
			for (int i = 0; i < children.size(); i++)
			{
				float previousZ = children[i]->transform.Position.z;
				children[i]->transform.Position = Lerp(children[i]->startTransform.Position, children[i]->endTransform.Position, progress);
				children[i]->transform.Position.z = previousZ;
				children[i]->transform.Rotation = Lerp(children[i]->startTransform.Rotation, children[i]->endTransform.Rotation, progress);
				children[i]->UpdateTransformationMatrix();
			}
			break;
		case End:
			for (int i = 0; i < children.size(); i++)
			{
				children[i]->transform.Position.z = (i+1) * (1.f - progress) * 0.1f + 0.05f;
				children[i]->UpdateTransformationMatrix();
			}
			break;
		default:
			break;
	}
}

void processAnimation()
{
	float animationDelta = input->GetAnimationDelta();
	float deltaTime = (std::chrono::duration<float>(std::chrono::steady_clock::now() - begin)).count();
	if (animationDelta == 0.f) return;
	
	animationDelta *= deltaTime;

	if (animationDelta > 0 && (animationState != AnimationState::End || animationProgress < 1.f))
	{
		animationProgress += animationDelta;
		if (animationProgress > 1.f)
		{
			subProcessAnimation(1.f);
			if (animationState != AnimationState::End)
			{
				animationProgress = animationProgress - 1.f;
				animationState = static_cast<AnimationState>(static_cast<int>(animationState) + 1);
			}
			else
			{
				animationProgress = 1.f;
			}
		}
	}
	else if (animationDelta < 0 && (animationState != AnimationState::Start || animationProgress > 0.f))
	{
		animationProgress += animationDelta;
		if (animationProgress < 0.f)
		{
			subProcessAnimation(0.f);
			if (animationState != AnimationState::Start)
			{
				animationProgress = 1.f + animationProgress;
				animationState = static_cast<AnimationState>(static_cast<int>(animationState) - 1);
			}
			else
			{
				animationProgress = 0.f;
			}
		}
	}
	subProcessAnimation(animationProgress);
}

void processMovement()
{
	Vec3 movement = input->GetMovement();

	bool bChanged = false;

	if (movement.x != 0.f)
	{
		bChanged = true;
		table->transform.Position.x += movement.x;
	}
	if (movement.y != 0.f)
	{
		bChanged = true;
		table->transform.Position.y += movement.y;
	}
	if (movement.z != 0.f)
	{
		bChanged = true;
		table->transform.Position.z -= movement.z;
	}
	if (bChanged) table->UpdateTransformationMatrix();
}

void processInput()
{
	camera->RotateCamera(input->GetMouseDelta());
	camera->Zoom(input->GetWheelDelta());

	processMovement();

	processAnimation();	
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

	begin = std::chrono::steady_clock::now();
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

void setupMeshes()
{
	meshLoader->CreateMesh(std::string("../../assets/models/TableTri.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/CenteredRightTriangle.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/Square.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/Paralelogram.obj"));

	int meshIndices[] = {
		1,1,1,1,1,2,3
	};
	Vec4 colors[] = {
		Vec4(1.f, 0.f, 0.f, 1.f),
		Vec4(0.f, 1.f, 0.f, 1.f),
		Vec4(0.f, 0.f, 1.f, 1.f),
		Vec4(1.f, 1.f, 0.f, 1.f),
		Vec4(1.f, 0.f, 1.f, 1.f),
		Vec4(0.f, 1.f, 1.f, 1.f),
		Vec4(1.f, 1.f, 1.f, 1.f),
	};

	table = scene->root->CreateNode(meshLoader->Meshes[0], Transform(), Vec4(0.6f, 0.4f, 0.0f, 1.f), ShaderProg);

	for (int i = 0; i < (sizeof(meshIndices) / sizeof(*meshIndices)); i++)
	{
		std::shared_ptr<SceneNode> newNode = table->CreateNode(meshLoader->Meshes[meshIndices[i]], animations[i].From, colors[i], ShaderProg);
		newNode->startTransform = animations[i].From;
		newNode->endTransform = animations[i].To;
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
	createShaderProgram();
	scene = std::make_shared<Scene>(camera, ShaderProg);
	setupMeshes();
	createBufferObjects();
	begin = std::chrono::steady_clock::now();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	std::cin.ignore(1);
	exit(EXIT_SUCCESS);
}