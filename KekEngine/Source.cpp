#include <iostream>
#include <string>
#include "Logging/Log.h"
#include "Maths/Transform.h"

int main()
{
	Transform2D tr({ 10,20 }, { 4,1 });
	tr.Up();
	Log('\n');
	tr.Right();
	return 0;
}