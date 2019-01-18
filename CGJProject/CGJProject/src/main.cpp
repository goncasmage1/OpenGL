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
#include "Shader/PostProcessingShader.h"
#include "PPFilterMesh.h"
#include "Input.h"
#include "Camera.h"
#include "MeshLoader.h"
#include "Mesh.h"
#include "PPFilterMesh.h"
#include "Scene.h"
#include "SceneNode.h"

#include "Shader/RTT.h"
#include "WaterFrameBuffer.h"
#include "PostProcessingFrameBuffer.h"
#include "Shader/SkyboxShader.h"
#include "SOIL.h"
#include "WaterRenderer.h"

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

typedef struct Light{
	Vec3 Position;
	Vec3 Color;
} Light;



std::shared_ptr<Input> input = std::make_shared<Input>();
std::shared_ptr<Camera> camera = std::make_shared<Camera>(WinX, WinY, 90);
std::shared_ptr<MeshLoader> meshLoader = std::make_shared<MeshLoader>();
std::shared_ptr<Scene> scene = nullptr;
std::vector<std::shared_ptr<ShaderProgram>> shaders = std::vector<std::shared_ptr<ShaderProgram>>();
std::vector<std::shared_ptr<TextureShader>> foggy = std::vector<std::shared_ptr<TextureShader>>();
std::shared_ptr<SkyboxShader> skybox = nullptr;

Light sun;
std::shared_ptr<WaterFrameBuffer> waterFBO = std::make_shared<WaterFrameBuffer>();
std::shared_ptr<WaterShader> water = nullptr;

std::shared_ptr<WaterRenderer> waterRenderer = nullptr;

//Post-Processing
std::shared_ptr<PostProcessingFrameBuffer> ppFBO = std::make_shared<PostProcessingFrameBuffer>();
std::shared_ptr<PostProcessingShader> ppFilter = nullptr;
std::shared_ptr<PPFilterMesh> ppMesh = nullptr;

float RGBIntensity[3] = { 1.0f, 0.0f, 0.0f };
float distortionIntensity = 0.f;

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

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
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
	//Skybox Shader
	std::shared_ptr<SkyboxShader> skyboxShader = std::make_shared<SkyboxShader>(camera->GetViewMatrix());
	skybox = skyboxShader;
	shaders.push_back(skyboxShader);

	//RTT Reflection
	std::shared_ptr<RTT> textureShader = std::make_shared<RTT>(waterFBO->getReflectionTexture());
	shaders.push_back(textureShader);
	
	// ------------------------ LIGHT ------------------------ 
	sun.Position = Vec3(1.0f, 10.0f, 0.0f);
	sun.Color = Vec3(1.0f, 1.0f, 1.0f);
	// ------------------------------------------------------- 
	
	//Water Shader
	water = std::make_shared<WaterShader>(Vec3(0.0f, 0.0f, 0.0f), sun.Position, sun.Color);
	water->SetCamera(camera);
	water->SetFBO(waterFBO);
	water->SetSkyColor(Vec3(0.5f, 0.5f, 0.5f));
	shaders.push_back(water);

	////Texture Mountains
	std::shared_ptr<TextureShader> NarutoShader = std::make_shared<TextureShader>();
	NarutoShader->SetTexture("../../assets/Textures/rockDiff.jpg");
	NarutoShader->SetNormalTexture("../../assets/Textures/rockNormal.jpg");
	NarutoShader->SetLightPosition(sun.Position);
	NarutoShader->SetCamera(camera);
	NarutoShader->setSkyColor(Vec3(0.5f, 0.5f, 0.5f));
	shaders.push_back(NarutoShader);
	foggy.push_back(NarutoShader);
	////Texture Wood
	std::shared_ptr<TextureShader> woodShader = std::make_shared<TextureShader>();
	woodShader->SetTexture("../../assets/Textures/diffuse.jpg");
	woodShader->SetNormalTexture("../../assets/Textures/normal.jpg");
	woodShader->SetLightPosition(sun.Position);
	woodShader->SetCamera(camera);
	shaders.push_back(woodShader);
	foggy.push_back(woodShader);
	//RTT
	std::shared_ptr<RTT> textureRefractShader = std::make_shared<RTT>(waterFBO->getRefractionTexture());
	shaders.push_back(textureRefractShader);

	//Post-Processing
	ppFilter = std::make_shared<PostProcessingShader>();
	ppFilter->SetFboTexture(ppFBO->GetFilterTexture());
	
	/*
	////Texture Sand
	std::shared_ptr<TextureShader> sandShader = std::make_shared<TextureShader>();
	sandShader->SetTexture("../../assets/Textures/rock.jpg");
	sandShader->SetNormalTexture("../../assets/Textures/rock_normal.jpg");
	sandShader->SetLightPosition(sun.Position);
	sandShader->SetCamera(camera);
	shaders.push_back(sandShader);
	foggy.push_back(sandShader);
	*/

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

