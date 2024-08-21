#include <Moly.h>

class Sandbox : public Moly::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Moly::Application* Moly::CreateApplication()
{
	ML_CLIENT_TRACE("Initializing Sandbox App");
	Sandbox* sandbox = new Sandbox();
	sandbox->SetWindowData("Sandbox", 1600, 900, true);

	return sandbox;
}