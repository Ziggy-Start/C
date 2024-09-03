#ifndef _COMMON_H_
#define _COMMON_H_

typedef struct Stu
{
    int id;
    char name[32];
    int score;

}data_t;

#define NUM 10

typedef struct sqlist
{
    data_t data[NUM];
    int last;

}sqlist_t;

sqlist_t *CreateSqList(void);
void DisplaySqList(sqlist_t *list);
int InsertSqList(sqlist_t *list,int local,data_t data);



#endif