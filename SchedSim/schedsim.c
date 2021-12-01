/* CSCI 401: Lab #6: Scheduling Simulator
 * Programmer (Student ID): Ben Corriette (@02956064)
 * Last modified date: 11/30/2021
 * 
 * Summary: A simulated process scheduler with using four algorithms.
 * 
 * Reference: Lab #4: Intro to Function Pointers
 */ 

#include <limits.h>
#include <stdlib.h>
#include <stdio.h> 

#include "process.h"
#include "util.h"

// Function to sort the Process acc. to ID
int id_comparer(const void *this, const void *that)
{ 
  ProcessType *process_one = (ProcessType *) this;
  ProcessType *process_two = (ProcessType *) that;
  int process_one_id = process_one->pid;
  int process_two_id = process_two->pid;
  
  if (process_one_id < process_two_id) {
    return 0;
  }
  return 1;
}

// Function to sort the Process acc. to burst time
int bt_comparer(const void *this, const void *that)
{ 
  ProcessType *process_one = (ProcessType *) this;
  ProcessType *process_two = (ProcessType *) that;
  int process_one_bt = process_one->bt;
  int process_two_bt = process_two->bt;
  
  if (process_one_bt < process_two_bt) {
    return 0;
  }
  return 1;
}

// Function to find the waiting time for all processes
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) 
{ 
  // Step 1: Create an array to monitor remaining burst times
  int rem_bt[n];
  // Step 2: Store waiting times of processes
  int rem_wt[n];
  
  for (int h = 0; h < n; h++) {
    rem_bt[h] = plist[h].bt;
    rem_wt[h] = 0;
  }
  // Step 3: Initialize time
  int time = 0;
  // Step 4: Keep traversing processes while all are not done
  for (int i = 0; i < n; i++) {
    if (rem_bt[i] > quantum) {
      time += quantum;
      rem_bt[i] -= quantum;
    } else {
      time += rem_bt[i];
      // Only assign value if the waiting time has not been set
      if (!rem_wt[i]) {
        rem_wt[i] = time - plist[i].bt;
        plist[i].wt = rem_wt[i];
      }
      rem_bt[i] = 0;
    }
    // On last iteration, restart iteration if processes are not done
    int are_processes_finished = 1;
    if (i == n - 1) {
      for (int j = 0; j < n; j++) {
        if (rem_bt[j] > 0) {
          are_processes_finished = 0;
        }
      }
      if (!are_processes_finished) {
        i = -1;
      }
    }
  }
} 

// Function to find the waiting time for all processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
  int mrt_index = -1;
  int current_process_const_bt = -1;
  int current_process_dynamic_bt = -1;
  int current_time = 0;
  int completion_time = 0;
  int processes_completed = 0;
  Comparer process_bt_comparer = &bt_comparer;
  
  // Sort the processes according to burst time
  qsort(plist, n, sizeof(ProcessType), process_bt_comparer);
 
  // Step 7: Increment time lap by one
  for (int time_lap = 0; processes_completed < n; time_lap++) {
    current_process_const_bt = plist[time_lap].bt;
    current_process_dynamic_bt = current_process_const_bt;
    // Step 1: Find process with minimum remaining time
    for (int i = 0; i < current_process_const_bt; i++, current_time++) {
      mrt_index = time_lap;

      // Step 2: Decrement its remaining time by one
      if (current_process_dynamic_bt > 0) {
        current_process_dynamic_bt -= 1;
      }

      // Step 3: Check if its remaining time is expired (equals zero)
      if (current_process_dynamic_bt == 0) {

        // Step 4: Increment the number of processes completed
        processes_completed += 1;

        // Step 5: Set the completion time of the current process
        completion_time = current_time + 1;

        // Step 6: Calculate the waiting time for each completed process
        plist[mrt_index].wt = 
            completion_time - 
            plist[mrt_index].art - 
            plist[mrt_index].bt;
      }
    } 
  }
} 

// Function to find the waiting time for all processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
}
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
  ProcessType *process_one = (ProcessType *) this;
  ProcessType *process_two = (ProcessType *) that;
  int process_one_priority = process_one->pri;
  int process_two_priority = process_two->pri;
  
  if (process_one_priority < process_two_priority) {
    return 1;
  }
  return 0;
}

//Function to calculate average time 
void findavgTimeFCFS(ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF(ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n);
  
    Comparer process_id_comparer = &id_comparer;
  
    // Re-sort the processes according to ID
    qsort(plist, n, sizeof(ProcessType), process_id_comparer);

    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority(ProcessType plist[], int n) 
{ 
    Comparer process_comparer = &my_comparer; 
    
    // Step 1: Input the processes
    // Step 2: Sort the processes by priority
    qsort(plist, n, sizeof(ProcessType), process_comparer);
  
    // Step 3: Apply First Come First Serve (FCFS) algorithm 
    findWaitingTime(plist, n);  
    findTurnAroundTime(plist, n); 
      
    //Display processes along with all details 
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS (Complete)
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 