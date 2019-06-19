#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "world.hpp"



const unsigned int World::POPULATION_SIZE = 8;
const unsigned int World::FITTEST_SURVIVORS_SIZE = 3;
const unsigned int World::FITTEST_SURVIVORS_TRAINING = 4;
const std::string World::SAVE_FILE = "save.csv";



World::World(Data * data, std::string & path):
	data(data) {
	std::cout << "creating new world" << std::endl;

	savePath = path + SAVE_FILE;

	std::ifstream fileStream(savePath);
	if (fileStream.fail()) {
		std::cout << "assumimg save file doesn't exist, creating new world" << std::endl;
		for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
			population.push_back(std::shared_ptr <Network> (new Network(data)));
		}
	} else {
		std::cout << "save file exists, loading world" << std::endl;
		for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
			population.push_back(std::shared_ptr <Network> (new Network(data, fileStream)));
		}
	}
	fileStream.close();

	winner = nullptr;
}

void World::evolve(double wantedError) {
	std::cout << "start evolving to error " << wantedError << std::endl;

	winner = nullptr;

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
		data->contamine();

		std::sort(population.begin(), population.end(), [](const std::shared_ptr <Network> & a, const std::shared_ptr <Network> & b) -> bool {
			return a->getFitness() > b->getFitness();
		});

		for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
			std::shared_ptr <Network> & network = population.at(i);
			std::cout << "network " << network->getId() << " (generation " << network->getGeneration() << ", trained " << network->getTrainingCount() << " times, " << network->getTotalAxons() << " axons) has fitness " << network->getFitness() << " with the error " << network->getError() << std::endl;
		}

		std::cout << "training fittest networks" << std::endl;
		for (unsigned int i = 0; i < FITTEST_SURVIVORS_TRAINING; i++) {
			for (unsigned j = 0; j < FITTEST_SURVIVORS_SIZE; j++) {
				std::shared_ptr <Network> & network = population.at(j);
				if (network->getError() <= wantedError) {
					winner = network;
					break;
				}
				network->train();
			}
			if (winner != nullptr) {
				break;
			}
			data->contamine();
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

		// saving network
		save();
	}

}

void World::predict(unsigned int amount) {
	std::cout << "start predicting the next " << amount << " vectors" << std::endl;

	for (unsigned int i = 0; i < amount; i++) {
		std::vector <float> result = winner->run(data->getDataSize(), false);
		data->addData(result);
	}
}

double World::getError() {
	return winner->getError();
}

void World::save() {
	std::cout << "saving save file to " << savePath << std::endl;

	std::ofstream fileStream(savePath);
	if (fileStream.fail()) {
		std::cout << "error occured while saving save file" << std::endl;
		std::exit(1);
	}

	for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
		std::shared_ptr <Network> & network = population.at(i);
		network->save(fileStream);
	}

	fileStream.close();
}