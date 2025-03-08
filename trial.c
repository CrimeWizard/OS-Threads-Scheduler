#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>

#define POLICY SCHED_FIFO 

double get_time_ms()
{
 struct timespec ts;
 clock_gettime(CLOCK_MONOTONIC, &ts);
 return ts.tv_sec * 1000.0 + ts.tv_nsec / 1.0e6; // Convert to milliseconds
}



void* func1(void* arg){
    printf("Inside the first thread\n");
    pthread_t id = pthread_self();
    printf ("first thread id %lu\n", (unsigned long) id);
    //Funciton start
    char ch1, ch2;
    printf("Enter two characters \n");
    scanf(" %c %c", &ch1, &ch2);

    if (ch1 > ch2){
        char temp = ch1;
        ch1 = ch2;
        ch2 = temp;
    }

    for (char i = ch1; i <= ch2; i++){
        printf("%c \n", i);
    }
    pthread_exit(NULL);
}

void* func2(void* arg){
    printf("Inside the second thread\n");
    pthread_t id = pthread_self();
    printf ("second thread id %lu\n", (unsigned long) id);
    
    printf("Print Statement 1 \n");
    printf("Print Statement 2 \n");
    printf("Print statement 3 \n");
    
    printf("Getting out of second thread, bye. \n");
    pthread_exit(NULL);
}

void* func3(void* arg){
    printf("Inside the third thread \n");
    pthread_t id = pthread_self();
    printf("Third thread id %lu \n", (unsigned long) id);
    // Function start
    int n1, n2, sum, avg, count;
    sum = 0;
    long long int product = 1;
    
    printf("Enter two non-negative integers \n");
    scanf("%d %d", &n1, &n2);
    if (n1 < 0 || n2 < 0){
        printf("Error: Only non-negative numbers allowed. \n");
        pthread_exit(NULL);
    }
    if (n1 > n2){
        int temp = n1;
        n1 = n2;
        n2 = temp;
    }
    
    count = n2-n1+1;
    
    if (count == 0){
        printf("you should enter some numbers \n");
        pthread_exit(NULL);
    }
    for (int i = n1; i <= n2; i++){
        sum+=i;
        product*=i;
    }
    avg = sum/count;
    printf("Sum of numbers between %d and %d is %d \n", n1,n2,sum);
    printf("Average of numbers between %d and %d is %d \n", n1,n2,avg);
    printf("Product of numbers between %d and %d is %lld \n", n1,n2,product);
    
    pthread_exit(NULL);
}


int main() {
    pthread_t ptid1, ptid2, ptid3;

    struct sched_param param; 

    // Define CPU affinity set
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset); // Initialize the CPU set
    CPU_SET(0, &cpuset); // Assign execution to CPU 0

    pthread_attr_t attr; // Thread attributes
    pthread_attr_init(&attr); // Initialize thread attributes
    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset); // Set thread CPU affinity

    pthread_attr_setschedpolicy(&attr, POLICY); //setting schedule policy

    if (POLICY == SCHED_FIFO || POLICY == SCHED_RR) {
        param.sched_priority = sched_get_priority_max(POLICY); // Highest priority
        pthread_attr_setschedparam(&attr, &param);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    }
    
    //Thread 1 
    pthread_create(&ptid1, &attr, &func1, NULL); 
    pthread_join(ptid1, NULL);
    
    // Thread 2
    pthread_create(&ptid2, &attr, &func2, NULL);
    pthread_join(ptid2, NULL);
    
    //Thread 3
    pthread_create(&ptid3, &attr, &func3, NULL);
    pthread_join(ptid3, NULL);
    
    pthread_attr_destroy(&attr); 

    printf("Threads Ended \n");
    
    return 0;
}