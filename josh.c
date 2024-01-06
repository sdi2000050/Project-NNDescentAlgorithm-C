#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>  
#include <stdlib.h> 
#include "graph.h"

typedef struct list List;
typedef struct queue* Queue;

struct list{
    void (*job)(void*);
    void* args;
    List* nextjob;
};

struct queue {
    int size;
    List* firstjob;
    List* lastjob;
};


typedef struct jobscheduler{
    int execution_threads;
    Queue q;
    pthread_t* tids;
    bool* thflag;
    pthread_mutex_t mutex;
    pthread_cond_t condv;
    bool destroy;
} JobS;


JobS* initialize_scheduler(int execution_threads){
    JobS* sch = (JobS*) malloc (sizeof(JobS));
    sch->execution_threads = execution_threads;
    sch->q = (Queue) malloc (sizeof(struct queue));
    sch->q->size = 0;
    sch->q->firstjob = NULL;
    sch->q->lastjob = NULL;

    sch->tids = (pthread_t*)malloc(execution_threads * sizeof(pthread_t));
    if (sch->tids == NULL) {
        perror("Error allocating memory for thread IDs");
        free(sch);
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<execution_threads; i++){
        if (pthread_create(&sch->tids[i],NULL,start_execute,(void*)sch) != 0){
            perror("Error creating thread");
            free(sch->tids);
            free(sch);
            return NULL;
        }
    }
    
    sch->thflag = (bool*) malloc (execution_threads*sizeof(bool));
    if (sch->thflag == NULL) {
        perror("Error allocating memory for thread flags");
        free(sch->tids);
        free(sch);
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<execution_threads; i++){
        sch->thflag[i] = false;
    }

    if (pthread_mutex_init(&sch->mutex, NULL) != 0) {
        perror("Error initializing mutex");
        free(sch->tids);
        free(sch->thflag);
        free(sch);
        return NULL;
    }

    if (pthread_cond_init(&sch->condv, NULL) != 0) {
        perror("Error initializing condition variable");
        free(sch->tids);
        free(sch->thflag);
        free(sch);
        return NULL;
    }

    sch->destroy = false;

    return sch;
}

int submit_job(JobS* sch, void (*jobfunc)(void*), void* args) {
    pthread_mutex_lock(&sch->mutex);

    if(sch->q->size == 0) {
        sch->q->firstjob = (List*)malloc(sizeof(List));
        sch->q->firstjob->job = jobfunc;
        sch->q->firstjob->args = args;
        sch->q->lastjob = sch->q->firstjob;
        sch->q->lastjob->nextjob = NULL;
        sch->q->size++;
    }
    else {
        List* newjob = (List*)malloc(sizeof(List));
        newjob->nextjob = NULL;
        newjob->job = jobfunc;
        newjob->args = args;
        sch->q->lastjob->nextjob = newjob;
        sch->q->size++;
    }

    pthread_mutex_unlock(&sch->mutex);

    pthread_cond_signal(&sch->condv);

    return 0;
}

void* start_execute(void* s){
    JobS* sch = (JobS*)s;

    while (1) {
        pthread_mutex_lock(&sch->mutex);

        while (sch->q->size == 0 && !sch->destroy) {
            printf("size %d\n",sch->q->size);
            pthread_cond_wait(&sch->condv, &sch->mutex);
        }

        if (sch->destroy) {
            pthread_mutex_unlock(&sch->mutex);
            pthread_exit(NULL);
        }

        List* current_job = sch->q->firstjob;
        if (current_job != NULL) {
            pthread_t thread_id = pthread_self();
            printf("Thread ID: %lu\n", (unsigned long)thread_id);
            printf("thread running size:%d\n",sch->q->size);
            current_job->job(current_job->args); 
            printf("job okey\n");
            sch->q->firstjob = current_job->nextjob;
            free(current_job);
            sch->q->size--;

            pthread_cond_signal(&sch->condv);
        }        

        pthread_mutex_unlock(&sch->mutex);
    }

    return NULL;
}

int wait_all_tasks_finish(JobS* sch) {
    pthread_mutex_lock(&sch->mutex);
    
    while (sch->q->size != 0 && !sch->destroy){
        printf("waiting...\n");
        pthread_cond_wait(&sch->condv,&sch->mutex);
    }

    pthread_mutex_unlock(&sch->mutex);

    return 0;
}


int destroy_scheduler(JobS* sch) {
    pthread_mutex_lock(&sch->mutex);
    sch->destroy = true;
    pthread_cond_broadcast(&sch->condv); 
    pthread_mutex_unlock(&sch->mutex);

    for (int i = 0; i < sch->execution_threads; i++) {
        pthread_join(sch->tids[i], NULL);
    }

    free(sch->tids);
    pthread_mutex_destroy(&sch->mutex);
    pthread_cond_destroy(&sch->condv);
    List* current = sch->q->firstjob;
    List* next;
    while (current != NULL) {
        next = current->nextjob;
        free(current);
        current = next;
    }
    free(sch->q);
    free(sch);

    return 0;
}
