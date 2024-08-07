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
	return new Sandbox();
}