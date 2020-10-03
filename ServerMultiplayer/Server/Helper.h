#pragma once
#define WINDOWS_ENABLED _WIN32
#define VERBOSE true
#include <iostream>

using namespace std;

class Log {
public:
	static void print_verbose(const char* _string) { if (VERBOSE) cout << _string << endl; }
};