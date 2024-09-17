#include <stdio.h>
#include <limits.h>

struct Process {
    int pid; // Process ID
    int bt;  // Burst Time
    int art; // Arrival Time
};

// Function to find the waiting time for all processes
void findWaitingTime(struct Process proc[], int n, int wt[], int gantt_chart[], int gc_index[], int *gc_count) {
    int rt[n];

    // Copy the burst time into rt[]
    for (int i = 0; i < n; i++)
        rt[i] = proc[i].bt;

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    int check = 0;

    // Process until all processes get completed
    while (complete != n) {
        // Find process with minimum remaining time among the processes that arrive till the current time
        for (int j = 0; j < n; j++) {
            if ((proc[j].art <= t) && (rt[j] < minm) && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = 1;
            }
        }

        if (check == 0) {
            t++;
            continue;
        }

        // If this process is executed, add it to the Gantt chart if it's the first time
        if (*gc_count == 0 || gantt_chart[*gc_count - 1] != proc[shortest].pid) {
            gantt_chart[*gc_count] = proc[shortest].pid;
            gc_index[*gc_count] = t;
            (*gc_count)++;
        }

        // Reduce remaining time by one
        rt[shortest]--;

        // Update minimum
        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        // If a process gets completely executed
        if (rt[shortest] == 0) {
            // Increment complete
            complete++;
            check = 0;

            // Find finish time of current process
            finish_time = t + 1;

            // Calculate waiting time
            wt[shortest] = finish_time - proc[shortest].bt - proc[shortest].art;

            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        // Increment time
        t++;
    }
    // Store the end time of the last process in Gantt chart
    gc_index[*gc_count] = t;
}

// Function to calculate turn around time
void findTurnAroundTime(struct Process proc[], int n, int wt[], int tat[]) {
    // Calculating turnaround time by adding bt[i] + wt[i]
    for (int i = 0; i < n; i++)
        tat[i] = proc[i].bt + wt[i];
}

// Function to calculate average time
void findavgTime(struct Process proc[], int n) {
    int wt[n], tat[n], total_wt = 0, total_tat = 0;
    int gantt_chart[100], gc_index[100], gc_count = 0; // Gantt chart arrays

    // Function to find waiting time of all processes
    findWaitingTime(proc, n, wt, gantt_chart, gc_index, &gc_count);

    // Function to find turn around time for all processes
    findTurnAroundTime(proc, n, wt, tat);

    // Display processes along with all details
    printf("\nP\tAT\tBT\tWT\tTAT\n");

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("%d\t%d\t%d\t%d\t%d\n", proc[i].pid, proc[i].art, proc[i].bt, wt[i], tat[i]);
    }

    printf("\nAverage waiting time = %.2f", (float)total_wt / (float)n);
    printf("\nAverage turn around time = %.2f\n", (float)total_tat / (float)n);

    // Display Gantt chart
    printf("\nGantt Chart:\n");
    // printf("---------------------------------------------------\n");
    for (int i = 0; i < gc_count; i++) {
        printf("|  P%d  ", gantt_chart[i]);
    }
    printf("|\n");

    // Print the timeline
    for (int i = 0; i <= gc_count; i++) {
        printf("%d", gc_index[i]);
        if (i != gc_count) {
            // Adjust space between times based on the process ID length (assumed fixed width for simplicity)
            float spaces = 5.5; 
            for (int j = 0; j < spaces; j++) printf(" ");
        }
    }
    printf("\n");
}

// Driver code
int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Enter arrival time for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].art);
        printf("Enter burst time for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].bt);
    }

    findavgTime(proc, n);
    return 0;
}
