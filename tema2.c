#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAX 20



/*struct*/
struct param {
    int from;
    int to;
    int thread_num;
};

/*shared variables*/
int g_arr[MAX];
int found = -1 ;
int index_thread_found = -1;
int search_num;
/*mutex*/
pthread_mutex_t found_lock;
/*condition variable*/
pthread_cond_t found_cond;

/*prototypes*/
void gen_nums();
void * search_for_num(void * void_ptr);

int main(int argc , char * argv[]){
    srand(time(NULL));
    if(argc != 3){
        fprintf(stderr , "Usage: %s <number to search> <number of threads>\n" , argv[0] );
        exit(1);
    }
    
    //arguments 
    search_num = atoi(argv[1]);   
    int thread_num = atoi(argv[2]);
    //gen array
    gen_nums();
    pthread_t threads[thread_num];
    struct param p[thread_num];

    //init mutex
    pthread_mutex_init(&found_lock , NULL);
    pthread_cond_init(&found_cond , NULL);

    //initialize threads
    int i;
    int length = MAX/thread_num;
    int count = 0;
    for(i = 0 ; i < MAX && count < thread_num ; i+= length){
        p[count].thread_num = count + 1;
        p[count].from = i; 
        if(i + length < MAX - 1)
            p[count].to = i + length - 1;
        else
            p[count].to = MAX - 1;
        
        //create thread
        if(pthread_create(threads + count , NULL , &search_for_num , (void *) (p + count)))
            perror("Failed to create thread");
        count++;
    }


    for(i = 0 ; i < thread_num ; i++)
        if(pthread_join(threads[i] , NULL) != 0)
            perror("Failed to join threads");
    pthread_mutex_destroy(&found_lock);
    pthread_cond_destroy(&found_cond);

    if(found == -1){
        printf("El elemento %d no se encuentra en el vector\n" , search_num);
    }else{
        printf("%d encontrado en %d, por el thread %d\n" , search_num, found , index_thread_found);
    }
    return 0;
}
/*aux functions*/
void gen_nums(){
    int i;
    for(i = 0 ; i < MAX ; i++)
        g_arr[i] = rand() % 10;

    printf("Array generated...\n");
    for(i = 0 ; i < MAX ; i++)
        printf("%d " , g_arr[i]);
    printf("\n");
}
/*end of aux functions*/


//function for threads
void * search_for_num(void * void_ptr){
    struct param p = *((struct param *) void_ptr);
    int i;
    i = p.from;
    while(found == -1 && i < p.to){
        pthread_mutex_lock(&found_lock);
        if(g_arr[i] == search_num){
            found = i;
            index_thread_found = p.thread_num;
        }
        pthread_mutex_unlock(&found_lock);
        i++;
    }
}
