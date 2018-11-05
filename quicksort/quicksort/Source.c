#include<stdio.h>
#include<stdlib.h>
#include<time.h>



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

 
  if(size < 100) {
    insertionSort(size, toSort);
    return;
  }
  
  else {

    
    // swap pivot and assign
    swap(&toSort[size/2], &toSort[size - 1]);
    int lessCount = 0, moreCount = size - 2;
    int pivot = toSort[size-1];


    // swap array elements around piviot
    for(; lessCount <= moreCount;) {
      
      if(toSort[lessCount] > pivot && toSort[moreCount] <= pivot) {
        swap(&toSort[lessCount++], &toSort[moreCount--]);        
      }      
      else {        
        lessCount = toSort[lessCount] <= pivot?lessCount + 1:lessCount;
        moreCount = toSort[moreCount] > pivot ? moreCount - 1 : moreCount;        
      }      
    }
    
    // swap pivot back and recursively call qp
    swap(&toSort[lessCount], &toSort[size-1]);
    quickSort(lessCount, toSort);
    quickSort(size - moreCount-1, &toSort[lessCount]);    
  }
}







int main(int argc, char** argv) {

  int arraySize;
  int biggestElement;

  if(argc == 2) {
    arraySize = atoi(argv[1]);
  }
  else if(argc == 3) {
    arraySize = atoi(argv[1]);
    biggestElement = atoi(argv[2]);
  }
  else {
    arraySize = 1000;
    biggestElement = 10000;
  }
  
  srand(time(NULL));
    
  int* myArray;

  myArray = (int*)malloc(arraySize*sizeof(int));


  for(int i = 0; i < arraySize; ++i) {
    myArray[i] = rand() % biggestElement;
  }
  

  quickSort(arraySize, myArray);

  
  for(int i = 0; i < arraySize; ++i) {
    printf("%d, ", myArray[i]);
  }
  printf("\n\n");
  printf("Array Size: %d", arraySize);

  free(myArray);
}