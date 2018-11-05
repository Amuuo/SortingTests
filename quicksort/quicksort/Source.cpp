#include<iostream>
#include<random>
#include<vector>

using namespace std;




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
  vector<int> debugArray;
  for(int i = 0; i < size - 1; ++i) {
    debugArray.push_back(toSort[i]);
  }
}



void quickSort(int size, int* toSort) {

  
  vector<int> debugArray;
  for(int i = 0; i < size - 1; ++i) {
    debugArray.push_back(toSort[i]);
  }  
  
  
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
        lessCount = toSort[lessCount] < pivot?lessCount + 1:lessCount;
        moreCount = toSort[moreCount] >= pivot ? moreCount - 1 : moreCount;        
      }      
    }

    
    // swap pivot back and recursively call qp
    swap(&toSort[lessCount], &toSort[size-1]);
    quickSort(lessCount, toSort);
    quickSort(size - moreCount-1, &toSort[lessCount]);


    cout << "\n\nPost qp: ";
    for(int i = 0; i < size; ++i) {
      cout << toSort[i] << ", " << endl;
    }
    cout << "\n\n";
  }
}







int main() {

  std::random_device rd;
  int* myArray = new int[1000];


  for(int i = 0; i < 1000; ++i) {
    myArray[i] = rd() % 10000;
  }

  for(int i = 0; i < 1000; ++i) {
    std::cout << myArray[i] << std::endl;
  }

  quickSort(1000, myArray);

  int pause;
  std::cin >> pause;
}