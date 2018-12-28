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
#include "Math/Quaternion.h"
#include "Shader/ShaderProgram.h"
#include "Shader/SailShader.h"
#include "Shader/TextureShader.h"
#include "Shader/WaterShader.h"
#include "Shader/WoodShader.h"
#include "Input.h"
#include "Camera.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "Scene.h"
#include "SceneNode.h"

#include "WaterFrameBuffer.h"
#include "Shader/SkyboxShader.h"
#include "SOIL.h"

#define CAPTION "Hello Modern 2D World"


int WinX = 1600, WinY = 900;
int WindowHandle = 0;
unsigned int FrameCount = 0;
float animationProgress = 0.f;

const float HEIGHT = 1.0f;

auto begin = std::chrono::steady_clock::now();

struct MeshData
{
	int MeshIndex = 0;
	int ShaderIndex = 0;
};

std::shared_ptr<Input> input = std::make_shared<Input>();
std::shared_ptr<Camera> camera = std::make_shared<Camera>(WinX, WinY, 90);
std::shared_ptr<MeshLoader> meshLoader = std::make_shared<MeshLoader>();
std::shared_ptr<Scene> scene = nullptr;
std::vector<std::shared_ptr<ShaderProgram>> shaders = std::vector<std::shared_ptr<ShaderProgram>>();

std::shared_ptr<WaterFrameBuffer> waterFBO = std::make_shared<WaterFrameBuffer>();
std::shared_ptr<WaterShader> water;
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
	//Instantiate specific ShaderProgram class
	/*shaders.push_back(std::make_shared<ShaderProgram>());

	//or send attributes explicitly
	shaders.push_back(std::make_shared<ShaderProgram>(
		std::vector<ShaderAttribute>{
		ShaderAttribute(0, "in_Position"),
			ShaderAttribute(1, "in_Coordinates"),
			ShaderAttribute(2, "in_Normal")
	},
		std::vector<std::string>{
			"src/Shader/GLSL/BrownShader.glsl",
				"src/Shader/GLSL/FragmentShader.glsl"
		}
		));

	shaders.push_back(std::make_shared<SailShader>());
	shaders.push_back(std::make_shared<WaterShader>());
	shaders.push_back(std::make_shared<WoodShader>());

	std::shared_ptr<SkyboxShader> skyboxShader = std::make_shared<SkyboxShader>();
	std::vector<const char*> faces = {
		"../../assets/Textures/sea/right.jpg",
		"../../assets/Textures/sea/left.jpg",
		"../../assets/Textures/sea/top.jpg",
		"../../assets/Textures/sea/bottom.jpg",
		"../../assets/Textures/sea/back.jpg",
		"../../assets/Textures/sea/front.jpg"
	};

	skyboxShader->LoadCubeMap(faces);
	shaders.push_back(skyboxShader);

	//Texture
	std::shared_ptr<TextureShader> NarutoShader = std::make_shared<TextureShader>();
	NarutoShader->SetTexture("../../assets/Textures/naruto_kun.png");
	shaders.push_back(NarutoShader);

	checkOpenGLError("ERROR: Could not create shaders.");*/

	//RTT Reflection
	std::shared_ptr<TextureShader> textureShader = std::make_shared<TextureShader>();
	textureShader->SetTexture(waterFBO->getReflectionTexture());
	shaders.push_back(textureShader);

	//Skybox Shader
	std::shared_ptr<SkyboxShader> skyboxShader = std::make_shared<SkyboxShader>();
	std::vector<const char*> faces = {
		"../../assets/Textures/sea/right.jpg",
		"../../assets/Textures/sea/left.jpg",
		"../../assets/Textures/sea/top.jpg",
		"../../assets/Textures/sea/bottom.jpg",
		"../../assets/Textures/sea/back.jpg",
		"../../assets/Textures/sea/front.jpg"
	};

	skyboxShader->LoadCubeMap(faces);
	shaders.push_back(skyboxShader);

	//Water Shader
	water = std::make_shared<WaterShader>();
	water->SetCamera(camera);
	water->SetFBO(waterFBO);
	// ------------------------ LIGHT ------------------------ 
	water->SetLightPosition(Vec3(0.0f, -10.0f, 0.0f)); //its a vector :) lol
	water->SetLightColour(Vec3(1.0f, 1.0f, 1.0f)); //white
	// ------------------------------------------------------- 
	shaders.push_back(water);

	//Texture 
	std::shared_ptr<TextureShader> NarutoShader = std::make_shared<TextureShader>();
	NarutoShader->SetTexture("../../assets/Textures/naruto_kun.png");
	shaders.push_back(NarutoShader);

	//RTT
	std::shared_ptr<TextureShader> textureRefractShader = std::make_shared<TextureShader>();
	textureRefractShader->SetTexture(waterFBO->getRefractionTexture());
	shaders.push_back(textureRefractShader);

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	for (int i = 0; i < shaders.size(); i++) shaders[i]->Destroy();

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
void processScene()
{
	Vec3 movementOffset = camera->GetCameraMovement();

	if (movementOffset != scene->root->transform.Position)
	{
		scene->root->transform.Position = movementOffset;
		scene->root->UpdateTransformationMatrix();
	}
}

