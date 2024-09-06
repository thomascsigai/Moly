#pragma one

#include <Core.h>

#include <Log.h>
#include <glfw3.h>

namespace Moly
{
	class MOLY_API InputProcessing
	{
	public:
		static bool W_PRESSED;
		static bool A_PRESSED;
		static bool S_PRESSED;
		static bool D_PRESSED;
		static bool Q_PRESSED;
		static bool E_PRESSED;
		
		static bool UP_PRESSED;
		static bool LEFT_PRESSED;
		static bool RIGHT_PRESSED;
		static bool DOWN_PRESSED;

		static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? W_PRESSED = true : W_PRESSED = false;
			glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? A_PRESSED = true : A_PRESSED = false;
			glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? S_PRESSED = true : S_PRESSED = false;
			glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? D_PRESSED = true : D_PRESSED = false;
			glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ? Q_PRESSED = true : Q_PRESSED = false;
			glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS ? E_PRESSED = true : E_PRESSED = false;

			glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ? UP_PRESSED = true : UP_PRESSED = false;
			glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ? LEFT_PRESSED = true : LEFT_PRESSED = false;
			glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ? RIGHT_PRESSED = true : RIGHT_PRESSED = false;
			glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ? DOWN_PRESSED = true : DOWN_PRESSED = false;
		}
	};
}