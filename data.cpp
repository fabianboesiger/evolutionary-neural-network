#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "data.hpp"

Data::Data() {

	// constructing path
	std::string path = __FILE__;
	if (path.find_last_of('\\') != std::string::npos) {
		path = path.substr(0, 1 + path.find_last_of('\\'));
	} else {
		path = path.substr(0, 1 + path.find_last_of('/'));
	}
	path += "data.csv";

	std::cout << "loading data from " << path << std::endl;

	// loading data
	std::ifstream fileStream(path);
	if (fileStream.fail()) {
		std::cout << "error occured while loading data" << std::endl;
		std::exit(1);
	}
	for (std::string line; getline(fileStream, line);) {
		std::istringstream lineStream(line);
		std::vector <float> temporary;
		for (std::string token; std::getline(lineStream, token, ',');) {
			temporary.push_back(std::stof(token));
		}
		data.push_back(temporary);
	}

}

size_t Data::getDataSize() {
	return data.size();
}

size_t Data::getVectorSize() {
	return data.at(0).size();
}

std::vector <float> Data::getData(unsigned int i) {
	return data.at(i);
}