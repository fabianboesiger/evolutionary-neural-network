#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

#include "network.hpp"



// training constants
const float Network::DELTA_MULTIPLIER = 0.5;
// the following constants have to be greater than one
const unsigned int Network::EXPECTED_LAYERS = 4;
const unsigned int Network::EXPECTED_LAYER_SIZE = 4;
const unsigned int Network::EXPECTED_HOLD = 3;
// mutation settings
const double Network::HOLD_MUTATION_CHANCE = 0.3;
const double Network::NEURON_MUTATION_CHANCE = 0.3;
const double Network::LAYER_MUTATION_CHANCE = 0.3;



unsigned int Network::idCount = 0;

Network::Network(Data * data):
	data(data),
	id(idCount++) {

	std::cout << "creating new network" << std::endl;

	generation = 1;
	trainingCount = 0;

	// creating neurons for each layer
	unsigned int layers = 2;
	while (std::rand() < RAND_MAX * (1 - 1.0 / EXPECTED_LAYERS)) {
		layers++;
	}
	for (unsigned int i = 0; i < layers; i++) {
		std::vector <Neuron> layer;
		size_t layerSize = 0;
		if (i == 0 || i == layers - 1) {
			layerSize = data->getVectorSize();
		} else {
			while (std::rand() < RAND_MAX * (1 - 1.0 / EXPECTED_LAYER_SIZE)) {
				layerSize++;
			}
		}

		for (unsigned int j = 0; j < layerSize; j++) {
			unsigned int hold = 1;
			while (std::rand() < RAND_MAX * (1 - 1.0 / EXPECTED_HOLD)) {
				hold++;
			}
			Neuron neuron;
			neuron.hold = 1;
			layer.push_back(neuron);
		}
		neurons.push_back(layer);
	}

	// creating axons to connect layers
	for (unsigned int i = 0; i < layers - 1; i++) {
		std::vector <std::vector <float>> matrix;
		for (unsigned int j = 0; j < neurons.at(i + 1).size(); j++) {
			std::vector <float> vector;
			for (unsigned int k = 0; k < neurons.at(i).size(); k++) {
				vector.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 2 - 1);
			}
			matrix.push_back(vector);
		}
		axons.push_back(matrix);
	}

	countAxons();
	calculateError();
}

