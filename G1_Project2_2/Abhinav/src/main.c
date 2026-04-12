#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"
#include "types.h"

// Reset all processes before each run
void init_tasks(Process p[], int n) {

    Process init[20] = {
        {1,0,8,8,0,READY,0,-1,-1},
        {2,1,25,25,0,READY,0,-1,-1},
        {3,2,3,3,0,READY,0,-1,-1},
        {4,5,12,12,0,READY,0,-1,-1},
        {5,8,2,2,0,READY,0,-1,-1},

        {6,10,40,40,0,READY,0,-1,-1},
        {7,12,5,5,0,READY,0,-1,-1},
        {8,15,6,6,0,READY,0,-1,-1},
        {9,20,15,15,0,READY,0,-1,-1},
        {10,22,4,4,0,READY,0,-1,-1},

        {11,25,8,8,0,READY,0,-1,-1},
        {12,30,50,50,0,READY,0,-1,-1},
        {13,35,2,2,0,READY,0,-1,-1},
        {14,40,9,9,0,READY,0,-1,-1},
        {15,45,3,3,0,READY,0,-1,-1},

        {16,50,18,18,0,READY,0,-1,-1},
        {17,55,5,5,0,READY,0,-1,-1},
        {18,60,7,7,0,READY,0,-1,-1},
        {19,65,22,22,0,READY,0,-1,-1},
        {20,70,4,4,0,READY,0,-1,-1}
    };

    for(int i=0;i<n;i++) p[i]=init[i];
}

int main() {
    int n = 20, m = 2;
    Process processes[20];

    int choice;

    while(1){
        init_tasks(processes,n);

        printf("\n1. Round Robin\n2. MLFQ\n3. Exit\nChoice: ");
        scanf("%d",&choice);

        if(choice==1) run_custom_rr(processes,n,m);
        else if(choice==2) run_mlfq(processes,n,m);
        else break;
    }
}