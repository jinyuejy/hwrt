//
// Created by jinyue on 2022/3/22.
//

#ifndef HWRT_SERVICE_H
#define HWRT_SERVICE_H

#endif //HWRT_SERVICE_H
typedef struct Node{
    int site;
    struct Node *next;
}node;

typedef struct RES_TABLE{
    int **res_body;
    int **res_node;
    int **res_time;
//    int *res_idxes;

} Res;

// 函数声明模块
int * sorted(int *arr,int length);
void trans(int*p,int *q);
void deal_works(int work,int *sites,int **constraint,Res *res,int *res_idx,int t,int node);
int *get_length_sort(int **node_sites,int node_num);
node *link_list(int *value,int length);
node *init_node(int value);
node *create_node(int value);
int ** pointer2array(int row_nums);
void allocate_job(Res *res,int* res_idx,int site,int t,int node,int value);
void remove_node(node *head,int value);
void read_node(node *head);
// 函数声明模块


// 函数实现模块
void deal_works(int work,int *sites,int **constraint,Res *res,int *res_idx,int t,int node){
    // 节点分配单元
    int *cursor=sites;
    while(1){
        if(*cursor==-1)break;
        if(work>=constraint[*cursor]){
            work-=constraint[*cursor][0];
            constraint[*cursor][0]=0;
            allocate_job(res,res_idx,*cursor,t,node,constraint[*cursor][0]);
            // 当当前站点使用完毕时，使用删除此站点
//            sites++;
//            remove_node(sites,*cursor);
            cursor++;
            if(work==0)break;
        }
        else{
            constraint[*cursor]-=work;
            allocate_job(res,res_idx,*cursor,t,node,work);
            break;
        }
        printf("%d\t",*cursor);

    }
}

Res * run(int **demand,int **node_sites,int **constraint,int row_num,int column_num,int site_nums){

    int *idxes= get_length_sort(node_sites,column_num);
    Res *res;
    res=(Res *) malloc(sizeof(Res));
    int res_idx[site_nums];
    // 初始化下标
    for(int i=0;i<site_nums;i++)res_idx[i]=0;
    // 初始化结果结构体
    res->res_body = pointer2array(site_nums);
    res->res_node= pointer2array(site_nums);
    res->res_time= pointer2array(site_nums);
    printf("各项内存分配完毕===\n");
    printf("col_num=%d\n",column_num);
    // 任务仿真
    for(int i=0;i<row_num;i++){
        for(int j=0;j<column_num;j++){
            deal_works(demand[i][j],*(node_sites+idxes[j]),constraint,res,res_idx,i,idxes[j]);
        }

    }
    printf("任务分配完毕\n");
    return res;
}

// 结果操作函数
void allocate_job(Res *res,int* res_idx,int site,int t,int node,int value){
    printf("向%2d 站点分配了 %3d 时间 %2d 节点 的%6d MB流量\n",site,t,node,value);
    int col=res_idx[site];
    res->res_body[site][col]=value;
    res->res_node[site][col]=node;
    res->res_time[site][col]=t;
    res_idx[site]++;
}

// 数组转链表函数
node *link_list(int *value,int length){
    node *head,*n,*cursor;
    int *p=value;
    head= init_node(value[0]);
    cursor=head;
    if(length==-1){
        while (1){
            p++;
            if(*p==-1)break;
            n= create_node(*p);
            cursor->next=n;
            cursor=cursor->next;

        }
    }
    else{
        for(int i=1;i<length;i++){
            n= create_node(value[i]);
            cursor->next=n;
            cursor=cursor->next;
        }
    }

    return head;
}


// 初始化链表头
node *init_node(int value){
    node *head;
    head= create_node(value);
    return head;
}

// 创建节点
node *create_node(int value){
    node *n;
    n=(node *) malloc(sizeof(node));
    n->site=value;
    n->next=NULL;
    return n;
}
void remove_node(node *head,int value){
    node *cursor=head,*free_pointer;
    while (1){
        if(cursor->next==NULL){
            if(cursor->site==value){
                free(cursor);
                printf("找到此数值\n");
                break;
            }
            printf("没有此数值\n");
            break;
        }
        if(cursor->next->site==value){
            free_pointer=cursor->next;
            cursor->next=cursor->next->next;
            free(free_pointer);
            printf("找到数值==>%d\n",value);
            break;
        }
        cursor=cursor->next;
    }
}

void read_node(node *head){
    node *cursor=head;
    while (1){
        if(cursor==NULL)break;
        printf("%d\t",cursor->site);
        cursor=cursor->next;
    }
    printf("\n");
}

// 对每个node的可用站点数量排序
int *get_length_sort(int **node_sites,int node_num){
    // 定义光标
    int *cursor,*arr,*idxes;
    int **node_cur=node_sites;

    // 开辟内存
    arr=(int *) malloc(node_num* sizeof(int));
    for(int i=0;i<node_num;i++){
        cursor=*node_cur;
        arr[i]=0;
        while(1){
            if(*cursor==-1)break;

            arr[i]++;
            cursor++;
        }
        node_cur++;
    }
    idxes= sorted(arr,node_num);
    return idxes;
}

// 数组初始化
int ** pointer2array(int row_nums){
    int space=1024,space_num=1,init_space;
    int **pointer;
    init_space=space*space_num;

    pointer=(int **) malloc(row_nums* sizeof(int *));
    for(int i=0;i<row_nums;i++){
        pointer[i]=(int *) malloc(init_space*sizeof(int));
    }
    return pointer;
}


// 排序函数并返回排序后的位置
int * sorted(int *arr,int length){
    int *idxes;
    idxes=(int *) malloc(length*sizeof(int));
    for(int i=0;i<length;i++){
        idxes[i]=i;
    }
    for(int i=0;i<length;i++){
        int idx=i,value=*(arr+i);
        for(int j=i+1;j<length;j++){
            if(value>*(arr+j)){
                value=*(arr+j);
                idx=j;
            }
        }
        trans(arr+i,arr+idx);
        trans(idxes+i,idxes+idx);
    }
    return idxes;
}
// 数值交换方法
void trans(int*p,int *q){
    int t=*p;
    *p=*q;
    *q=t;
}