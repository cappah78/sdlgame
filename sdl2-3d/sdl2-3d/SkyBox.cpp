#include "SkyBox.h"

SkyBox::SkyBox()
	: mesh("box.obj")
{
	std::string* shiz = new std::string[6];
	shiz[0] = "skybox/right.bmp";
	shiz[1] = "skybox/left.bmp";
	shiz[2] = "skybox/top.bmp";
	shiz[3] = "skybox/bottom.bmp";
	shiz[5] = "skybox/back.bmp";
	shiz[4] = "skybox/front.bmp";

	cubeMap = CubeMap::createFromTextures(shiz);
	delete [] shiz;
}

SkyBox::~SkyBox()
{
	delete cubeMap;
}

void SkyBox::render()
{
	cubeMap->bind(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	mesh.render();
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
}