#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "data.hpp"



const std::string Data::INPUT_PATH = "input.csv";
const std::string Data::OUTPUT_PATH = "output.csv";



Data::Data() {

	// constructing path
	path = __FILE__;
	if (path.find_last_of('\\') != std::string::npos) {
		path = path.substr(0, 1 + path.find_last_of('\\'));
	} else {
		path = path.substr(0, 1 + path.find_last_of('/'));
	}

	inputPath = path + INPUT_PATH;
	outputPath = path + OUTPUT_PATH;

	std::cout << "loading data from " << inputPath << std::endl;
	std::cout << "saving data to " << outputPath << std::endl;

	// loading data
	std::ifstream fileStream(inputPath);
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
	fileStream.close();

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

void Data::addData(std::vector <float> & newData) {
	std::cout << "add data: [";
	for (unsigned int i = 0; i < newData.size(); i++) {
		if (i > 0) {
			std::cout << ", ";
		}
		std::cout << newData.at(i);
	}
	std::cout << "]" << std::endl;
	data.push_back(newData);
}

void Data::save() {
	std::cout << "saving data" << std::endl;

	// saving data
	std::ofstream fileStream(outputPath);
	if (fileStream.fail()) {
		std::cout << "error occured while saving data" << std::endl;
		std::exit(1);
	}
	for (std::vector <float> vector : data) {
		std::cout << "[";
		for (unsigned int i = 0; i < vector.size(); i++) {
			if (i > 0) {
				std::cout << ", ";
				fileStream << ",";
			}
			std::cout << vector.at(i);
			fileStream << vector.at(i);
		}
		std::cout << "]" << std::endl;
		fileStream << '\n';
	}
	fileStream.close();
}