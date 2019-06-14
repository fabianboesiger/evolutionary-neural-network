#ifndef DATA_HPP
#define DATA_HPP

#include <vector>

class Data {
	
public:
	static const std::string INPUT_FILE;
	static const std::string OUTPUT_FILE;
	static const float CONTAMINATION_SCALE;

	Data(std::string &);
	size_t getDataSize();
	size_t getVectorSize();
	std::vector <float> getData(unsigned int, bool);
	void addData(std::vector <float> &);
	void save(double);
	void contamine();

private:
	std::vector <std::vector <float>> data;
	std::vector <std::vector <float>> contaminedData;
	std::string inputPath;
	std::string outputPath;
	unsigned int inputDataSize;
	float average;
	float variance;
	unsigned int count;

};

#endif