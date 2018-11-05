#include<iostream>
#include<random>

using namespace std;


void quickSort(int size, int* toSort) {

  if(size == 1) return;
  
  int pivot = size / 2;
  swap(&toSort[pivot], &toSort[size - 1]);
  int lessCount = 0, moreCount = size - 2;

  while(lessCount <= moreCount) {
    if(toSort[lessCount++] >= toSort[size] && 
        toSort[moreCount--] <= toSort[size]) {
      swap(&toSort[lessCount - 1], &toSort[moreCount + 1]);
    }
  }
  swap(&toSort[lessCount], &toSort[size]);
  quickSort(lessCount + 1, toSort);
  quickSort(size - moreCount, &toSort[lessCount]);


  cout << "\n\nPost qp: ";
  for(int i = 0; i < size; ++i) {    
    cout << toSort[i] << ", ";   
  }
  cout << "\n\n";
}

void swap(int* s1, int* s2) {
  int tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
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

  int pause;
  std::cin >> pause;
}