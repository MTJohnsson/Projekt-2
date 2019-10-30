
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
float generateRand(int rmax);
int loadDataSet(int dataSetSize, float* v, const char* fileName);
int writeDataSet(int dataSetSize, float* v, const char* fileName, float avg, float max, float min);
void createDataSet(int dataSetSize, const char* fileName);
void swap(int* xp, int* yp);
void selectionSort(int arr[], int n);
float average(float* v, int dataSize);
float maxValue(float* v, int dataSize);
float minValue(float* v, int dataSize);

//Input: DataSetSize, BufferSize, DatasetFilename, OutputFilename
//Output : the file OutputFilename containing the sorted dataset.
int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(0))); //initialize the random number generator
	std::string filePath = "DataTest.txt";
	int size = 100;
	float* v = new float[size];
	createDataSet(size, filePath.c_str());
	auto time1 = std::chrono::high_resolution_clock::now();
	loadDataSet(size, v , filePath.c_str());
	float avg = average(v, size);
	float max = maxValue(v, size);
	float min = minValue(v, size);
	printf("avg = %f\nmax = %f\nmin = %f\n",avg,max,min);
	writeDataSet(size, v , filePath.c_str(), avg, max, min);
	auto time2 = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count();
	float seconds = ms / 1000.0f;
	printf("Time in s(%f)", seconds);
	system("PAUSE");
	return 0;
}


float generateRand(int rmax) {
	//Generate a floating point random number between 0 and rmax
	return (((float)rand() / (float)(RAND_MAX)) * rmax);
}

float minValue(float* v, int dataSize) {
	float minValue = v[0];
	for (int i = 1; i < dataSize; i++)
	{
		if (minValue > v[i])
		{
			minValue = v[i];
		}
	}
	return minValue;
}

float maxValue(float* v, int dataSize) {
	float maxValue = v[0];
	for (int i = 1; i < dataSize; i++)
	{
		if (maxValue < v[i])
		{
			maxValue = v[i];
		}
	}
	return maxValue;
}

float average(float* v, int dataSize)
{
	float average = v[0];
	for (int i = 0; i < dataSize; i++)
	{
		average += v[i];
	}
	return ( average / dataSize );
}

int loadDataSet(int dataSetSize, float* outV, const char* fileName) {
	std::ifstream file;
	
	file.open(fileName);
	int number = 0;
	while (file >> outV[number])
	{
		number++;
	}
	printf("\n");
	file.close();
	return 1;
}

int writeDataSet(int dataSetSize, float * v,const char* fileName,float avg,float max,float min)
{
	std::ofstream fileStream("test.txt");
	if (fileStream.is_open()) {
		fileStream << "avg : " << avg << std::endl << "max : " << max << std::endl << "min : " << min << std::endl;
		for (int i = 0; i < dataSetSize; i++)
		{
			fileStream << v[i] << std::endl;
		}
	}
	fileStream.close();
	return(0);
}

void createDataSet(int dataSetSize, const char* fileName) {
	std::ofstream fileStream(fileName);
	if (fileStream.is_open()) {
		for (int i = 0; i < dataSetSize; i++)
		{
			fileStream << generateRand(100) << std::endl;
		}
	}
	fileStream.close();
}

void swap(int* xp, int* yp) {
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void selectionSort(int arr[], int n) {
	int min_idx;

	for (int i = 0; i < n - 1; i++)
	{

		min_idx = i;
		for (int j = i + 1; j < n; j++)
			if (arr[j] < arr[min_idx])
				min_idx = j;

		swap(&arr[min_idx], &arr[i]);
	}
}

