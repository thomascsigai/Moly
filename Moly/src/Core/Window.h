#pragma once

#include <Core.h>
#include <Log.h>

#include <sstream>
#include <glad/glad.h>
#include <glfw3.h>

namespace Moly {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool VSync;

		WindowProps(const std::string& title = "Moly Engine",
			uint32_t width = 1600,
			uint32_t height = 900,
			bool VSync = true)
			: Title(title), Width(width), Height(height), VSync(VSync)
		{
		}
	};

	class MOLY_API Window
	{
	public:
		Window(const WindowProps& props);
		~Window() = default;

		void Clear();
		void Update();
		bool Should_close();

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		void SetVSync(bool enabled);
		bool IsVSync() const;

	private:
		virtual void Init(const WindowProps& props);

		WindowProps windowData;
		GLFWwindow* windowGLFW;
	};
}