//
// Created by jinyue on 2022/3/21.
//

#ifndef HWRT_UTILS_H
#define HWRT_UTILS_H

#endif //HWRT_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
//#include <pthread.h>



typedef struct DEMAND {
    int shape[2];
    int **body;
    char **idxes;
    char heads[36][4];
} demand;

typedef struct KWARGS{
    demand *d;
    int node;
    int constraint;
}kwargs;



void Reader(char *, demand *);

void Get_heads(FILE *fp, demand *d, char split);

void Get_Body(FILE *fp, demand *d, char *split);

void **Add_memory(void **ptr, int old_size, int add_size, int col, int flag);
void *get_single_qos_site(void *args);
void **get_qos_site(int node_num,int constraint,demand *d);
int * get_site_focus_num(int **qos,int row_num,int col_num,int constraint);

int Read_qos_constraint(char *filePath){
    FILE *fp= fopen(filePath,"r");
    if(!fp)printf("打开文件失败");
    else{
        int buffer=512;
        char str[buffer];
        char *p=NULL;
        int res;

        fgets(str,buffer,fp);
        fgets(str,buffer,fp);

        p=strtok(str,"=");
        p= strtok(0,"=");

        res= atoi(p);

        return res;
    }

    return  -1;
}

// 得到每一个节点的符合约束的站点
void *get_single_qos_site(void *args){
    kwargs *arg=(kwargs *)args;

    int max_size=136;
    int *p,*cursor;
    p=(int *) malloc(max_size*sizeof(int));

    cursor=p;
    int node=arg->node;
    int constraint=arg->constraint;
    int rows=arg->d->shape[0];
    int count=0;
    for(int i=0;i<rows;i++){
        int value=*(*(arg->d->body+i)+node);
        if(value<constraint){
            *cursor=i;
            cursor++;
            count++;
        }
    }
    *cursor=-1;

    return  p;

}

int * get_site_focus_num(int **qos,int row_num,int col_num,int constraint){
    int *arr;
    arr=(int *) malloc(row_num*sizeof(int));
    printf("%d\n",col_num);
    for(int i=0;i<row_num;i++){
        arr[i]=0;
        for(int j=0;j<col_num;j++){

            if(qos[i][j]<constraint)arr[i]+=1;
        }
    }
    return arr;
}


// 多线程分配节点----暂时不使用多线程

void **get_qos_site(int node_num,int constraint,demand *d){
    kwargs args[node_num];
    int **p,**cursor;
    p=(int **) malloc(node_num*sizeof(int *));
    cursor=p;
//    printf("共%d个站点,%d个节点,行内存已分配完毕\n",site_num,node_num);
    for(int i=0;i<node_num;i++){
        args[i].node=i;
        args[i].constraint=constraint;
        args[i].d=d;
        *cursor = (int *)get_single_qos_site(&args[i]);
        cursor++;
    }
    return  p;

}

// 获取表格头部
void Get_heads(FILE *fp, demand *d, char split) {
    /*
        获取表头以及列数
    */
    char c;
    char head[16];
    int v = 0, count = 0;
    while (!feof(fp)) {
        c = fgetc(fp);
        // 只需第一行
        if (c == '\n') {
            head[count] = '\0';
//            printf("%8s\t",head);
            strcpy(d->heads[v - 1], head);
            v++;
            break;
        } else if (c == split) {
            if (v > 0) {
                head[count] = '\0';
//                printf("%8s\t",head);
                strcpy(d->heads[v - 1], head); //复制表头
            }
            v++;
            count = 0;
            continue;
        }
        head[count] = c;
        count++;
    }
    d->shape[1] = v - 1;
    printf("\n");
}


// void Init_tensor(void *tensor,int init_size){
//     tensor->body=()
// }


// 获取表格主体
void Get_Body(FILE *fp, demand *d, char *split) {
    //相关参数
    int init_size = 10;
    int add_size = 10;
    int col_size = 36;
    int name_size = 32;

    // 创建动态内存
    //// 表格主体初始化
    d->body = (int **) malloc(sizeof(int *) * init_size);
    for (int i = 0; i < init_size; i++) {
        d->body[i] = (int *) malloc(sizeof(int) * col_size);
    }

    ////表格index初始化

    d->idxes = (char **) malloc(sizeof(char *) * init_size);
    for (int i = 0; i < init_size; i++) {
        d->idxes[i] = (char *) malloc(sizeof(char) * name_size);
    }


    int buffer_size = 1024;
    char str[buffer_size];

    int cols = d->shape[1];
    char  *p = NULL;
    int count = 0;
    while (fgets(str, buffer_size, fp)) {
        p = strtok(str, split);
        for (int i = 0; i < cols + 1; i++) {
            //    buffer[i]=p;
            if (i == 0) {
                strcpy(d->idxes[count], p);
//                printf("%8s\t",p);
                p = strtok(0, split);
            } else {
                // d->body[count][i-1]=atoi(p);
                *(*(d->body + count) + i - 1) = atoi(p);
//                printf("%8s\t",p);
                p = strtok(0, split);
            }

        }
        count++;

        if (count >= init_size) {
            d->body = (int **) Add_memory((void **) d->body, init_size, add_size, col_size,0);
            d->idxes = (char **) Add_memory((void **) d->idxes, init_size, add_size, name_size,1);
            init_size = init_size + add_size;
        }
    }
    d->shape[0] = count;
    printf("\n");
}


// 读取csv文件
void Reader(char *filename, demand *d) {

    // int h=0,v=0;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        printf("打开文件错误\n");

    else {
        Get_heads(fp, d,',');
        Get_Body(fp, d,",");
    }

    fclose(fp);
}



// 内存分配
void **Add_memory(void **ptr, int old_size, int add_size, int col, int flag) {

    int ptr_size=sizeof (int *);
    int data_size=sizeof(int);
    if(flag==1){
        ptr_size=sizeof (char *);
        data_size= sizeof(char);
    }
    int new_size = old_size + add_size;

    void **new_ptr = (void **) realloc(ptr, new_size * ptr_size);

    for (int i = old_size; i < new_size; i++) {
        if(flag==0)new_ptr[i] = (int *) malloc(col * data_size);
        else new_ptr[i] = (char *) malloc(col * data_size);
    }
    return new_ptr;
}
