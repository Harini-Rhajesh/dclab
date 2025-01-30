#include <stdio.h>
#include <stdlib.h>

#define MAX 20

int processes[MAX], n, coordinator;

// Function to display the process status
void display() {
    printf("\nProcesses: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", i);
    }

    printf("\nAlive:     ");
    for (int i = 0; i < n; i++) {
        printf("%d ", processes[i]);
    }

    printf("\nCurrent Coordinator: %d\n", coordinator);
}

// Function to elect a new coordinator (highest alive process)
void elect_coordinator() {
    for (int i = n - 1; i >= 0; i--) {
        if (processes[i]) {
            coordinator = i;
            return;
        }
    }
    coordinator = -1;  // No alive processes
}

// Function to simulate election process
void bully_algorithm() {
    int choice, pid, initiator;

    while (1) {
        printf("\n\n1. Crash Process\n2. Activate Process\n3. Display Status\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: // Crash a process
                printf("\nEnter process ID to crash (0 to %d): ", n - 1);
                scanf("%d", &pid);

                if (pid < 0 || pid >= n || !processes[pid]) {
                    printf("Invalid or already crashed process!\n");
                    break;
                }

                processes[pid] = 0;
                printf("Process %d crashed.\n", pid);

                if (pid == coordinator) {  // Coordinator crashed, start election
                    printf("\nEnter election initiator (0 to %d): ", n - 1);
                    do {
                        scanf("%d", &initiator);
                        if (initiator < 0 || initiator >= n || !processes[initiator]) {
                            printf("Enter a valid alive process: ");
                        }
                    } while (initiator < 0 || initiator >= n || !processes[initiator]);

                    for (int i = initiator + 1; i < n; i++) {
                        if (processes[i]) {
                            printf("Election message sent from %d to %d\n", initiator, i);
                            printf("Response received from %d to %d\n", i, initiator);
                            coordinator = i;
                        }
                    }
                    if (coordinator == pid) coordinator = initiator;
                }

                display();
                break;

            case 2: // Activate a process
                printf("\nEnter process ID to activate (0 to %d): ", n - 1);
                scanf("%d", &pid);

                if (pid < 0 || pid >= n || processes[pid]) {
                    printf("Invalid or already active process!\n");
                    break;
                }

                processes[pid] = 1;
                printf("Process %d activated.\n", pid);

                // If activated process is higher than the current coordinator, start election
                if (pid > coordinator) {
                    for (int i = pid + 1; i < n; i++) {
                        if (processes[i]) {
                            printf("Election message sent from %d to %d\n", pid, i);
                            printf("Response received from %d to %d\n", i, pid);
                            coordinator = i;
                        }
                    }
                    if (coordinator == pid) coordinator = pid;
                }

                display();
                break;

            case 3:
                display();
                break;

            case 4:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}

int main() {
    printf("Enter the number of processes (max %d): ", MAX);
    scanf("%d", &n);

    if (n <= 0 || n > MAX) {
        printf("Invalid number of processes! Exiting.\n");
        return 1;
    }

    // Initialize process states
    for (int i = 0; i < n; i++) {
        printf("Is Process %d alive? (1 for Yes, 0 for No): ", i);
        scanf("%d", &processes[i]);
    }

    // Elect initial coordinator
    elect_coordinator();

    if (coordinator == -1) {
        printf("No alive processes! Exiting.\n");
        return 1;
    }

    display();
    bully_algorithm();

    return 0;
}
