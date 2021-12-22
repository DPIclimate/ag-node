#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <math.h>
#include <sstream>

std::map<std::string, std::vector<float>> readCsv(std::string filename){
	std::map<std::string, std::vector<float>> input;
	std::ifstream csvFile(filename, std::ios::in);

	std::vector<float> ts;
	std::vector<float> weight1;
	std::vector<float> weight2;

	if(csvFile.is_open()){
		std::string tmp;
		while(getline(csvFile, tmp)){
			std::cout << tmp << std::endl;
			std::replace(tmp.begin(), tmp.end(), ',', ' ');
			std::cout << tmp << std::endl;
			std::stringstream ss(tmp);
			std::string tmp2;
			std::vector<std::string> cols;
			while( ss >> tmp2 ){
				cols.push_back(tmp2);
			}
			std::cout << cols[0] << std::endl;
			ts.push_back(std::stof(cols[0]));
			weight1.push_back(std::stof(cols[1]));
			weight2.push_back(std::stof(cols[2]));
		}
		input["ts"] = ts;
		input["w1"] = weight1;
		input["w2"] = weight2;
	}
	return input;
}


float calculate_slope(std::map<std::string, std::vector<float>>& input){
	// Get the 25, 50 and 75% points
	int p25 = input["ts"].size() * 0.25f;
	int p50 = input["ts"].size() * 0.50f;
	int p75 = input["ts"].size() * 0.75f;

	double xSum = 0, x2Sum = 0, ySum = 0, xySum = 0;	
	for(int i = 0; i < (p50 - p25); i++){
		xSum = xSum + input["ts"][i];
		ySum = ySum + input["w1"][i];
		x2Sum = x2Sum + pow(input["ts"][i], 2);
		xySum = xySum + input["ts"][i] * input["w1"][i];
	}

	double a = ((p50 - p25) * xySum - xSum * ySum) / ((p50 - p25) * x2Sum - xSum * xSum);

	printf("%f\n", a);
	printf("%f\n", input["w1"][p25]);
	printf("%f\n", input["w1"][p50]);


	return 0.0f;
}


int main(){
	std::map<std::string, std::vector<float>> input = readCsv("data/readings.csv");
	calculate_slope(input);


	return 0;
}