void drawScene()
{	
	glEnable(GL_CLIP_DISTANCE0);

	//Render Reflection
	waterFBO->bindReflectionFrameBuffer(); //Binds the Reflection Buffer
	camera->FlipView(); //Set camera for reflection (flips) 
	skybox->SetViewMatrix(camera->GetViewMatrix()); //Update Skybox ViewMatrix (without position)
	scene->Draw(Vec4(0.0f, 1.0f, 0.0f, -water->GetPosition().y)); // Render the Scene above the surface
	camera->FlipView(); // Unflip camera
	skybox->SetViewMatrix(camera->GetViewMatrix()); //Reset skybox's ViewMatrix
	waterFBO->unbindFrameBuffer(); //Unbinds the Reflection Buffer
	//

	//Render Refraction
	glDisable(GL_CULL_FACE);
	waterFBO->bindRefractionFrameBuffer(); //Binds the Refraction Buffer
	scene->Draw(Vec4(0.0f, -1.0f, 0.0f, water->GetPosition().y)); //draws everything bellow the plane
	glEnable(GL_CULL_FACE);
	waterFBO->unbindFrameBuffer(); //Unbinds the Refraction Buffer
	//

	ppFBO->bindFilterFrameBuffer();

	//Render Scene Normally
	glDisable(GL_CLIP_DISTANCE0);
	scene->Draw(Vec4(0.0f, -1.0f, 0.0f, 1000)); //after GL_CLIP disabled this should be redundant. Might depend on the graphic
	waterRenderer->Draw(Vec4(0.0f, -1.0f, 0.0f, 1000));
	//Draw scene to texture

	ppFilter->Use();
	ppMesh->Draw();

	checkOpenGLError("ERROR: Could not draw scene.");
}

void processCamera()
{
	camera->RotateCamera(input->GetMouseDelta());
	camera->MoveCamera(input->GetMovement());



}

void processPostProcessingShader() 
{
	float intensityChange = input->GetIntensityChange();
	int RGBIndex = input->GetRGBIndex();
	if (intensityChange != 0.0f)
	{
		if (intensityChange > 0.f)
		{
			if ((RGBIntensity[RGBIndex] + intensityChange) >= 1.f)
			{
				RGBIntensity[RGBIndex] = 1.f;
			}
			else RGBIntensity[RGBIndex] += intensityChange;
		}
		else
		{
			if ((RGBIntensity[RGBIndex] + intensityChange) <= 0.f)
			{
				RGBIntensity[RGBIndex] = 0.f;
			}
			else RGBIntensity[RGBIndex] += intensityChange;
		}
	}
	float distortionChange = input->GetDistortionChange();
	if (distortionChange != 0.0f)
	{
		if (distortionChange > 0.f)
		{
			if ((distortionIntensity + distortionChange) >= 0.15f)
			{
				distortionIntensity = 0.15f;
			}
			else distortionIntensity += distortionChange;
		}
		else
		{
			if ((distortionIntensity + distortionChange) <= 0.f)
			{
				distortionIntensity = 0.f;
			}
			else distortionIntensity += distortionChange;
		}
	}
	std::cout << distortionIntensity << std::endl;
	glUseProgram(ppFilter->GetProgramId());
	glUniform3f(glGetUniformLocation(ppFilter->GetProgramId(), "rgbIntensity"), RGBIntensity[0], RGBIntensity[1], RGBIntensity[2]);
	glUniform1i(glGetUniformLocation(ppFilter->GetProgramId(), "mode"), input->GetPostProcessingMode());
	glUniform1f(glGetUniformLocation(ppFilter->GetProgramId(), "distortionAmount"), distortionIntensity);
	glUseProgram(0);
}

void processInput()
{
	processCamera();
	processPostProcessingShader();
	water->SetFog(input->GetFog());
	for (std::shared_ptr<TextureShader> shader : foggy)
	{
		shader->SetFog(input->GetFog());
	}
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
	waterFBO->cleanUp();
	ppFBO->cleanUp();
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
	glDepthMask(GL_FALSE);
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
	//MeshLoader loads all necessary meshes
	meshLoader->CreateMesh(std::string("../../assets/models/sphere.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/skybox.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/water_surface.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/boat.obj"));
	meshLoader->CreateMesh(std::string("../../assets/models/terrain2.obj"));

	ppMesh = meshLoader->CreatePPFilterMesh(ppFilter->GetVCoordId());

	//Skybox must be the first to be drawn in the scene
	std::shared_ptr<SceneNode> sky = scene->root->CreateNode(meshLoader->Meshes[1], Transform(), shaders[0]);

	//// DO NOT DELETE THESE LINES /////
	//reflection check
	//scene->root->CreateNode(meshLoader->Meshes[2], Transform(Vec3(-12.0, water->GetPosition().y, 0.0), Quat(), Vec3(0.5, 0.5, 0.5)), shaders[1]);
	//refraction check
	//scene->root->CreateNode(meshLoader->Meshes[2], Transform(Vec3(0.0, water->GetPosition().y, 0.0), Quat(), Vec3(0.5, 0.5, 0.5)), shaders[4]);
	////////////////////////////////////


	//Water
	//scene->root->CreateNode(meshLoader->Meshes[2], Transform(water->GetPosition(), Quat(), Vec3(3.5f, 3.5f, 3.5f)), water);
	waterRenderer = std::make_shared<WaterRenderer>(meshLoader->Meshes[2], Transform(water->GetPosition(), Quat(), Vec3(3.5f, 3.5f, 3.5f)), scene->root, water);
	//Boat
	scene->root->CreateNode(meshLoader->Meshes[3], Transform(Vec3(0.0f, water->GetPosition().y - 0.2f, 0.0f), Quat(), Vec3(0.5f, 0.5f, 0.5f)), shaders[4]);
	//Terrain
	scene->root->CreateNode(meshLoader->Meshes[4], Transform(Vec3(1.45f, -1.25f, 15.0f), Quat(), Vec3(6.0f, 6.0f, 6.0f)), shaders[3]);




}

void setupFBO()
{
	waterFBO->initializeWater(WinX, WinY);

	ppFBO->initializePostProcessing(WinX, WinY);
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