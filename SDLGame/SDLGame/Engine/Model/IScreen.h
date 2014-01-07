#ifndef I_SCREEN_H_
#define I_SCREEN_H_

class IScreen {
public:
	virtual void render(float deltaSec) = 0;
	virtual void resize(int width, int height) = 0;
};

#endif //I_SCREEN_H_