void drawScene()
{

	glEnable(GL_CLIP_DISTANCE0);

	//Render Reflection
	waterFBO->bindReflectionFrameBuffer();
	std::vector<Vec3> pre = camera->FlipView(); //Set camera for reflection and Saves the previous camera settings
	Vec3 water_heightRefl = Vec3(0.0f, 0.0f, 0.0f) + camera->GetCameraMovement(); //FIXME Put me in a class
	processScene();
	scene->Draw(Vec4(0.0f, 1.0f, 0.0f, -water_heightRefl.y)); //draws everything upper the surface
	camera->UnflipView(pre); //Set previous Camera settings
	processScene();
	//
	waterFBO->unbindFrameBuffer();
	//Render Refraction
	waterFBO->bindRefractionFrameBuffer();
	Vec3 water_height = Vec3(0.0f, 0.5f, 0.0f) + camera->GetCameraMovement(); //FIXME Put me in a class
	scene->Draw(Vec4(0.0f, -1.0f, 0.0f, water_height.y)); //draws everything bellow the plane
	//
	waterFBO->unbindFrameBuffer();
	//Render Scene Normally
	glDisable(GL_CLIP_DISTANCE0);
	scene->Draw(Vec4(0.0f, -1.0f, 0.0f, 1000)); //after GL_CLIP disabled this should be redundant. Might depend on the graphic
	checkOpenGLError("ERROR: Could not draw scene.");
}


void processCamera()
{
	//if (camera->IsOrbiting() != input->IsMiddleMouseButtonDown()) camera->ToggleOrbiting();
	camera->RotateCamera(input->GetMouseDelta());
	camera->MoveCamera(input->GetMovement());
	//camera->Zoom(input->GetWheelDelta());

	Vec3 movementOffset = camera->GetCameraMovement();

	if (movementOffset != scene->root->transform.Position)
	{
		scene->root->transform.Position = movementOffset;
		scene->root->UpdateTransformationMatrix();
	}
}

void processInput()
{
	processCamera();
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
	waterFBO->cleanUp();
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

	/*meshLoader->CreateMesh(std::string("../../assets/models/skybox.obj"));

	scene->root->CreateNode(meshLoader->Meshes[0], Transform(), shaders[5]);*/

	//MeshLoader loads all necessary meshes
	meshLoader->CreateMesh(std::string("../../assets/models/sphere.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/skybox.obj"));
	//meshLoader->CreateMesh(std::string("../../assets/models/sphere.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/water_surface.obj"));

	//reflection check
	/*meshLoader->CreateMesh(std::string("../../assets/models/water_surface.obj"));
	//refraction check
	meshLoader->CreateMesh(std::string("../../assets/models/water_surface.obj"));

	meshLoader->CreateMesh(std::string("../../assets/models/sphere.obj"));*/
	//meshLoader->CreateQuadMesh(1.f, 6, 4);


	//Optionally indicate mesh and shader index to use for each SceneNode
	/*MeshData meshData[] = {
		{1, 0},
		{2, 1},
	};*/


	//Skybox must be the first to be drawn in the scene
	scene->root->CreateNode(meshLoader->Meshes[1], Transform(), shaders[1]);

	//reflection check
	//scene->root->CreateNode(meshLoader->Meshes[4], Transform(Vec3(6.0, 0.0, -4.0), Quat(), Vec3(0.5f, 0.5f, 0.5f)), shaders[0]);
	//refraction check
	//scene->root->CreateNode(meshLoader->Meshes[5], Transform(Vec3(0.0, 0.0, 0.0), Quat(), Vec3(0.5f, 0.5f, 0.5f)), shaders[4]);

	//Naruto
	scene->root->CreateNode(meshLoader->Meshes[0], Transform(Vec3(-2.0, 0.5, -2.0), Quat(), Vec3(2.0f, 2.0f, 2.0f)), shaders[3]);

	//Water
	scene->root->CreateNode(meshLoader->Meshes[2], Transform(Vec3(0.0, 0.0, 0.0), Quat(), Vec3(2.0f, 2.0f, 2.0f)), shaders[2]);

}

void setupFBO()
{
	//FIXME: Save window Settings and have a lower resolution for the water
	waterFBO->initializeWater(WinX, WinY);

}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();

	setupFBO();
	createShaderProgram();
	scene = std::make_shared<Scene>(camera);
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