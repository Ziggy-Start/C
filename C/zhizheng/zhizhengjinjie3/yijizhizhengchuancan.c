#include <stdio.h>

void print(int*ptr, int sz)
{
    int i = 0;
    for ( i = 0; i < sz; i++)
    {
        printf ( " %d ",*(ptr + i));
    }
    
}
void test(char *p)
{
    printf("6\t");
}
int main()
{
    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    int*p = arr;
    int sz = sizeof(arr)/sizeof(arr[0]);
    print (p,sz);
    
    char ch;
    char *p1 = &ch;
    test(&ch);
    test(p1);

    return 0;
}