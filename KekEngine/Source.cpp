#include <iostream>
#include <string>
#include "Logging/Log.h"
#include "Maths/Transform.h"

// Camera Class
// Rendering

int main()
{
	Transform3D tr({ 10,20,5 }, { 4,1 ,12 }, { 10,20,30 });
	Log(tr);

	return 0;
}