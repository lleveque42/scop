#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define ERR_INIT "Error: failed to initialize GLFW."
#define ERR_CREATE_WIN "Error: could not create window (OpenGL version may be too old)."

#define WIN_WIDTH 640
#define WIN_HEIGHT 480
#define WIN_TITLE "Scop"

class Window {
	private:
		GLFWwindow *_window;
		static void _error_callback(int error, const char* description);

	public:
		Window();
		~Window();

		void initialize();
		void render();
};

