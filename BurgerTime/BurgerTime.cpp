#pragma once

//#include <stdio.h>
//#include <iostream> // std::cout
//#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>
#include <string>

//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"

int main(int, char* []) {
	dae::Minigin engine;
	engine.Run();
	return 0;
}