#include "KekEngine/Core/Log.h"
#include "KekEngine/Core/Application.h"
#include "KekEngine/SystemIO/SystemIO.h"
/// KURDE JAK MIE PROGRAM ZWALNIA TO ZAS MI KLAWIATURA NIE DZIAA DOBRZE.
/// ADD MODULATORS TO SYSTEM_IO TO MATCH GLFW, AND ALSO PUT THEM BACK TO GLFW.
using namespace Kek;

Application app;
void Kek::Setup()
{
	SystemIO::Init();
}
void Kek::Update()
{
	if(SystemIO::Key(Key_Enter) != State_Click) return;

	for(int i = 0; i < 5000; i++)
	{
		SystemIO::SetKey({ Mouse_Left, State_Click });
	}
	app.Close();
}