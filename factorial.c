#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int turno[20] = {0};
int cont = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* factfun(void* arg){
    while(cont < 20){
        pthread_mutex_lock(&mutex);
        int num = *(int*)arg;
        if(num == turno[cont]){
            long fact = 1;
            for(int i = 1; i <= num; i++){
                fact = fact * i;
            }
            printf("Factorial de %d es: %ld \n", num, fact);
            free(arg);
            cont++;
        }else{
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
}

int main(int argc, char *argv[]){
    pthread_t fact_t[20];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    char* aux;
    int num;
    for(int i = 1; i < argc; ++i){
        aux = argv[i];
        num = atoi(aux);
        turno[i-1] = num;
        int* index = malloc(sizeof(int));
        *index = num;
        if(pthread_create(&fact_t[i], NULL, &factfun, index) != 0){
            perror("No se pudo crear el thread");
        }        
    }
    for(int i = 1; i < argc; ++i){
        if(pthread_join(fact_t[i], NULL) != 0){
            perror("No se pudo crear el thread");
        } 
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}