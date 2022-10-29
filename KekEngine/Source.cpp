#include "KekEngine/Core/Log.h"
#include "KekEngine/Core/Application.h"
#include "KekEngine/SystemIO/SystemIO.h"
#include <thread>
#include <chrono>
using namespace std::chrono_literals;
/// KURDE JAK MIE PROGRAM ZWALNIA TO ZAS MI KLAWIATURA NIE DZIAA DOBRZE.
/// ADD MODULATORS TO SYSTEM_IO TO MATCH GLFW, AND ALSO PUT THEM BACK TO GLFW.
using namespace Kek;

void LogMouse(vec2i mpos)
{
	Log<Info>(mpos);
}
Application app;
void Kek::Setup()
{
	SystemIO::Init();
	SystemIO::MouseMoveEvent() += LogMouse;
	app.AddWindow(Window{"window", vec2i(800,600)});
}
void Kek::Update()
{
	if(app.WindowCount() == 0) app.Close();
	for(int i = 0; i < app.WindowCount(); i++)
	{
		app[i].SwapBuffers();
	}
}