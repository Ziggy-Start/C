#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"common.h"

sqlist_t *CreateSqList(void)
{
    sqlist_t *list;
    list=malloc(sizeof(sqlist_t));
    if (list==NULL)
    {
        perror("NO create");
        return NULL;
    }
    
    memset(list,0,sizeof(sqlist_t));
    list->last=-1;

    return list;
}
void DisplaySqList(sqlist_t *list)
{

    int i;

    if (list==NULL)
    {
        return;
    }
    
    for (i= 0; i <= list->last; i++)
    {
        printf("%d",list->data[i]);
    }
    printf("\n");
}
int InsertSqList(sqlist_t *list,int local,data_t data)
{
    if (list==NULL)
    {
        return;
    }

    if(list->last==NUM-1)
    {
        return -1;
    }

    if (local<0||local>list->last+1)
    {
        return -1;
    }
    

    
}