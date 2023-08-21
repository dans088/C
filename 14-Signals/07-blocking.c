/*
    Program to test the blocking of signals
    It can take a number of seconds to live as an argument

    Gilberto Echeverria
    gilecheverria@yahoo.com
    28/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define LIFETIME 20

// Function declarations
sigset_t setupMask();
void unsetMask(sigset_t old_set);
void detectBlocked(int signal);
void waitLoop();

int main(int argc, char * argv[])
{
    int seconds;
    sigset_t old_set;

    // Use a timeout obtained as an argument
    if (argc >= 2)
        seconds = atoi(argv[1]);
    // Otherwise use a default time
    else
        seconds = LIFETIME;

    // Change the signal handlers
    old_set = setupMask();

    // Wait a few seconds before quiting on its own
    waitLoop(seconds);

    detectBlocked(SIGINT);
    detectBlocked(SIGUSR1);

    // Remove the masks
    unsetMask(old_set);

    // Wait a few seconds before quiting on its own
    waitLoop(seconds);

    return 0;
}

// Function definitions

// Modify the signal mask
sigset_t setupMask()
{
    sigset_t new_set;
    sigset_t old_set;

    // Clear the set from all signals
    sigemptyset(&new_set);
    // Add the signals that I want blocked
    sigaddset(&new_set, SIGINT);
    sigaddset(&new_set, SIGTSTP);

    if (sigismember(&new_set, SIGTSTP))
    {
        printf("Blocking SIGTSTP\n");
    }

    // Apply the set to the program
    if (sigprocmask(SIG_BLOCK, &new_set, &old_set) == -1)
    {
        perror("sigprocmask");
    }

    return old_set;
}

// Reset the blocking mask to the previous one
void unsetMask(sigset_t old_set)
{
    sigprocmask(SIG_SETMASK, &old_set, NULL);
}

void detectBlocked(int signal)
{
    sigset_t temp_set;

    // Get any signals that have been received (while blocking)
    sigpending(&temp_set);

    if (sigismember(&temp_set, signal))
    {
        printf("Received %d\n", signal);
    }
}

// Wait a few seconds until the program ends
void waitLoop(int seconds)
{
    int i;

    // Show the PID so that it can be sent signals form the terminal
    //  or other process
    printf("My PID is: %d\n", getpid());

    printf("Will finish on my own in %3d s", seconds);
    fflush(stdout);
    // Loop to wait for something to happen
    for (i=seconds-1; i>=0; i--)
    {
        sleep(1);
        printf("\b\b\b\b\b%3d s", i);
        fflush(stdout);
    }
    printf("\nTime out. Good bye\n");
}
