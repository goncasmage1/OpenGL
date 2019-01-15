#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <assert.h>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "NvCloth/Factory.h"
#include "NvCloth/Solver.h"
#include "NvClothExt/ClothFabricCooker.h"
#include "cuda.h"

#include "PxCallbacks/CustomAllocator.h"
#include "PxCallbacks/CustomAssertHandler.h"
#include "PxCallbacks/CustomError.h"
#include "PxCallbacks/CustomProfiler.h"

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

#define CAPTION "Hello Modern 2D World"


int WinX = 1600, WinY = 900;
int WindowHandle = 0;
unsigned int FrameCount = 0;
float animationProgress = 0.f;
physx::PxVec3 windVelocity = physx::PxVec3(6.0f, 0.f, 0.f);

float fpsInterval = 1000.f / 60.f;
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

CustomAllocator* customAllocator = new CustomAllocator();
CustomAssertHandler* customAssert = new CustomAssertHandler();
CustomError* customError = new CustomError();
CustomProfiler* customProfiler = new CustomProfiler();

nv::cloth::Factory* factory = nullptr;
nv::cloth::Solver* solver = nullptr;
CUcontext cudaContext;

std::shared_ptr<WaterFrameBuffer> waterFBO = std::make_shared<WaterFrameBuffer>();

std::shared_ptr<WaterShader> water = nullptr;
std::shared_ptr<SceneNode> Light = nullptr;
Vec3 LightPosition = Vec3(1.2f, 6.0f, 2.0f);

//Post-Processing
std::shared_ptr<PostProcessingFrameBuffer> ppFBO = std::make_shared<PostProcessingFrameBuffer>();
std::shared_ptr<PostProcessingShader> ppFilter = nullptr;
std::shared_ptr<PPFilterMesh> ppMesh = nullptr;

float RGBIntensity[3] = { 0.0f, 0.2f, 0.5f };

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

/////////////////////////////////////////////////////////////////////// NvCloth

void setupNvCloth()
{
	nv::cloth::InitializeNvCloth(customAllocator, customError, customAssert, nullptr);
	
	cuInit(0);
	int deviceCount = 0;
	CUresult result = cuDeviceGetCount(&deviceCount);
	assert(CUDA_SUCCESS == result);
	assert(deviceCount >= 1);
	result = cuCtxCreate(&cudaContext, 0, 0); //Pick first device
	assert(CUDA_SUCCESS == result);

	factory = NvClothCreateFactoryCUDA(cudaContext);
	if (factory == nullptr)
	{
		std::cerr << "ERROR:" << std::endl;
		std::cerr << "  source:     " << "NvCloth" << std::endl;
		std::cerr << "  severity:   " << "Error" << std::endl;
		std::cerr << "  debug call: " << "Could not create NvCloth Factory" << std::endl;
	}

	solver = factory->createSolver();
}

void destroyNvCloth()
{
	NvClothDestroyFactory(factory);
	cuCtxDestroy(cudaContext);
	NV_CLOTH_DELETE(solver);
	NvClothDestroyFactory(factory);
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
	std::shared_ptr<RTT> textureShader = std::make_shared<RTT>();
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
	water->SetLightPosition(LightPosition);
	water->SetLightColour(Vec3(1.0f, 1.0f, 1.0f)); //white
												   // ------------------------------------------------------- 
	shaders.push_back(water);

	////Texture 
	std::shared_ptr<TextureShader> NarutoShader = std::make_shared<TextureShader>();
	NarutoShader->SetTexture("../../assets/Textures/brickwall.jpg");
	NarutoShader->SetNormalTexture("../../assets/Textures/brickwall_normal.jpg");
	NarutoShader->SetLightPosition(Vec3(0.0f, -10.0f, 0.0f));
	NarutoShader->SetCamera(camera);
	shaders.push_back(NarutoShader);

	//RTT
	std::shared_ptr<RTT> textureRefractShader = std::make_shared<RTT>();
	textureRefractShader->SetTexture(waterFBO->getRefractionTexture());
	shaders.push_back(textureRefractShader);

	//Post-Processing
	ppFilter = std::make_shared<PostProcessingShader>();
	ppFilter->SetFboTexture(ppFBO->GetFilterTexture());

	checkOpenGLError("ERROR: Could not create shaders.");

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

	//Texture 
	std::shared_ptr<TextureShader> tugaShader = std::make_shared<TextureShader>();
	//tugaShader->SetTexture("../../assets/Textures/portugal.jpg");
	//NarutoShader->SetNormalTexture("../../assets/Textures/brickwall_normal.jpg");
	//tugaShader->SetLightPosition(Vec3(0.0f, -10.0f, 0.0f));
	//NarutoShader->SetCamera(camera);
	//shaders.push_back(tugaShader);
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

	water->SetLightPosition(LightPosition + camera->GetCameraMovement());
	glEnable(GL_CLIP_DISTANCE0);

	//Render Reflection
	waterFBO->bindReflectionFrameBuffer(); //Binds the Reflection Buffer
	std::vector<Vec3> pre = camera->FlipView(); //Set camera for reflection (flips) and Saves the previous camera settings
	Vec3 water_heightRefl = Vec3(0.0f, 0.0f, 0.0f) + camera->GetCameraMovement();
	processScene();
	scene->Draw(Vec4(0.0f, 1.0f, 0.0f, -water_heightRefl.y)); // Render the Scene above the surface
	camera->UnflipView(pre); // Set previous Camera settings
	processScene();
	waterFBO->unbindFrameBuffer(); //Unbinds the Reflection Buffer
								   //

	//Render Refraction
	waterFBO->bindRefractionFrameBuffer(); //Binds the Refraction Buffer
	glDisable(GL_CULL_FACE);
	Vec3 water_height = Vec3(0.0f, 0.0f, 0.0f) + camera->GetCameraMovement(); // Sets the hight of the plane
	scene->Draw(Vec4(0.0f, -1.0f, 0.0f, water_height.y)); //draws everything bellow the plane
	glEnable(GL_CULL_FACE);
	waterFBO->unbindFrameBuffer(); //Unbinds the Refraction Buffer
								   //

	ppFBO->bindFilterFrameBuffer();

	//Render Scene Normally
	glDisable(GL_CLIP_DISTANCE0);
	scene->Draw(Vec4(0.0f, -1.0f, 0.0f, 1000)); //after GL_CLIP disabled this should be redundant. Might depend on the graphic

	//Draw scene to texture
	ppFilter->Use();
	ppMesh->Draw();

	checkOpenGLError("ERROR: Could not draw scene.");
}

