#pragma once

#ifdef ML_PLATFORM_WINDOWS

extern Moly::Application* Moly::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Moly::CreateApplication();
	app->Run();
	delete app;
}

#endif