Network::Network(std::shared_ptr <Network> parent):
	data(parent->data),
	id(idCount++) {

	std::cout << "creating new network from parent" << std::endl;

	generation = parent->generation + 1;
	trainingCount = parent->trainingCount;

	int newNeuronsCount = 0;
	std::vector <int> neuronList;
	int passedNewNeuronsCount = 0;
	std::vector <int> passedNeuronList;

	for (unsigned int i = 0; i < parent->neurons.size(); i++) {
		std::vector <Neuron> layer;
		std::vector <std::vector <float>> matrix;
		for (unsigned int j = 0; j < parent->neurons.at(i).size(); j++) {
			bool neuronPreserved = false;
			// create neurons
			if (i == 0 || i == parent->neurons.size() - 1) {
				// first and last layer, no neuron mutations
				Neuron neuron;
				neuron.hold = parent->neurons.at(i).at(j).hold;
				if (std::rand() < RAND_MAX * HOLD_MUTATION_CHANCE) {
					if (std::rand() < RAND_MAX * 0.5) {
						// hold mutates up
						neuron.hold += 1;
					} else
					if (neuron.hold > 1) {
						// hold mutates down
						neuron.hold -= 1;
					}
				}
				neuronList.push_back(j);
				layer.push_back(neuron);
				neuronPreserved = true;
			} else {
				// middle layers, with neuron mutations
				if (std::rand() < RAND_MAX * NEURON_MUTATION_CHANCE) {
					// neuron mutation
					if (std::rand() < RAND_MAX * 0.5) {
						// new neuron
						unsigned int hold = 1;
						while (std::rand() < RAND_MAX * (1 - 1.0 / EXPECTED_HOLD)) {
							hold++;
						}
						Neuron newNeuron;
						newNeuron.hold = 1;
						newNeuronsCount--;
						neuronList.push_back(newNeuronsCount);
						layer.push_back(newNeuron);
						
						// create random axons for new neuron
						std::vector <float> vector;
						
						for (unsigned int k = 0; k < neurons.back().size(); k++) {
							vector.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 2 - 1);
						}
						matrix.push_back(vector);
						// preserve old neuron
						Neuron neuron;
						neuron.hold = parent->neurons.at(i).at(j).hold;
						if (std::rand() < RAND_MAX * HOLD_MUTATION_CHANCE) {
							if (std::rand() < RAND_MAX * 0.5) {
								// hold mutates up
								neuron.hold += 1;
							} else
							if (neuron.hold > 1) {
								// hold mutates down
								neuron.hold -= 1;
							}
						}
						neuronList.push_back(j);
						layer.push_back(neuron);
						neuronPreserved = true;
					}
				} else {
					Neuron neuron;
					neuron.hold = parent->neurons.at(i).at(j).hold;
					if (std::rand() < RAND_MAX * HOLD_MUTATION_CHANCE) {
						if (std::rand() < RAND_MAX * 0.5) {
							// hold mutates up
							neuron.hold += 1;
						} else
						if (neuron.hold > 1) {
							// hold mutates down
							neuron.hold -= 1;
						}
					}
					neuronList.push_back(j);
					layer.push_back(neuron);
					neuronPreserved = true;
				}
			}
			// create axons
			if (i > 0) {
				if (neuronPreserved) {
					if (neuronList.back() >= 0) {
						std::vector <float> vector;
						for (unsigned int k = 0; k < neurons.back().size(); k++) {
							if (passedNeuronList.at(k) >= 0) {
								vector.push_back(parent->axons.at(i - 1).at(neuronList.back()).at(passedNeuronList.at(k)));
							} else {
								vector.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 2 - 1);
							}
						}
						matrix.push_back(vector);
					}
				}
			}
		}

		if (false && i != 0 && i != parent->neurons.size() - 1 && std::rand() < RAND_MAX * LAYER_MUTATION_CHANCE) {
			// mutate layer

			if (std::rand() < RAND_MAX * 0.5) {
				// preserve old layer
				neurons.push_back(layer);
				if (i > 0) {
					axons.push_back(matrix);
				}

				// add new layer
				std::vector <Neuron> newLayer;
				for (unsigned int j = 0; j < layer.size(); j++) {
					Neuron neuron;
					neuron.hold = 1;
					newLayer.push_back(neuron);
				}
				neurons.push_back(newLayer);

				std::vector <std::vector <float>> newMatrix;
				for (unsigned int j = 0; j < layer.size(); j++) {
					std::vector <float> vector;
					for (unsigned int k = 0; k < layer.size(); k++) {
						if (j == k) {
							vector.push_back(1);
						}
						else {
							vector.push_back(0);
						}
					}
					newMatrix.push_back(vector);
				}
				axons.push_back(newMatrix);

				// information about last layer
				passedNewNeuronsCount = newNeuronsCount;
				passedNeuronList.swap(neuronList);
				newNeuronsCount = 0;
				neuronList.clear();
			}
		} else {
			// do not mutate layer
			neurons.push_back(layer);
			if (i > 0) {
				axons.push_back(matrix);
			}

			// information about last layer
			passedNewNeuronsCount = newNeuronsCount;
			passedNeuronList.swap(neuronList);
			newNeuronsCount = 0;
			neuronList.clear();
		}
	}

	countAxons();
	calculateError();
}

Network::Network(Data * data, std::ifstream & fileStream):
	data(data),
	id(idCount++) {

	std::cout << "loading network" << std::endl;

	// loading generation
	std::string generationString;
	getline(fileStream, generationString);
	generation = stoi(generationString);

	// loading training count
	std::string trainingCountString;
	getline(fileStream, trainingCountString);
	trainingCount = stoi(trainingCountString);

	// loading neurons
	std::string neuronsIString;
	getline(fileStream, neuronsIString);
	unsigned int neuronsI = stoi(neuronsIString);
	for (unsigned int i = 0; i < neuronsI; i++) {
		std::string neuronsJString;
		getline(fileStream, neuronsJString);
		unsigned int neuronsJ = stoi(neuronsJString);
		std::vector <Neuron> layer;
		for (unsigned int j = 0; j < neuronsJ; j++) {
			std::string holdString;
			getline(fileStream, holdString);
			unsigned int hold = stoi(holdString);
			Neuron neuron;
			neuron.hold = hold;
			layer.push_back(neuron);
		}
		neurons.push_back(layer);
	}
	// loading axons
	std::string axonsIString;
	getline(fileStream, axonsIString);
	unsigned int axonsI = stoi(axonsIString);
	for (unsigned int i = 0; i < axonsI; i++) {
		std::string axonsJString;
		getline(fileStream, axonsJString);
		unsigned int axonsJ = stoi(axonsJString);
		std::vector <std::vector <float>> matrix;
		for (unsigned int j = 0; j < axonsJ; j++) {
			std::string axonsKString;
			getline(fileStream, axonsKString);
			unsigned int axonsK = stoi(axonsKString);
			std::vector <float> vector;
			for (unsigned int k = 0; k < axonsK; k++) {
				std::string valueString;
				getline(fileStream, valueString);
				float value = stof(valueString);
				vector.push_back(value);
			}
			matrix.push_back(vector);
		}
		axons.push_back(matrix);
	}

	countAxons();
	calculateError();
}

