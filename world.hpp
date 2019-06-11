#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <thread>

#include "network.hpp"
#include "data.hpp"

class World {
	
public:
	static const unsigned int POPULATION_SIZE;
	static const unsigned int FITTEST_SURVIVORS_SIZE;
	static const unsigned int FITTEST_SURVIVORS_TRAINING;

	World(Data * data);
	void evolve(double);
	void predict(unsigned int);

private:
	std::vector <std::shared_ptr <Network>> population;
	Data * data;
	std::shared_ptr <Network> winner;

};

#endif