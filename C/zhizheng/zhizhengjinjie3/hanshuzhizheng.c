//存放函数地址的指针
// 数组名 != &数组名
// 函数名 == &函数名
#include <stdio.h>

int add(int x, int y)
{
    return x + y;
}
void test(char *str)
{

}

int main(int argc, char const *argv[])
{
    // printf("%p\n", &add);
    // printf("%p", add);
    // int(*pf)(int, int) = &add;
    
    void (*test)(char *p) = &test;
    return 0;
}
