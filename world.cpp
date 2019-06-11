#include <iostream>

#include "world.hpp"



const unsigned int World::POPULATION_SIZE = 8;
const unsigned int World::FITTEST_SURVIVORS_SIZE = 3;
const unsigned int World::FITTEST_SURVIVORS_TRAINING = 4;



World::World(Data * data):
	data(data) {
	std::cout << "creating new world" << std::endl;

	for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
		population.push_back(std::shared_ptr <Network> (new Network(data)));
	}

	winner = nullptr;
}

void World::evolve(double wantedError) {
	std::cout << "start evolving" << std::endl;

	while (true) {
		std::cout << "training all networks" << std::endl;
		for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
			std::shared_ptr <Network> & network = population.at(i);
			if (network->getError() <= wantedError) {
				winner = network;
				break;
			}
			network->train();
		}
		if (winner != nullptr) {
			break;
		}

		std::sort(population.begin(), population.end(), [](const std::shared_ptr <Network> & a, const std::shared_ptr <Network> & b) -> bool {
			return a->getFitness() > b->getFitness();
		});

		for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
			std::shared_ptr <Network> & network = population.at(i);
			std::cout << "network " << network->getId() << " (generation " << network->getGeneration() << ", trained " << network->getTrainingCount() << " times) has fitness " << network->getFitness() << " with the error " << network->getError() << std::endl;
		}

		std::cout << "training fittest networks" << std::endl;
		for (unsigned int i = 0; i < FITTEST_SURVIVORS_SIZE; i++) {
			for (unsigned j = 0; j < FITTEST_SURVIVORS_TRAINING; j++) {
				std::shared_ptr <Network> & network = population.at(i);
				if (network->getError() <= wantedError) {
					winner = network;
					break;
				}
				network->train();
			}
			if (winner != nullptr) {
				break;
			}
		}
		if (winner != nullptr) {
			break;
		}
		
		std::cout << "letting fittest networks reproduce" << std::endl;
		for (unsigned int i = 0; i < FITTEST_SURVIVORS_SIZE; i++) {
			std::shared_ptr <Network> & parent = population.at(i);
			std::shared_ptr <Network> & child = population.at(i + FITTEST_SURVIVORS_SIZE);
			child.reset(new Network(parent));
		}

		std::cout << "filling population" << std::endl;
		for (unsigned int i = FITTEST_SURVIVORS_SIZE * 2; i < POPULATION_SIZE; i++) {
			std::shared_ptr <Network> & network = population.at(i);
			network.reset(new Network(data));
		}
	}

}

void World::predict(unsigned int amount) {
	std::cout << "start predicting" << std::endl;

	for (unsigned int i = 0; i < amount; i++) {
		std::vector <float> result = winner->run(data->getDataSize());
		data->addData(result);
	}
}