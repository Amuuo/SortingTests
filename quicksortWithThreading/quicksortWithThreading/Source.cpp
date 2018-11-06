#include<iostream>
#include<random>
#include<thread>
#include<mutex>
#include<vector>
#include<memory>
#include<utility>
#include<functional>
#include<cmath>

using namespace std;



struct arrayStruct{
  
  arrayStruct(){};
  arrayStruct(int s, int* a) : size{s}, _array{a}{}
  int size;
  int* _array;
};


struct partitionContainer {
  
  partitionContainer() {}
  partitionContainer(arrayStruct a) : subArray{new arrayStruct{a}} {}  
  arrayStruct* subArray;
  partitionContainer* left;
  partitionContainer* right;
};



int threadcount = 0;
int activeThreadCounter = 0;
int recursionLevel = 0;
mutex mtx;
thread* threadArray = nullptr;
int insertionSortSize = 500;
vector<arrayStruct> _arrayStruct;
bool threadsActive = false;
arrayStruct ** subArrayContainer = nullptr;









void swap(int* s1, int* s2) {
  int tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}






void insertionSort(arrayStruct sortObj) {



  int j;
  for(int i = 1; i < sortObj.size; ++i) {

    if(sortObj._array[i] < sortObj._array[i - 1]) {
      j = i;
      while(sortObj._array[j] < sortObj._array[j - 1]) {
        swap(&sortObj._array[j - 1], &sortObj._array[j]);
        j--;
      }
    }
  }
}






void sortAroundPivot() {

}

void partitionArray(partitionContainer* sortObj) {

  int* _array = sortObj->subArray->_array;
  int size = sortObj->subArray->size;

  int pivot = (_array[0] + _array[size / 2] + _array[size - 1]) / 3;
  int lessCount = 0;
  int moreCount = size - 1;


  while (lessCount <= moreCount) {

    if (_array[lessCount] >= pivot && _array[moreCount] < pivot) {
      swap(&_array[lessCount++], &_array[moreCount--]);
    }
    else {
      if (_array[lessCount] < pivot) ++lessCount;
      if (_array[moreCount] >= pivot) --moreCount;
    }
  }
  
  sortObj->left = new partitionContainer{{lessCount, sortObj->subArray->_array}};
  sortObj->right = new partitionContainer{{size - lessCount, &sortObj->subArray->_array[lessCount]}};

}







void quickSort(arrayStruct sortObj) {
  
  if (sortObj.size < insertionSortSize) {
    insertionSort(sortObj);
    return;
  }
  else {
    partitionContainer tmp{sortObj};
    partitionArray(&tmp);
    quickSort({tmp.left->subArray->size, tmp.left->subArray->_array});
    quickSort({tmp.right->subArray->size, tmp.right->subArray->_array});
  }
}






int main(int argc, char** argv) {

  int            arraySize=1000;
  int            biggestElement=10000;
  random_device  rd;
  int*           myArray;
  int goalHeight;
  partitionContainer* rootContainer = nullptr;
  
  

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
    rootContainer = new partitionContainer[threadcount];
  }  
  
  myArray = new int[arraySize];




  for(int i = 0; i < arraySize; ++i) {
    myArray[i] = rd() % biggestElement;
  }


  goalHeight = (int)log2(threadcount);

  // setup threading if requested
  if (threadcount != 0) {

    partitionContainer* rootContainer = new partitionContainer{{arraySize, myArray}};

    int arrayCounter = 0;

    // run recursive algorithm to fill thread tree
    function<void(partitionContainer*, int level)>
      fillContainer = [&goalHeight, &arrayCounter, &fillContainer]
      (partitionContainer* container, int level) {

      if (level == goalHeight) {
        threadArray[arrayCounter++] = thread{quickSort, *container->subArray};
        return;
      }
      else {
        partitionArray(container);

        fillContainer(container->left, level + 1);
        fillContainer(container->right, level + 1);
      }
    };
    fillContainer(rootContainer, 0);

    for (int i = 0; i < goalHeight; ++i) {
      threadArray[i].join();
    }
  }
  



  for(int i = 0; i < arraySize; ++i) {
    std::printf("%d, ", myArray[i]);
  }
  std::printf("\n\n");
  std::printf("Array Size: %4d", arraySize);
  

  delete[] myArray;
}
  

