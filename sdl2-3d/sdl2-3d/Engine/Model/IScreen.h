#pragma once

class IScreen {
public:
	virtual ~IScreen() {};
	virtual void render(float deltaSec) = 0;
	virtual void resize(int width, int height) = 0;
};