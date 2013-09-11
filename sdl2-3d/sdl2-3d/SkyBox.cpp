#include "SkyBox.h"

#include <gl\glew.h>
#include "CubeMap.h"
#include "Shader.h"

SkyBox::SkyBox()
	: mesh("box.obj")
{
	std::string* textureNames = new std::string[6];
	textureNames[0] = "skybox/right.bmp";
	textureNames[1] = "skybox/left.bmp";
	textureNames[2] = "skybox/top.bmp";
	textureNames[3] = "skybox/bottom.bmp";
	textureNames[5] = "skybox/back.bmp";
	textureNames[4] = "skybox/front.bmp";

	cubeMap = CubeMap::createFromTextures(textureNames);
	delete [] textureNames;
}

SkyBox::~SkyBox()
{
	delete cubeMap;
}

void SkyBox::render()
{
	cubeMap->bind(GL_TEXTURE0);

	glDisable(GL_DEPTH_TEST);
	//TODO: make messh with 
	glCullFace(GL_FRONT);
	mesh.render();
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
}