#include "KekEngine/Core/Application.h"
#include "KekEngine/Core/Log.h"
#include "KekEngine/Core/Window.h"
#include "GLFW/glfw3.h"

using namespace Kek;

Application app;
void Kek::Setup()
{
	app.AddWindow(Window("KekEngine", WindowStyle::None, vec2f(0.5, 0.5), vec2f(0.25, 0.25)));
}
void Kek::Update()
{
	if(app.GetWindow(0).GetKey(GLFW_KEY_ESCAPE))app.Quit();

	glfwSwapBuffers(app.GetWindow(0));
}