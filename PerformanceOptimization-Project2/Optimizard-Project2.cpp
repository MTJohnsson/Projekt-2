
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
const int EXPONENT = 5;
const int DATASETSIZE = 10;
const int BLOCKSIZE = 4096;

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
int partition(float* arr, float size, int low, int high);
void literateDataSet(int dataSetSize, float* v, const char* fileName, float avg, float max, float min);

//Input: DataSetSize, BufferSize, DatasetFilename, OutputFilename
//Output : the file OutputFilename containing the sorted dataset.
int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(0))); //initialize the random number generator
	std::string filePath = "DataTest.txt";
	int size = pow(DATASETSIZE, EXPONENT);
	int nrOfBlocks = (size * sizeof(float)) / BLOCKSIZE;
	if ((size * sizeof(float)) % BLOCKSIZE != 0)
		nrOfBlocks++;
	float* v = new float[BLOCKSIZE * nrOfBlocks];
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
	quickSort(v, size, 0, size-1);
	auto timeSort = std::chrono::high_resolution_clock::now();
	writeDataSet(size, v, "test.txt", avg, max, min);
	auto timeWrite = std::chrono::high_resolution_clock::now();

	auto msLoad = std::chrono::duration_cast<std::chrono::microseconds>(timeLoad - time1).count();
	auto msAvg = std::chrono::duration_cast<std::chrono::microseconds>(timeAvg - timeLoad).count();
	auto msMax = std::chrono::duration_cast<std::chrono::microseconds>(timeMax - timeAvg).count();
	auto msMin = std::chrono::duration_cast<std::chrono::microseconds>(timeMin - timeMax).count();
	auto msSort = std::chrono::duration_cast<std::chrono::microseconds>(timeSort - timeMin).count();
	auto msWrite = std::chrono::duration_cast<std::chrono::microseconds>(timeWrite - timeSort).count();
	auto msTotal = std::chrono::duration_cast<std::chrono::microseconds>(timeWrite - time1).count();

	printTimes("Unoptimized-Third.txt", size, msLoad, msAvg, msMax, msMin, msSort, msWrite, msTotal);
	literateDataSet(size, v, "readable-test.txt", avg, max, min);
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
	/*std::ifstream file;

	file.open(fileName);
	int number = 0;
	while (file >> outV[number])
	{
		number++;
	}
	printf("\n");
	file.close();*/

	/*std::fstream backing;
	backing.open(fileName, std::ios::in | std::ios::binary);
	backing.read(reinterpret_cast<char*>(&outV[0]), sizeof(float) * dataSetSize);
	backing.close();*/
	int blockSize = 4096;
	int nrOfBlocks = (dataSetSize * sizeof(float)) / blockSize;
	if ((dataSetSize * sizeof(float)) % blockSize != 0) {
		nrOfBlocks++;
	}
	std::fstream loading;
	loading.open(fileName, std::ios::in | std::ios::binary);
	for (int i = 0; i < nrOfBlocks; i++) {
		loading.seekg(blockSize * i, loading.beg);
		loading.read((char*)(&outV[i * (blockSize / sizeof(float))]), blockSize);
	}
	/*for (int i = 0; i < dataSetSize; i++) {
		printf(" outV[%i] : %f", i, outV[i]);
	}*/
	loading.close();

	return 1;
}

int writeDataSet(int dataSetSize, float* v, const char* fileName, float avg, float max, float min)
{
	/*std::ofstream fileStream(fileName);
	if (fileStream.is_open()) {
		fileStream << "avg : " << avg << std::endl << "max : " << max << std::endl << "min : " << min << std::endl;
		for (int i = 0; i < dataSetSize; i++)
		{
			fileStream << v[i] << std::endl;
		}
	}
	fileStream.close();*/
	int blockSize = 4096;
	int nrOfBlocks = (dataSetSize * sizeof(float)) / blockSize;
	if ((dataSetSize * sizeof(float)) % blockSize != 0) {
		nrOfBlocks++;
	}
	std::fstream writing;
	writing.open(fileName, std::ios_base::out | std::ios::binary);
	writing.seekg((sizeof(float) * 0), writing.beg);
	writing.write((char*)(&avg), sizeof(float));
	writing.seekg((sizeof(float) * 1), writing.beg);
	writing.write((char*)(&max), sizeof(float));
	writing.seekg((sizeof(float) * 2), writing.beg);
	writing.write((char*)(&min), sizeof(float));

	for (int i = 0; i < nrOfBlocks; i++) {
		writing.seekg((sizeof(float)*3) + blockSize * i, writing.beg);
		writing.write((char*)(&v[i * (blockSize / sizeof(float))]), blockSize);
	}
	writing.close();

	return(0);
}

void createDataSet(int dataSetSize, const char* fileName) {
	/*std::ofstream fileStream(fileName);
	if (fileStream.is_open()) {
		for (int i = 0; i < dataSetSize; i++)
		{
			fileStream << generateRand(100) << std::endl;
		}
	}
	fileStream.close();*/
	int blockSize = 4096;
	int nrOfBlocks = (dataSetSize * sizeof(float)) / blockSize;
	if ((dataSetSize * sizeof(float)) % blockSize != 0) {
		nrOfBlocks++;
	}
	std::fstream creating;
	float *v = new float[dataSetSize];
	for (int i = 0; i < dataSetSize; i++)
	{
		v[i] = generateRand(100);
		//printf("outV[%i] : %f", i, v[i]);
	}
	creating.open(fileName, std::ios_base::out | std::ios_base::binary);
	for (int i = 0; i < nrOfBlocks; i++) {
		creating.seekg(blockSize * i, creating.beg);
		creating.write((char*)(&v[i * (blockSize/sizeof(float))]), blockSize);

	}
	creating.close();

	//std::fstream creating;
	//float *v = new float[dataSetSize];
	//for (int i = 0; i < dataSetSize; i++)
	//{
	//	v[i] = generateRand(100);
	//	//printf("outV[%i] : %f", i, v[i]);
	//}
	//creating.open(fileName, std::ios_base::out | std::ios_base::binary);
	//creating.write(reinterpret_cast<char*>(&v[0]), sizeof(float)*dataSetSize);
	//creating.close();
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

int partition(float* arr, float size,int low, int high)
{
	float pivot = arr[high];
	int i = (low - 1);  

	for (int j = low; j <= high - 1; j++)
	{
		if (arr[j] < pivot)
		{
			i++; 
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void quickSort(float *arr, float size, int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, size, low, high);

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

void literateDataSet(int dataSetSize, float* v, const char* fileName, float avg, float max, float min) {

	std::ofstream fileStream(fileName);
	if (fileStream.is_open()) {
		fileStream << "avg : " << avg << std::endl << "max : " << max << std::endl << "min : " << min << std::endl;
		for (int i = 0; i < dataSetSize; i++)
		{
			fileStream << v[i] << std::endl;
		}
	}
	fileStream.close();
}
