#include<stdio.h>

void test(int**ppa)
{
    **ppa=40;
}
int main(int argc, char const *argv[])
{
    int a = 10;

    int* pa = &a;
    int** ppa = &pa;
    int* arr[10] = {&a};

    // test(ppa);
    // test(&pa);
    test(arr);


    printf("%d\n",*arr[0]);

    return 0;

}
