
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
const int EXPONENT = 5;
const int DATASETSIZE = 10;

float generateRand(int rmax);
int loadDataSet(int dataSetSize, float* v, const char* fileName);
int writeDataSet(int dataSetSize, float* v, const char* fileName, float avg, float max, float min);
void createDataSet(int dataSetSize, const char* fileName);
void swap(float* xp, float* yp);
void selectionSort(float* v, int dataSetSize);
float average(float* v, int dataSize);
float maxValue(float* v, int dataSize);
float minValue(float* v, int dataSize);
void printTimes(const char* timeFile, int dataSetSize, long long load, long long avg, long long max, long long min, long long sort, long long write, long long total);
void quickSort(float* arr, float size, int low, int high);
int partition(float arr[], float size, int low, int high);
//Input: DataSetSize, BufferSize, DatasetFilename, OutputFilename
//Output : the file OutputFilename containing the sorted dataset.
int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(0))); //initialize the random number generator
	std::string filePath = "DataTest.txt";
	int size = pow(DATASETSIZE, EXPONENT);
	float* v = new float[size];
	createDataSet(size, filePath.c_str());

	auto time1 = std::chrono::high_resolution_clock::now();
	loadDataSet(size, v, filePath.c_str());
	auto timeLoad = std::chrono::high_resolution_clock::now();
	float avg = average(v, size);
	auto timeAvg = std::chrono::high_resolution_clock::now();
	float max = maxValue(v, size);
	auto timeMax = std::chrono::high_resolution_clock::now();
	float min = minValue(v, size);
	auto timeMin = std::chrono::high_resolution_clock::now();
	//selectionSort(v, size);
	quickSort(v, size, 0, size-1);
	auto timeSort = std::chrono::high_resolution_clock::now();
	writeDataSet(size, v, filePath.c_str(), avg, max, min);
	auto timeWrite = std::chrono::high_resolution_clock::now();

	auto msLoad = std::chrono::duration_cast<std::chrono::microseconds>(timeLoad - time1).count();
	auto msAvg = std::chrono::duration_cast<std::chrono::microseconds>(timeAvg - timeLoad).count();
	auto msMax = std::chrono::duration_cast<std::chrono::microseconds>(timeMax - timeAvg).count();
	auto msMin = std::chrono::duration_cast<std::chrono::microseconds>(timeMin - timeMax).count();
	auto msSort = std::chrono::duration_cast<std::chrono::microseconds>(timeSort - timeMin).count();
	auto msWrite = std::chrono::duration_cast<std::chrono::microseconds>(timeWrite - timeSort).count();
	auto msTotal = std::chrono::duration_cast<std::chrono::microseconds>(timeWrite - time1).count();

	printTimes("Unoptimized.txt", size, msLoad, msAvg, msMax, msMin, msSort, msWrite, msTotal);
	printf("avg = %f\nmax = %f\nmin = %f\n", avg, max, min);
	printf("Time in ms(%lld)", msTotal);
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
	return (average / dataSize);
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

int writeDataSet(int dataSetSize, float* v, const char* fileName, float avg, float max, float min)
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

void swap(float* xp, float* yp) {
	float temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void selectionSort(float* v, int dataSetSize) {
	int min_idx;

	for (int i = 0; i < dataSetSize - 1; i++)
	{

		min_idx = i;
		for (int j = i + 1; j < dataSetSize; j++)
			if (v[j] < v[min_idx])
				min_idx = j;

		//printf("swapping %f & %f\n", v[min_idx], v[i]);
		swap(&v[min_idx], &v[i]);
		//printf("swapped %f & %f\n", v[min_idx], v[i]);
	}
}

int partition(float arr[], float size,int low, int high)
{
	int pivot = arr[high]; // pivot  
	int i = (low - 1); // Index of smaller element  

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than the pivot  
		if (arr[j] < pivot)
		{
			i++; // increment index of smaller element  
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(float *arr, float size, int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, size, low, high);

		// Separately sort elements before  
		// partition and after partition  
		quickSort(arr, size, low, pi - 1);
		quickSort(arr, size, pi + 1, high);
	}
}


void printTimes(const char* timeFile, int dataSetSize, long long load, long long avg, long long max, long long min, long long sort, long long write, long long total) {
	std::ofstream saveTime;
	saveTime.open(timeFile, std::ios_base::app);
	if (saveTime.is_open()) {
		saveTime << "Dataset Size : " << dataSetSize << "." << std::endl;
		saveTime << "Loading dataset time : " << load << " (ms)" << std::endl;
		saveTime << "Avg value calculation time : " << avg << " (ms)" << std::endl;
		saveTime << "Max value calculation time : " << max << " (ms)" << std::endl;
		saveTime << "Min value calculation time : " << min << " (ms)" << std::endl;
		saveTime << "Sorting time : " << sort << " (ms)" << std::endl;
		saveTime << "Write sorted dataset time : " << write << " (ms)" << std::endl;
		saveTime << "Total operation time : " << total << " (ms)" << std::endl;
		saveTime << "--------------------------" << std::endl;
	}
	saveTime.close();
}
