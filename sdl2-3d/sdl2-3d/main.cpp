#include <gl\glew.h>	//including these here to make sure they're not included out of order later on

#include "Game.h"
#include "Screens/GameScreen.h"

#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_thread.h>
#include <IL\il.h>

#include "Engine\Graphics\GL\GLCommandQueue.h"

/* If using gl3.h */
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#define GLM_FORCE_RADIANS

#define NULL 0

const char* PROGRAM_NAME = "sdlgame";
const unsigned int INIT_WINDOW_WIDTH = 1600;
const unsigned int INIT_WINDOW_HEIGHT = 900;
const unsigned int INIT_WINDOW_XPOS = 30;
const unsigned int INIT_WINDOW_YPOS = 30;

/* A simple function that prints a message, the error code returned by SDL,
 * and quits the application */
void sdldie(const char *msg) {
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}
 
void checkSDLError(int line = -1) {
	const char *error = SDL_GetError();
	if (*error != '\0') {
		printf("SDL Error: %s\n", error);
        if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
}

// maximum mumber of lines the output console should have
static const WORD MAX_CONSOLE_LINES = 500;

void redirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
	
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}
 
/* Our program's entry point */
int main(int argc, char *argv[])
{
	redirectIOToConsole();

    SDL_Window *mainwindow;
 
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdldie("Unable to initialize SDL");
 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);	//Req opengl 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
 
	mainwindow = SDL_CreateWindow(PROGRAM_NAME, INIT_WINDOW_XPOS, INIT_WINDOW_YPOS,
		INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!mainwindow)
        sdldie("Unable to create window");
    checkSDLError(__LINE__);

	ilInit();

	int width, height;
	SDL_GetWindowSize(mainwindow, &width, &height);
	Game::graphics.initialize(width, height, mainwindow);

	SDL_SetRelativeMouseMode(SDL_TRUE); //capture mouse

	{
		GameScreen gameScreen;
		Game::setScreen(&gameScreen);
		Game::startLoop(); // blocks untill Game::stopLoop()
	}

    SDL_DestroyWindow(mainwindow);
    SDL_Quit();

	printf("Bye");
#ifdef _DEBUG
	SDL_Delay(1000);
#endif //_DEBUG
    return 0;
}