void Network::train() {
	double errorBeforeTrain = error;

	// train all axons
	std::vector <unsigned int> shuffled;
	for (unsigned int i = 0; i < axons.size(); i++) {
		shuffled.push_back(i);
	}

	std::random_shuffle(shuffled.begin(), shuffled.end());
	for (unsigned int i = 0; i < axons.size(); i++) {
		unsigned int shuffledI = shuffled.at(i);
		std::vector <std::thread> threads;
		Network clone = *this;
		for (unsigned int j = 0; j < axons.at(shuffledI).size(); j++) {
			for (unsigned int k = 0; k < axons.at(shuffledI).at(j).size(); k++) {
				// threads.push_back(std::thread(&Network::trainAxon, clone, this, shuffledI, j, k));
				trainAxon(this, shuffledI, j, k);
			}
		}
		for (unsigned int i = 0; i < threads.size(); i++) {
			threads.at(i).join();
		}
	}

	std::cout << std::endl;

	errorDelta = errorBeforeTrain - calculateError();
	trainingCount++;
}

std::vector <float> Network::run(size_t to, bool contamined) {

	// clear all neurons
	for (unsigned int i = 0; i < neurons.size(); i++) {
		for (unsigned int j = 0; j < neurons.at(i).size(); j++) {
			std::queue <float> empty;
			std::swap(neurons.at(i).at(j).values, empty);
		}
	}

	std::vector <float> output;

	for (unsigned int i = 0; i < to; i++) {
		
		std::vector <float> input = data->getData(i, contamined);

		// initialize input neurons
		for (unsigned int j = 0; j < neurons.at(0).size(); j++) {
			neurons.at(0).at(j).values.push(sigmoid(input.at(j)));
		}

		// calculate next neurons
		for (unsigned int j = 1; j < neurons.size(); j++) {

			for (unsigned int k = 0; k < neurons.at(j).size(); k++) {

				float value = 0;
				unsigned int count = 0;

				for (unsigned int l = 0; l < neurons.at(j - 1).size(); l++) {
					Neuron& origin = neurons.at(j - 1).at(l);
					if (origin.values.size() >= origin.hold) {
						float originValue = origin.values.front();
						if (!std::isnan(originValue)) {
							value += originValue * axons.at(j - 1).at(k).at(l);
							count++;
						} 
					}
				}

				if (count > 0) {
					value /= static_cast <float> (count);
				}

				Neuron& destination = neurons.at(j).at(k);
				destination.values.push(sigmoid(value));
			}

			for (unsigned int k = 0; k < neurons.at(j - 1).size(); k++) {
				Neuron& neuron = neurons.at(j - 1).at(k);
				if (neuron.values.size() >= neuron.hold) {
					neuron.values.pop();
				}
			}
		}

		// age neurons
		size_t lastLayer = neurons.size() - 1;
		for (unsigned int j = 0; j < neurons.at(lastLayer).size(); j++) {
			Neuron& neuron = neurons.at(lastLayer).at(j);
			if (neuron.values.size() >= neuron.hold) {
				if (i == to - 1) {
					output.push_back(sigmoidReverse(neuron.values.front()));
				}
				neuron.values.pop();
			} else {
				if (i == to - 1) {
					output.push_back(0);
				}
			}
		}

	}
	return output;
}

