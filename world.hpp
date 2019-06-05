#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <thread>

#include "network.hpp"
#include "data.hpp"

class World {
	
public:
	static const int POPULATION_SIZE = 4;
	World();
	~World();
	void start();

private:
	std::vector <std::shared_ptr <Network>> population;
	Data* data;

};

#endif