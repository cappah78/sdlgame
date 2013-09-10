#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "Mesh.h"
#include "CubeMap.h"
#include "Shader.h"



class SkyBox {
public:
	SkyBox();
	~SkyBox();
	void render();
private:
	CubeMap* cubeMap;
	Mesh mesh;
};

#endif //SKYBOX_H_