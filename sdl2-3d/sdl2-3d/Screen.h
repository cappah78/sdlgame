#ifndef SCREEN_H_
#define SCREEN_H_

class Screen {
public:
	virtual void render(float deltaSec) = 0;
	virtual void resize(int width, int height) = 0;
protected:
	int screenWidth;
	int screenHeight;
};

#endif //SCREEN_H_