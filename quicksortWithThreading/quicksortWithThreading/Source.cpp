#include<iostream>
#include<random>
#include<thread>
#include<mutex>
#include<vector>
#include<memory>
#include<utility>
#include<functional>


using namespace std;

int            threadcount = 0;
int            activeThreadCounter = 0;
int            recursionLevel = 0;
mutex          mtx;
thread*        threadArray = nullptr;
int insertionSortSize = 50;

struct arrayStruct{
  arrayStruct(){};
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






pair<arrayStruct, arrayStruct> partitionArray(arrayStruct sortObj) {

  int* _array = sortObj._array;
  int size = sortObj.size;

  int pivot = (_array[0] + _array[size / 2] + _array[size - 1]) / 3;
  int lessCount = 0;
  int moreCount = size - 1;


  while (lessCount <= moreCount) {

    if (_array[lessCount] >= pivot && _array[moreCount] < pivot) {
      swap(&_array[lessCount++], &_array[moreCount--]);
    }
    if (_array[lessCount] < pivot) ++lessCount;
    if (_array[moreCount] >= pivot) --moreCount;
  }
  
  return pair<arrayStruct, arrayStruct>{{ lessCount, _array }, {size - lessCount, &_array[lessCount]}};
}







void quickSort(arrayStruct sortObj) {
  
  if (sortObj.size < insertionSortSize) {
    insertionSort(move(sortObj));
    return;
  }
  else {
    pair<arrayStruct, arrayStruct> _pair = partitionArray(sortObj);
    quickSort(_pair.first);
    quickSort(_pair.second);
  }
}






int main(int argc, char** argv) {

  int            arraySize=1000;
  int            biggestElement=10000;
  random_device  rd;
  int*           myArray;

  struct partitionContainer{
    partitionContainer() {}
    partitionContainer(arrayStruct a) : subArray{new arrayStruct{a}} {}
    arrayStruct* subArray;
    partitionContainer* left = nullptr;
    partitionContainer* right = nullptr;
  };
  
  

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
    partitionContainer = new arrayStruct[threadcount];
  }  
  
  myArray = new int[arraySize];




  for(int i = 0; i < arraySize; ++i) {
    myArray[i] = rd() % biggestElement;
  }




  // setup threading if requested
  if (threadcount != 0) {

    partitionContainer* pContainerRoot = new partitionContainer{{arraySize, myArray}};

    int counter = 0;
    int level = 0;
    
    // run recursive algorithm to fill thread tree
    function<void(partitionContainer*)> 
      fillContainer = [&counter, &fillContainer](partitionContainer* container) {

      ++counter;
      if (counter==threadcount) {
        return;
      }
      else {
        pair<arrayStruct, arrayStruct> tmpPair = partitionArray(*container->subArray);
        container->left->subArray = new arrayStruct{tmpPair.first};
        container->right->subArray = new arrayStruct{tmpPair.second};
        counter += 2;
        
        if(!container->left)  fillContainer(container->left);
        else if(!container->right) fillContainer(container->right);
      
        
        if (counter == threadcount) return;
        fillContainer(container->right);
        if (counter == threadcount) return;
      }
    };
    fillContainer(pContainerRoot);

    
    

    counter = 0;

    function<void(partitionContainer*)> initializeThreads = [&counter, &initializeThreads](partitionContainer* container) {
      if (!container) {
        return;
      }
      threadArray[counter++] = thread{quickSort, container->subArray};
      initializeThreads(container->left);
      initializeThreads(container->right);
    };

    for (int i = 0; i < threadcount; ++i) {
      threadArray[i].join();
    }
  }
  else {
    quickSort({arraySize, myArray});
  }





  for(int i = 0; i < arraySize; ++i) {
    std::printf("%d, ", myArray[i]);
  }
  std::printf("\n\n");
  std::printf("Array Size: %4d", arraySize);
  

  delete[] myArray;
}
  

