#include <iostream>
#include <ctime>

#include "world.hpp"

int main(int argc, char* argv[]) {
	// set random seed
	srand(static_cast <unsigned> (time(0)));

	// create and run a world
	World* world = new World();
	world->start();
	delete world;
}