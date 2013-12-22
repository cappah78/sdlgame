#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SDL.h>

/** 
Manages rendering related global variables and objects.
*/
class Graphics
{
public:
	static void swap()
	{
		SDL_GL_SwapWindow(m_window);
	};

	static unsigned int getScreenWidth()
	{
		return m_screenWidth;
	};

	static unsigned int getScreenHeight()
	{
		return m_screenHeight;
	};

	static void resizeScreen(unsigned int screenWidth, unsigned int screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
	};

	static SDL_Window* const getWindow()
	{
		return m_window;
	};

	static void setWindowTitle(const char* title)
	{
		SDL_SetWindowTitle(m_window, title);
	};

	static void setWindow(SDL_Window* const window)
	{
		m_window = window;
	}

private:
	static SDL_Window* m_window;
	static unsigned int m_screenWidth;
	static unsigned int m_screenHeight;

	Graphics();
	Graphics(const Graphics& copy) {};
	~Graphics() {};
};

#endif //GRAPHICS_H_