void processCamera()
{
	camera->RotateCamera(input->GetMouseDelta());
	camera->MoveCamera(input->GetMovement());

	Vec3 movementOffset = camera->GetCameraMovement();

	if (movementOffset != scene->root->transform.Position)
	{
		scene->root->transform.Position = movementOffset;
		scene->root->UpdateTransformationMatrix();
	}
}

void processPostProcessingShader() 
{
	float intensityChange = (input->GetIntensityChange());
	std::cout << intensityChange << std::endl;
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
	glUseProgram(ppFilter->GetProgramId());
	glUniform3f(glGetUniformLocation(ppFilter->GetProgramId(), "rgbIntensity"), RGBIntensity[0], RGBIntensity[1], RGBIntensity[2]);
	glUniform1i(glGetUniformLocation(ppFilter->GetProgramId(), "mode"), input->GetPostProcessingMode());
	glUseProgram(0);
}

void processInput()
{
	processCamera();
	processPostProcessingShader();
}

void processCloth()
{
	//TODO: Fix
	float deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - begin).count() * 1000.f;
	solver->beginSimulation(1.f/60.f);
	for (int i = 0; i < solver->getSimulationChunkCount(); i++)
	{
		solver->simulateChunk(i);
	}
	solver->endSimulation();

	physx::PxVec3 newWind = windVelocity + (windVelocity.getNormalized() * input->GetWindDelta());
	if (newWind.magnitude() > 0.1f) windVelocity += (windVelocity.getNormalized() * input->GetWindDelta());
	meshLoader->UpdateSailData(windVelocity);
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
	waterFBO->cleanUp();
	ppFBO->cleanUp();
	destroyNvCloth();
}

void display()
{
	auto frameStart = std::chrono::steady_clock::now();

	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	processInput();

	processCloth();
	drawScene();

	glutSwapBuffers();

	begin = std::chrono::steady_clock::now();

	/*float frameTime = std::chrono::duration<float>(begin - frameStart).count() * 1000.f;
	std::cout << "Frame Time: " << frameTime << std::endl;
	float waitTime = fpsInterval - frameTime;
	
	if (waitTime > 0.f) Sleep(waitTime);*/
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

	meshLoader->CreateMesh(std::string("../../assets/models/sphere.obj"));

	ppMesh = meshLoader->CreatePPFilterMesh(ppFilter->GetVCoordId());
	
	SailProperties properties = SailProperties();
	properties.liftCoefficient = 0.7f;
	properties.dragCoefficient = 0.7f;
	properties.gravity = physx::PxVec3(0.f, 1.f, 0.f);
	properties.damping = 0.2f;
	meshLoader->CreateSailMesh(properties, factory, solver, 0.05f, 30, 20);

	//Skybox must be the first to be drawn in the scene
	scene->root->CreateNode(meshLoader->Meshes[1], Transform(), shaders[1]);

	//reflection check
	//scene->root->CreateNode(meshLoader->Meshes[2], Transform(Vec3(-12.0, 0.0, 0.0), Quat(), Vec3(0.5, 0.5, 0.5)), shaders[0]);
	//refraction check
	//scene->root->CreateNode(meshLoader->Meshes[2], Transform(Vec3(0.0, 0.0, 0.0), Quat(), Vec3(0.5, 0.5, 0.5)), shaders[4]);

	//Naruto
	scene->root->CreateNode(meshLoader->Meshes[0], Transform(Vec3(-2.0, 0.5, -2.0), Quat(), Vec3(2.0f, 2.0f, 2.0f)), shaders[3]);

	//Water
	scene->root->CreateNode(meshLoader->Meshes[2], Transform(Vec3(0.0, 0.0, 0.0), Quat(), Vec3(2.0f, 2.0f, 2.0f)), shaders[2]);
	
	Light = scene->root->CreateNode(meshLoader->Meshes[0], Transform(LightPosition, Quat(), Vec3(1.0f, 1.0f, 1.0f)), shaders[4]);

	Transform sailTransform = Transform(Vec3(0.f, 4.f, 0.f), Quat(), Vec3(1.f));
	sailTransform.Rotation = FromAngleAxis(Vec4(1.f, 0.f, 0.f, 1.f), 90.f);
	scene->root->CreateSailNode(meshLoader->Meshes[5], sailTransform, shaders[5]);
}

void setupFBO()
{
	//FIXME: Save window Settings and have a lower resolution for the water
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
	setupNvCloth();
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