#include <iostream>

#include "world.hpp"

World::World() {
	std::cout << "creating new world" << std::endl;

	data = new Data();
	for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
		population.push_back(std::shared_ptr <Network> (new Network(data)));
	}
}

World::~World() {
	delete data;
}

void World::start() {

	while (true) {
		std::cout << "training networks" << std::endl;

		std::shared_ptr <Network> best;

		for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
			std::shared_ptr <Network> network = population.at(i);
			network->train();
			if (best == nullptr || network->getErrorDelta() > best->getErrorDelta()) {
				best = network;
			}
		}

		std::cout << std::endl;
		for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
			std::cout << "network " << i << " error is " << population.at(i)->getError() << std::endl;
		}

		std::cout << "best network has error " << best->getError() << " with error delta " << best->getErrorDelta() << std::endl;
	}
}