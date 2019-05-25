#include <iostream>

#include "world.hpp"

World::World() {
	std::cout << "Creating new Environment" << std::endl;

	for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
		population[i] = Network();
	}
}

World::~World() {
	delete[] population;
}

void World::start() {
}