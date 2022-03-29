#include <stdio.h>

//#include <math.h>
#include <string.h>
#include "utils.h"
#include "service.h"
#include <stdlib.h>
int main() {
    char filePath[] = "./data/";
    char name[32] = "*********";
    const char *names[4] = {"demand.csv", "qos.csv", "site_bandwidth.csv", "config.ini"};
    demand d[3];
    int qos_constraint=0;

    // 读取文件
    for (int i = 0; i < 4; i++) {

        strcpy(name, filePath);

        strcat(name, names[i]);
        printf("正在读 %s ====请稍后\n", name);
        if(i<3)Reader(name, &d[i]);
        else qos_constraint=Read_qos_constraint(name);
    }
    if(qos_constraint==-1){
        return -1;
    }
    // 获取站点的focus值
//    int *focus,*idxes;
//    focus= get_site_focus_num(d[1].body,d[1].shape[0],d[1].shape[1],qos_constraint);
//    idxes= sorted(focus,d[1].shape[0]);
//    for(int i=0;i<d[1].shape[0];i++){
//        printf("idx=%d,value=%d\n",idxes[i],focus[i]);
//    }
//    printf("\n");

    int node_num=d[1].shape[1];
    int **p,*idxes;
//    node **heads;
//    heads=(node **) malloc(node_num* sizeof(node *));
    p= (int **)get_qos_site(node_num,qos_constraint,&d[1]);
    Res *res;
    res=run(d[0].body,p,d[2].body,d[0].shape[0],d[0].shape[1],d[1].shape[0]);

    for(int i=0;i<3;i++){
        printf("%d\t",*(*res->res_body+i));
    }


    return 0;
}

