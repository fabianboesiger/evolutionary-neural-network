#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "data.hpp"



const std::string Data::INPUT_FILE = "input.csv";
const std::string Data::OUTPUT_FILE = "output.csv";
const float Data::CONTAMINATION_SCALE = 0;



Data::Data(std::string & path) {

	inputPath = path + INPUT_FILE;
	outputPath = path + OUTPUT_FILE;

	std::cout << "loading data from " << inputPath << std::endl;

	average = 0;
	variance = 0;
	count = 0;

	// loading data
	inputDataSize = 0;
	std::ifstream fileStream(inputPath);
	if (fileStream.fail()) {
		std::cout << "error occured while loading data" << std::endl;
		std::exit(1);
	}
	for (std::string line; getline(fileStream, line);) {
		std::istringstream lineStream(line);
		std::vector <float> temporary;
		for (std::string token; std::getline(lineStream, token, ',');) {
			float value = std::stof(token);
			temporary.push_back(value);

			average += value;
			count++;
		}
		data.push_back(temporary);
		inputDataSize++;
	}
	fileStream.close();

	average /= count;

	for (std::vector <float> vector : data) {
		for (float value : vector) {
			variance += std::pow(average - value, 2);
		}
	}

	variance /= (count - 1);

	contamine();
}

size_t Data::getDataSize() {
	return data.size();
}

size_t Data::getVectorSize() {
	return data.at(0).size();
}

std::vector <float> Data::getData(unsigned int i, bool contamined) {
	if (contamined) {
		return contaminedData.at(i);
	} else {
		return data.at(i);
	}
}

void Data::addData(std::vector <float> & newData) {
	data.push_back(newData);
}

void Data::save(double error) {
	std::cout << "saving data to " << outputPath << std::endl;

	// saving data
	std::ofstream fileStream(outputPath);
	if (fileStream.fail()) {
		std::cout << "error occured while saving data" << std::endl;
		std::exit(1);
	}

	fileStream << "sep=,\nerror=" << error << '\n';

	for (std::vector <float> vector : data) {
		for (unsigned int i = 0; i < vector.size(); i++) {
			if (i > 0) {
				fileStream << ",";
			}
			fileStream << vector.at(i);
		}
		fileStream << '\n';
	}
	fileStream.close();

	while (data.size() > inputDataSize) {
		data.pop_back();
	}
}

void Data::contamine() {
	contaminedData.clear();
	for (unsigned int i = 0; i < data.size(); i++) {
		std::vector <float> vector;
		for (unsigned int j = 0; j < data.at(i).size(); j++) {
			vector.push_back(data.at(i).at(j) + (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 2 - 1) * variance * CONTAMINATION_SCALE);
		}
		contaminedData.push_back(vector);
	}
}