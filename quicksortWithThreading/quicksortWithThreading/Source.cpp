#include<iostream>
#include<random>
#include<thread>
#include<mutex>
#include<vector>


using namespace std;

int            threadcount = 0;
int            activeThreadCounter = 0;
int            recursionLevel = 0;
mutex          mtx;
thread*        threadArray = nullptr;
int insertionSortSize = 1000;

struct arrayStruct{
  arrayStruct(int s, int* a) : size{s}, _array{a}{}
  int size;
  int* _array;
};

vector<arrayStruct> _arrayStruct;

bool threadsActive = false;


void swap(int* s1, int* s2) {
  int tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}



void insertionSort(arrayStruct* sortObj) {



  int j;
  for(int i = 1; i < sortObj->size; ++i) {

    if(sortObj->_array[i] < sortObj->_array[i - 1]) {
      j = i;
      while(sortObj->_array[j] < sortObj->_array[j - 1]) {
        swap(&sortObj->_array[j - 1], &sortObj->_array[j]);
        j--;
      }
    }
  }
}



pair<arrayStruct, arrayStruct>&& partitionArray(int size, int* toSort, arrayStruct* _arrayPair=nullptr) {

  int pivot = (toSort[0] + toSort[size / 2] + toSort[size - 1]) / 3;
  int lessCount = 0;
  int moreCount = size - 1;


  while (lessCount <= moreCount) {

    if (toSort[lessCount] >= pivot && toSort[moreCount] < pivot) {
      swap(&toSort[lessCount++], &toSort[moreCount--]);
    }
    if (toSort[lessCount] < pivot) ++lessCount;
    if (toSort[moreCount] >= pivot) --moreCount;
  }

  return pair<arrayStruct, arrayStruct>({lessCount, toSort}, {size - lessCount, &toSort[lessCount]});
}


void quickSort(arrayStruct* sortObj) {
  
  if (sortObj->size < insertionSortSize) {
    insertionSort(sortObj);
    return;
  }
  else {


  }
}
/*
partitionArray(lessCount, toSort);
partitionArray(size - moreCount - 1, &toSort[lessCount]);

if (recursionLevel < threadcount) {
  _arrayStruct.emplace_back(lessCount, toSort);
  _arrayStruct.emplace_back(size - lessCount, &toSort[lessCount]);
}

if (!threadsActive) {
  for (auto a : _arrayStruct) {
    threadArray[activeThreadCounter++] = thread{a.size, a._array};
  }
  threadsActive = true;
}*/


int main(int argc, char** argv) {

  int            arraySize=1000;
  int            biggestElement=10000;

  random_device  rd;
  int*           myArray;


  if(argc == 2) {
    arraySize = atoi(argv[1]);
  }
  else if(argc == 3) {
    arraySize = atoi(argv[1]);
    biggestElement = atoi(argv[2]);
  }
  else if(argc == 4) {
    
    threadcount = atoi(argv[3]);
    arraySize = atoi(argv[1]);
    biggestElement = atoi(argv[2]);

    threadArray = new thread[threadcount];
  }  
  
  myArray = new int[arraySize];


  for(int i = 0; i < arraySize; ++i) {
    myArray[i] = rd() % biggestElement;
  }

  partitionArray(arraySize, myArray);

  /*for(int i = 0; i < threadcount; ++i) {
    threadArray[i].join();
  }*/
  

  for(int i = 0; i < arraySize; ++i) {
    std::printf("%d, ", myArray[i]);
  }
  std::printf("\n\n");
  std::printf("Array Size: %4d", arraySize);
  

  delete[] myArray;
}
  

