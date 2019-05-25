#include <iostream>

#include "environment.hpp"

int main(int argc, char* argv[]) {
	Environment* environment = new Environment();
	environment->start();
	delete environment;
}