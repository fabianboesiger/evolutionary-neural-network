#ifndef DATA_HPP
#define DATA_HPP

#include <vector>

class Data {
	
public:
	Data();
	size_t getDataSize();
	size_t getVectorSize();
	std::vector <float> getData(unsigned int);

private:
	std::vector <std::vector <float>> data;

};

#endif