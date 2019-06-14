#include <iostream>
#include <fstream>
#include <ctime>

#include "world.hpp"

int main(int argc, char* argv[]) {
	std::cout << "starting application" << std::endl;

	// set random seed
	srand(static_cast <unsigned> (time(0)));

	// constructing path
	std::string path = __FILE__;
	if (path.find_last_of('\\') != std::string::npos) {
		path = path.substr(0, 1 + path.find_last_of('\\'));
	} else {
		path = path.substr(0, 1 + path.find_last_of('/'));
	}

	// load data
	Data * data = new Data(path);

	// create and run a world
	World * world = new World(data, path);
	for (double wantedError = 0.25; wantedError >= 0; wantedError -= (wantedError / 100.0)) {
		// evolve to wanted error
		world->evolve(wantedError);
		// predict a specific amount of results
		world->predict(10);
		// save result
		data->save(world->getError());
	}

	delete data;
	delete world;

	std::cout << "terminating application" << std::endl;

	return EXIT_SUCCESS;
}