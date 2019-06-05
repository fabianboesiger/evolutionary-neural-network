#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#include <memory>
#include <queue>

#include "data.hpp"

struct Neuron {
	std::queue <float> values;
	unsigned int hold;
};

class Network {

public:
	Network(Data*);
	void train();
	std::vector <float> run(Data*, size_t);
	float getError();
	float getErrorDelta();

private:
	std::vector <std::vector <Neuron>> neurons;
	std::vector <std::vector <std::vector <float>>> axons;
	float vectorDifference(std::vector <float>, std::vector <float>);
	Data* data;
	void trainAxon(Network*, unsigned int, unsigned int, unsigned int);
	float calculateError();
	float error;
	float errorDelta;

};

#endif