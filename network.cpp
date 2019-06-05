#include <iostream>
#include <thread>

#include "network.hpp"

Network::Network(Data* data)
	:data(data) {

	std::cout << "creating new network" << std::endl;

	// creating neurons for each layer
	unsigned int layers = 2;
	while (std::rand() < RAND_MAX * 1 / 2) {
		layers++;
	}
	for (unsigned int i = 0; i < layers; i++) {
		std::vector <Neuron> layer;
		size_t layerSize = 0;
		if (i == 0 || i == layers - 1) {
			layerSize = data->getVectorSize();
		} else {
			while (std::rand() < RAND_MAX * 9 / 10) {
				layerSize++;
			}
		}

		for (unsigned int j = 0; j < layerSize; j++) {
			unsigned int hold = 1;
			while (std::rand() < RAND_MAX * 1 / 2) {
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
				vector.push_back(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2)) - 1);
			}
			matrix.push_back(vector);
		}
		axons.push_back(matrix);
	}

	calculateError();
}

void Network::train() {
	float errorBeforeTrain = error;

	// train all axons
	std::vector <std::thread> threads;
	for (unsigned int i = 0; i < axons.size(); i++) {
		for (unsigned int j = 0; j < axons.at(i).size(); j++) {
			for (unsigned int k = 0; k < axons.at(i).at(j).size(); k++) {
				Network clone = *this;
				threads.push_back(std::thread(&Network::trainAxon, clone, this, i, j, k));
			}
		}
	}

	for (unsigned int i = 0; i < threads.size(); i++) {
		threads.at(i).join();
	}

	errorDelta = errorBeforeTrain - calculateError();
}

std::vector <float> Network::run(Data* data, size_t to) {
	
	// clear all neurons
	for (unsigned int i = 0; i < neurons.size(); i++) {
		for (unsigned int j = 0; j < neurons.at(i).size(); j++) {
			std::queue <float> empty;
			std::swap(neurons.at(i).at(j).values, empty);
		}
	}

	std::vector <float> output;
	

	for (unsigned int i = 0; i < to; i++) {
		
		// initialize input neurons
		for (unsigned int j = 0; j < neurons.at(0).size(); j++) {
			neurons.at(0).at(j).values.push(data->getData(i).at(j));
		}

		// calculate next neurons
		for (unsigned int j = 1; j < neurons.size(); j++) {

			for (unsigned int k = 0; k < neurons.at(j).size(); k++) {

				float value = 0;
				for (unsigned int l = 0; l < neurons.at(j - 1).size(); l++) {
					Neuron& origin = neurons.at(j - 1).at(l);
					if (origin.values.size() >= origin.hold) {
						value += origin.values.front() * axons.at(j - 1).at(k).at(l);
					}
				}

				Neuron& destination = neurons.at(j).at(k);
				destination.values.push(value);
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
		output.clear();
		for (unsigned int k = 0; k < neurons.at(lastLayer).size(); k++) {
			Neuron& neuron = neurons.at(lastLayer).at(k);
			if (neuron.values.size() >= neuron.hold) {
				output.push_back(neuron.values.front());
				neuron.values.pop();
			}
			else {
				output.push_back(0);
			}
		}

	}

	return output;
}

float Network::vectorDifference(std::vector <float> vector1, std::vector <float> vector2) {
	float output = 0;
	size_t size = std::min(vector1.size(), vector2.size());
	for (unsigned int i = 0; i < size; i++) {
		output += std::abs(vector1.at(i) - vector2.at(i));
	}
	return output / static_cast <float> (size);
}

float Network::calculateError() {
	error = 0;
	for (unsigned int l = 1; l < data->getDataSize(); l++) {
		error += vectorDifference(run(data, l), data->getData(l));
	}
	error /= (data->getDataSize() - 1);
	return error;
}

float Network::getError() {
	return error;
}

float Network::getErrorDelta() {
	return errorDelta;
}

void Network::trainAxon(Network* original, unsigned int i, unsigned int j, unsigned int k) {

	float originalValue = axons.at(i).at(j).at(k);

	float originalDifferenceSum = 0;
	for (unsigned int l = 1; l < data->getDataSize(); l++) {
		originalDifferenceSum += vectorDifference(run(data, l), data->getData(l));
	}
	originalDifferenceSum /= (data->getDataSize() - 1);

	float delta = originalDifferenceSum / 10;

	// up it
	float uppedDifferenceSum = 0;
	axons.at(i).at(j).at(k) = originalValue + delta;
	for (unsigned int l = 1; l < data->getDataSize(); l++) {
		uppedDifferenceSum += vectorDifference(run(data, l), data->getData(l));
	}
	uppedDifferenceSum /= (data->getDataSize() - 1);

	// down it
	float downedDifferenceSum = 0;
	axons.at(i).at(j).at(k) = originalValue - delta;
	for (unsigned int l = 1; l < data->getDataSize(); l++) {
		downedDifferenceSum += vectorDifference(run(data, l), data->getData(l));
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