#include<iostream>
#include<random>
#include<thread>
#include<mutex>

using namespace std;

int            threadcount = 0;
int            activeThreadCounter = 0;
int            recursionLevel = 0;
bool           currentlyThreading = false;
mutex          mtx;
thread*        threadArray = nullptr;


void swap(int* s1, int* s2) {
  int tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}



void insertionSort(int size, int* toSort) {



  int j;
  for(int i = 1; i < size; ++i) {

    if(toSort[i] < toSort[i - 1]) {
      j = i;
      while(toSort[j] < toSort[j - 1]) {
        swap(&toSort[j - 1], &toSort[j]);
        j--;
      }
    }
  }
}



void quickSort(int size, int* toSort) {

  if(activeThreadCounter != threadcount && recursionLevel == (threadcount/2)) return;
  /*
  if(activeThreadCounter != threadcount && recursionLevel - activeThreadCounter == 2) {
    return;
  }*/
  else if(size < 100) {
    insertionSort(size, toSort);
    return;
  }

  else {

    ++recursionLevel;

    // swap pivot and assign
    swap(&toSort[size / 2], &toSort[size - 1]);
    int lessCount = 0, moreCount = size - 2;
    int pivot = toSort[size - 1];


    // swap array elements around piviot
    for(; lessCount <= moreCount;) {

      if(toSort[lessCount] > pivot && toSort[moreCount] <= pivot) {
        swap(&toSort[lessCount++], &toSort[moreCount--]);
      }
      else {
        lessCount = toSort[lessCount] <= pivot?lessCount + 1:lessCount;
        moreCount = toSort[moreCount] > pivot?moreCount - 1:moreCount;
      }
    }
    
    

    if(activeThreadCounter < threadcount) {
      swap(&toSort[lessCount], &toSort[size - 1]);      
      quickSort(lessCount, toSort);
      quickSort(size - moreCount - 1, &toSort[lessCount]);
      threadArray[activeThreadCounter++] = thread{quickSort, lessCount, toSort};
      threadArray[activeThreadCounter++] = thread{quickSort, size - moreCount - 1, &toSort[lessCount]};
      
    }
    else if(activeThreadCounter == threadcount) {
      quickSort(lessCount, toSort);
      quickSort(size - moreCount - 1, &toSort[lessCount]);
    }
  }
}







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

  quickSort(arraySize, myArray);
  for(int i = 0; i < threadcount; ++i) {
    threadArray[i].join();
  }
  
  if(argc == 5) {
    for(int i = 0; i < arraySize; ++i) {
      std::printf("%d, ", myArray[i]);
    }
    std::printf("\n\n");
    std::printf("Array Size: %d", arraySize);
  }

  delete[] myArray;
}
  

