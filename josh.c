#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>  
#include <stdlib.h> 
#include "graph.h"

typedef struct List{
    void (*job)(void*);
    List* nextjob;
} List;


struct queue {
    int size;
    List* firstjob;
    List* lastjob;
};

typedef struct queue* Queue;

typedef struct jobscheduler{
    int execution_threads;
    Queue q;
    pthread_t* tids;
    bool* thflag;
    pthread_mutex_t mutex;

} JobS;

JobS* initialize_scheduler(int execution_threads){
    JobS* sch = (JobS*) malloc (sizeof(JobS));
    sch->execution_threads = execution_threads;
    sch->q = (Queue*)malloc(sizeof(Queue));

    sch->tids = (pthread_t*)malloc(execution_threads * sizeof(pthread_t));
    if (sch->tids == NULL) {
        perror("Error allocating memory for thread IDs");
        free(sch);
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<execution_threads; i++){
        if (pthread_create(&sch->tids[i],NULL,NULL,NULL) != 0){
            perror("Error creating thread");
            free(sch->tids);
            free(sch);
            return 1;
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
        return 1;
    }

    return sch;
}



int submit_job(JobS* sch, void (*jobfunc)(void*)) {
    sch->q->lastjob = (Queue*)malloc(sizeof(Queue));

}

int start_execute(JobS* sch){}

int wait_all_tasks_finish(JobS* sch) {}

int destroy_scheduler(JobS* sch){}
