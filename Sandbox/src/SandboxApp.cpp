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
	Sandbox* sandbox = new Sandbox();
	ML_CLIENT_TRACE("Initializing Sandbox App");
	return sandbox;
}