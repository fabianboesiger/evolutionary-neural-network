#ifndef DATA_HPP
#define DATA_HPP

#include <vector>

class Data {
	
public:
	static const std::string INPUT_PATH;
	static const std::string OUTPUT_PATH;

	Data();
	size_t getDataSize();
	size_t getVectorSize();
	std::vector <float> getData(unsigned int);
	void addData(std::vector <float> &);
	void save();

private:
	std::vector <std::vector <float>> data;
	std::string path;
	std::string inputPath;
	std::string outputPath;

};

#endif