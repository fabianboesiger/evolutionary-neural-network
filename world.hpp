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
	static const std::string SAVE_FILE;

	World(Data *, std::string &);
	void evolve(double);
	void predict(unsigned int);
	double getError();

private:
	std::vector <std::shared_ptr <Network>> population;
	Data * data;
	std::shared_ptr <Network> winner;
	std::string savePath;
	void save();

};

#endif