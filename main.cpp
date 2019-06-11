#include <iostream>
#include <fstream>
#include <ctime>

#include "world.hpp"

int main(int argc, char* argv[]) {
	std::cout << "starting application" << std::endl;

	// set random seed
	srand(static_cast <unsigned> (time(0)));

	// load data
	Data * data = new Data();

	// create and run a world
	World * world = new World(data);
	world->evolve(0.15);
	world->predict(10);

	// save result
	data->save();

	delete data;
	delete world;

	std::cout << "terminating application" << std::endl;

	return EXIT_SUCCESS;
}