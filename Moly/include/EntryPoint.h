#pragma once

#ifdef ML_PLATFORM_WINDOWS

extern Moly::Application* Moly::CreateApplication();

int main(int argc, char** argv)
{
	Moly::Log::Init();
	ML_CORE_INFO("Log initialized");

	auto app = Moly::CreateApplication();
	app->Run();
	delete app;
}

#endif