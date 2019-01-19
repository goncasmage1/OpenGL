# Project Instructions

This OpenGL project contains a scene that includes a wooden raft/boat sailing through the ocean with a flag attached.

Camera:
* W, A, S, D to move forward and sideways
* E, Q to move up and down
* Hold Right or Middle mouse button and move the mouse to look

Wind:
* X, Z to increase or decrease the wind force (hold down)

Fog:
* F to toggle fog

Screenshot:
* H to take a screenshot (they will be saved inside the Screenshots folder)

Post-Processing:
* B, N, M to select which color component to control (R, G and B respectively)
* '.', ',' to increase or decrease the currently selected color component's strength
* P to toggle between overlay and filter ("overlay" adds selected color on top of scene and "filter" filters only the selected colors)
* L, K to increase or decrease wavy shader intensity
* O, I to increase or decresae wavy shader speed
* 9, 8 to increase or decrease wavy shader frequency


# Building NvCloth

This file will help you compile NvCloth in order to be able to use it with this OpenGL project.

You will need a Windows system in order to compile the library (we used Windows 10)

## Step 1 - Installing requirements
In order to compile NvCloth, you will need to install:
* [Visual Studio 2015 with Update 3](https://visualstudio.microsoft.com/pt-br/vs/older-downloads/):
    * Make sure to tick all the boxes related to C++ and Windows 8.1 (no need for mobile development)
* [Windows 8.1 SDK](https://developer.microsoft.com/pt-pt/windows/downloads/sdk-archive)
* [CUDA SDK](https://developer.nvidia.com/cuda-downloads), which we recommend installing in the following location for simplicity: "D:\ProgramFiles\Cuda\v 10.0". The documentation and temporary install locations are irrelevant.

## Step 2 - Editing bat files
After you have installed all these, you will need to go to dependencies/NvCloth-master/NvCloth/scripts and edit the locate_cuda.bat and locate_win8sdk.bat files to point to the install locations of each of those SDK's, but if you followed our recommendations you shouldn't need to change these files (just make sure they're correct).

## Step 3 - Generating Visual Studio solution files
Go to the dependencies/NvCloth-master/NvCloth folder and run CmakeGenerateProjects.bat. This will install something called packman (which we're not sure what it is) and generate the visual studio project files to build the library.
After this bat file has finished running, go into the dependencies/NvCloth-master/NvCloth/compiler folder. There should be 5 folders inside: a cmake folder, and the combination of vc14 or vc12 with win32 or win64.
If you're using a 64-bit operating system, you can delete the folders related to 32-bit.
After that you will have a vc14win64 folder and a vc12win64 folder. One of them should have Visual Studio related files, and the other will not.
Because we've had different results in this part on the installation procedure, we will have to create branching instructions depending on your results.

## Step 4 - Building NvCloth

### 4.1 - Visual Studio files are in vc14win64

This is the desired situation, although the least likely one. In this case, all you have to do is:
* Open the NvCloth.sln file using Visual Studio 2015, not 2017.
* Open the Solution Explorer, right click on ALL_BUILD and select Build. You will have to do this twice, for the following solution configurations: debug and release.

### 4.2 - Visual Studio files are in vc12win64

If this is your situation, these are the steps you will have to take:
* Open the NvCloth.sln file using Visual Studio 2015, not 2017.
* A message should pop up saying your compiler needs an update, just click OK.
* Open the Solution Explorer, go to each of the 4 projects' Properties, and in the General section, change the Platform Toolset to Visual Studio 2015 (v140). Before doing this, change the Configuration (in the top left corner of the Properties window) to "All Configurations"
* Open the Solution Explorer, right click on ALL_BUILD and select Build. You will have to do this twice, for the following solution configurations: debug and release.


## Step 5 - Copying binaries

Once the building procedure has been completed, go to dependencies/NvCloth-master/NvCloth/bin/vc<>win<>-cmake and copy these files into the bin/x64/ folder of the root OpenGL project folder:
* NvCloth_x64.dll
* NvClothDEBUG_x64.dll
* PxFoundation_x64.dll
* PxFoundationDEBUG_x64.dll

## Step 6 - Visual Studio files were in vc12win64

This step is only necessary if your Visual Studio solution files were inside a vc12 folder.

Go to the CGJProject/CGJProject folder, open CGJProject.vcxproj from your favorite text editor (not Visual Studio) and replace all "vc14"s with "vc12"s.

## Step 7

Run the thing!

Inside the CGJProject folder, you will find the CGJProject.sln file. Open it with Visual Studio (2017 this time) and you *should* be able to run the project just fine.