#include <stdio.h>

#define MAX 20

int processes[MAX], n, coordinator;

void display() {
    printf("\nProcesses: ");
    for (int i = 1; i <= n; i++)
        printf("%d ", i);

    printf("\nAlive:     ");
    for (int i = 1; i <= n; i++)
        printf("%d ", processes[i]);

    printf("\nCoordinator: %d\n", coordinator);
}

void start_election(int starter) {
    int highest = starter;

    for (int i = (starter % n) + 1; i != starter; i = (i % n) + 1) {
        if (processes[i]) {
            printf("\nMessage passed to %d", i);
            highest = (i > highest) ? i : highest;
        }
    }

    coordinator = highest;
    printf("\nNew Coordinator: %d\n", coordinator);
}

void crash_process() {
    int p;
    printf("\nEnter process number to crash: ");
    scanf("%d", &p);

    if (!processes[p]) {
        printf("\nProcess %d is already dead!", p);
        return;
    }

    processes[p] = 0;
    printf("\nProcess %d crashed!", p);

    if (p == coordinator) {
        int starter;
        do {
            printf("\nEnter election starter process: ");
            scanf("%d", &starter);
        } while (processes[starter] == 0);

        start_election(starter);
    }
}

void activate_process() {
    int p;
    printf("\nEnter process number to activate: ");
    scanf("%d", &p);

    if (processes[p]) {
        printf("\nProcess %d is already alive!", p);
        return;
    }

    processes[p] = 1;
    printf("\nProcess %d activated!", p);

    start_election(p);
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        printf("Is process %d alive? (0/1): ", i);
        scanf("%d", &processes[i]);
        if (processes[i]) coordinator = i;
    }

    int choice;
    do {
        printf("\n1. Crash Process\n2. Activate Process\n3. Display\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: crash_process(); break;
            case 2: activate_process(); break;
            case 3: display(); break;
            case 4: printf("\nExiting...\n"); break;
            default: printf("\nInvalid choice! Try again.");
        }
    } while (choice != 4);

    return 0;
}
