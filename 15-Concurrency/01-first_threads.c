/*
    First example of using threads in C

    Gilberto Echeverria
    04/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Function declaration
void * threadStart(void * arg);

int main()
{
    int num_threads;
    int i;
    pthread_t * tid = NULL;
    int * index = NULL;
    int * result = NULL;

    printf("Number of threads to run: ");
    scanf("%d", &num_threads);

    // Dynamic allocation of the array for the thread indices
    index = malloc(num_threads * sizeof(int));
    tid = malloc(num_threads * sizeof(pthread_t));

    // Create a new thread with the function as a starting point
    for (i=0; i<num_threads; i++)
    {
        index[i] = i + 1;
        pthread_create(&tid[i], NULL, threadStart, &index[i]);
        //printf("Created a new thread with id: %ld\n", tid);
    }

    printf("This is the main thread\n");

    // Wait for all threads to finish, and get the value they return
    for (i=0; i<num_threads; i++)
    {
        pthread_join(tid[i], (void**) &result);
        printf("Thread %d returned: %d\n", i+1, *result);
        free(result);
    }

    free(index);
    free(tid);

    pthread_exit(NULL);
}

void * threadStart(void * arg)
{
    int * id = (int *) arg;
    int num = *id;
    int * result = malloc(sizeof(int));
    *result = num * num;
    printf("This is the new thread: %d\n", *id);

    pthread_exit(result);
}
