#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "network.hpp"

class World {
	
public:
	static const int POPULATION_SIZE = 8;
	World();
	~World();
	void start();

private:
	Network* population = new Network[POPULATION_SIZE];

};

#endif