#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>  
#include <stdlib.h> 
#include "graph.h"

typedef struct list List;
typedef struct queue* Queue;
typedef struct job Job;

struct job {
    void (*job)(void*);
    void* args;
};

struct list{
    Job* j;
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
    pthread_mutex_t mutex;
    pthread_cond_t condv;
    bool destroy;
} JobS;

Job* jobcreate (void (*job)(void*), void* args){
    Job* j = (Job*) malloc(sizeof(Job));
    j->job = job;
    j->args = args;

    return j;
}


JobS* initialize_scheduler(int execution_threads){
    JobS* sch = (JobS*) malloc (sizeof(JobS));
    sch->execution_threads = execution_threads;
    sch->q = (Queue) malloc (sizeof(struct queue));
    sch->q->size = 0;
    sch->q->firstjob = NULL;
    sch->q->lastjob = NULL;

    sch->tids = (pthread_t*)malloc(execution_threads * sizeof(pthread_t));

    if (pthread_mutex_init(&sch->mutex, NULL) != 0) {
        perror("Error initializing mutex");
        free(sch->tids);
        free(sch);
        return NULL;
    }

    if (pthread_cond_init(&sch->condv, NULL) != 0) {
        perror("Error initializing condition variable");
        free(sch->tids);
        free(sch);
        return NULL;
    }

    sch->destroy = false;

    return sch;
}

int submit_job(JobS* sch, Job* j) {
    pthread_mutex_lock(&sch->mutex);

    if(sch->q->size == 0) {
        sch->q->firstjob = (List*)malloc(sizeof(List));
        sch->q->firstjob->j = j;
        sch->q->lastjob = sch->q->firstjob;
        sch->q->lastjob->nextjob = NULL;
    }
    else {
        List* newjob = (List*)malloc(sizeof(List));
        newjob->nextjob = NULL;
        newjob->j = j;
        sch->q->lastjob->nextjob = newjob;
        sch->q->lastjob = newjob;
    }

    sch->q->size++;
    pthread_mutex_unlock(&sch->mutex);

    pthread_cond_signal(&sch->condv);
    
    return 0;
}

int start_execute(JobS* sch) {

    for (int i = 0; i < sch->execution_threads; ++i) {
        pthread_create(&sch->tids[i], NULL, execute, sch);
    }

    return 0; 
}

void* execute(void* s){
    JobS* sch = (JobS*)s;

    while (1) {
        pthread_mutex_lock(&sch->mutex);

        while (sch->q->size == 0 && !sch->destroy) {
            pthread_cond_wait(&sch->condv, &sch->mutex);
        }

        if (sch->destroy) {
            pthread_mutex_unlock(&sch->mutex);
            pthread_exit(NULL);
        }

        List* current_job = sch->q->firstjob;
        if (current_job != NULL) {
         
            current_job->j->job(current_job->j->args); 

            sch->q->firstjob = current_job->nextjob;
            free(current_job);
            sch->q->size--;
        }        
        pthread_mutex_unlock(&sch->mutex);

        pthread_cond_signal(&sch->condv);

    }

    return NULL;
}

int wait_all_tasks_finish(JobS* sch) {
    pthread_mutex_lock(&sch->mutex);

    while (sch->q->size != 0 && !sch->destroy){
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

    free(sch->tids);
    pthread_mutex_destroy(&sch->mutex);
    pthread_cond_destroy(&sch->condv);
    free(sch->q);
    free(sch);

    return 0;
}