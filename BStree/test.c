#include<stdio.h>
int main()
{
  int* arr=NULL;
  int arr2[2][1]={1,2};
  arr=arr2; 
  printf("%d",*arr);
  return 0;
}