float Network::vectorDifference(std::vector <float> vector1, std::vector <float> vector2) {
	float output = 0;
	unsigned int count = 0;
	size_t size = std::min(vector1.size(), vector2.size());
	for (unsigned int i = 0; i < size; i++) {
		if (!std::isnan(vector1.at(i)) && !std::isnan(vector2.at(i))) {
			output += (std::abs(vector1.at(i) - vector2.at(i)));
			count++;
		}
	}
	if (count > 0) {
		output /= static_cast <float> (count);
	}
	return output;
}

double Network::calculateError() {
	error = 0;
	for (unsigned int l = 1; l < data->getDataSize(); l++) {
		error += static_cast <double> (vectorDifference(run(l, false), data->getData(l, false)));
	}
	error /= (data->getDataSize() - 1);
	return error;
}

double Network::getError() {
	return error;
}

double Network::getErrorDelta() {
	return errorDelta;
}

double Network::getFitness() {
	if (errorDelta == 0) {
		return 0;
	}
	return (1 - error);
}

void Network::trainAxon(Network* original, unsigned int i, unsigned int j, unsigned int k) {

	float originalValue = axons.at(i).at(j).at(k);

	float originalDifferenceSum = 0;
	for (unsigned int l = 1; l < data->getDataSize(); l++) {
		originalDifferenceSum += vectorDifference(run(l, true), data->getData(l, false));
	}
	originalDifferenceSum /= (data->getDataSize() - 1);

	float delta = originalDifferenceSum * DELTA_MULTIPLIER;

	// up it
	float uppedDifferenceSum = 0;
	axons.at(i).at(j).at(k) = originalValue + delta;
	for (unsigned int l = 1; l < data->getDataSize(); l++) {
		uppedDifferenceSum += vectorDifference(run(l, true), data->getData(l, false));
	}
	uppedDifferenceSum /= (data->getDataSize() - 1);

	// down it
	float downedDifferenceSum = 0;
	axons.at(i).at(j).at(k) = originalValue - delta;
	for (unsigned int l = 1; l < data->getDataSize(); l++) {
		downedDifferenceSum += vectorDifference(run(l, true), data->getData(l, false));
	}
	downedDifferenceSum /= (data->getDataSize() - 1);

	// correct axon value
	if (uppedDifferenceSum < originalDifferenceSum || downedDifferenceSum < originalDifferenceSum) {
		if (uppedDifferenceSum < originalDifferenceSum) {
			std::cout << "+";
			original->axons.at(i).at(j).at(k) = originalValue + delta;
		}
		else {
			std::cout << "-";
			original->axons.at(i).at(j).at(k) = originalValue - delta;
		}
	}
	else {
		original->axons.at(i).at(j).at(k) = originalValue;
	}
}

float Network::sigmoid(float input) {
	// return input / (1 + std::abs(input));
	return std::tanh(input);
}

float Network::sigmoidReverse(float input) {
	return std::atanh(input);
}

unsigned int Network::getId() {
	return id;
}

unsigned int Network::getGeneration() {
	return generation;
}

unsigned int Network::getTrainingCount() {
	return trainingCount;
}

void Network::save(std::ofstream & fileStream) {
	fileStream << generation << '\n';
	fileStream << trainingCount << '\n';

	fileStream << neurons.size() << '\n';
	for (unsigned int i = 0; i < neurons.size(); i++) {
		fileStream << neurons.at(i).size() << '\n';
		for (unsigned int j = 0; j < neurons.at(i).size(); j++) {
			fileStream << neurons.at(i).at(j).hold << '\n';
		}
	}

	fileStream << axons.size() << '\n';
	for (unsigned int i = 0; i < axons.size(); i++) {
		fileStream << axons.at(i).size() << '\n';
		for (unsigned int j = 0; j < axons.at(i).size(); j++) {
			fileStream << axons.at(i).at(j).size() << '\n';
			for (unsigned int k = 0; k < axons.at(i).at(j).size(); k++) {
				fileStream << axons.at(i).at(j).at(k) << '\n';
			}
		}
	}
}

void Network::countAxons() {
	totalAxons = 0;
	for (unsigned int i = 0; i < axons.size(); i++) {
		for (unsigned int j = 0; j < axons.at(i).size(); j++) {
			for (unsigned int k = 0; k < axons.at(i).at(j).size(); k++) {
				totalAxons++;
			}
		}
	}
}

unsigned int Network::getTotalAxons() {
	return totalAxons;
}