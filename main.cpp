#include <iostream>
#include <fstream>
#include <ctime>

#include "world.hpp"

int main(int argc, char* argv[]) {
	// set random seed
	srand(static_cast <unsigned> (time(0)));

	// create and run a world
	World * world = new World();
	world->evolve(0.2);
	std::vector <float> result = world->predict();

	std::string path = __FILE__;
	if (path.find_last_of('\\') != std::string::npos) {
		path = path.substr(0, 1 + path.find_last_of('\\'));
	}
	else {
		path = path.substr(0, 1 + path.find_last_of('/'));
	}
	path += "result.csv";

	std::ofstream resultStream(path);

	std::cout << "the result is [";
	for (unsigned int i = 0; i < result.size(); i++) {
		if (i > 0) {
			std::cout << ", ";
			resultStream << ",";
		}
		std::cout << result.at(i);
		resultStream << result.at(i);
	}
	std::cout << "]" << std::endl;

	resultStream.close();

	delete world;

	return EXIT_SUCCESS;
}