#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int main(int argc , char * argv[]){
    if(argc != 3){
        fprintf(stderr , "Usage: %s <number to search> <number of threads>\n" , argv[0] );
        exit(1);
    }
    int search_num = atoi(argv[1]);   
    int thread_num = atoi(argv[2]);

    printf("%d %d\n" , search_num , thread_num);
    return 0;
}