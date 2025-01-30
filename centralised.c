#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROCESSES 10

int site1_wfg[MAX_PROCESSES][MAX_PROCESSES];  // Local WFG for Site 1
int site2_wfg[MAX_PROCESSES][MAX_PROCESSES];  // Local WFG for Site 2
int global_wfg[MAX_PROCESSES][MAX_PROCESSES]; // Global WFG
int num_processes_site1, num_processes_site2;

// Function to display a WFG
void display_wfg(int wfg[MAX_PROCESSES][MAX_PROCESSES], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_processes; j++) {
            printf("%d ", wfg[i][j]);
        }
        printf("\n");
    }
}

// Combine local WFGs into the global WFG
void combine_global_wfg() {
    memset(global_wfg, 0, sizeof(global_wfg)); // Reset global WFG

    // Merge dependencies from Site 1
    for (int i = 0; i < num_processes_site1; i++) {
        for (int j = 0; j < num_processes_site1; j++) {
            global_wfg[i][j] = site1_wfg[i][j];
        }
    }

    // Merge dependencies from Site 2
    for (int i = 0; i < num_processes_site2; i++) {
        for (int j = 0; j < num_processes_site2; j++) {
            global_wfg[i][j] |= site2_wfg[i][j];  // Use OR operation to merge dependencies
        }
    }
}

// Function to detect cycles in a WFG using DFS
bool dfs(int process, bool visited[], bool stack[], int wfg[MAX_PROCESSES][MAX_PROCESSES], int num_processes) {
    visited[process] = true;
    stack[process] = true;

    for (int i = 0; i < num_processes; i++) {
        if (wfg[process][i]) {
            if (!visited[i] && dfs(i, visited, stack, wfg, num_processes)) {
                return true;
            } else if (stack[i]) {
                return true;
            }
        }
    }

    stack[process] = false;
    return false;
}

// Function to check for deadlock in a WFG
bool detect_deadlock(int wfg[MAX_PROCESSES][MAX_PROCESSES], int num_processes) {
    bool visited[MAX_PROCESSES] = {false};
    bool stack[MAX_PROCESSES] = {false};

    for (int i = 0; i < num_processes; i++) {
        if (!visited[i]) {
            if (dfs(i, visited, stack, wfg, num_processes)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    int choice, site, from, to;

    // Initialize local WFGs
    memset(site1_wfg, 0, sizeof(site1_wfg));
    memset(site2_wfg, 0, sizeof(site2_wfg));

    printf("Enter the number of processes in Site 1 (max %d): ", MAX_PROCESSES);
    scanf("%d", &num_processes_site1);

    printf("Enter the number of processes in Site 2 (max %d): ", MAX_PROCESSES);
    scanf("%d", &num_processes_site2);

    int max_processes = (num_processes_site1 > num_processes_site2) ? num_processes_site1 : num_processes_site2;

    if (num_processes_site1 > MAX_PROCESSES || num_processes_site2 > MAX_PROCESSES) {
        printf("Error: Number of processes cannot exceed %d.\n", MAX_PROCESSES);
        return 1;
    }

    while (1) {
        printf("\n1. Add dependency\n2. Display local WFGs\n3. Combine and display global WFG\n4. Detect deadlock\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter the site (1 or 2): ");
            scanf("%d", &site);

            if (site == 1) {
                printf("Enter the process creating the dependency (0 to %d): ", num_processes_site1 - 1);
                scanf("%d", &from);
                printf("Enter the process it is waiting for (0 to %d): ", num_processes_site1 - 1);
                scanf("%d", &to);

                if (from >= 0 && from < num_processes_site1 && to >= 0 && to < num_processes_site1) {
                    site1_wfg[from][to] = 1;
                    printf("Dependency added in Site 1: P%d -> P%d\n", from, to);
                } else {
                    printf("Invalid process IDs.\n");
                }
            } else if (site == 2) {
                printf("Enter the process creating the dependency (0 to %d): ", num_processes_site2 - 1);
                scanf("%d", &from);
                printf("Enter the process it is waiting for (0 to %d): ", num_processes_site2 - 1);
                scanf("%d", &to);

                if (from >= 0 && from < num_processes_site2 && to >= 0 && to < num_processes_site2) {
                    site2_wfg[from][to] = 1;
                    printf("Dependency added in Site 2: P%d -> P%d\n", from, to);
                } else {
                    printf("Invalid process IDs.\n");
                }
            } else {
                printf("Invalid site.\n");
            }
        } else if (choice == 2) {
            printf("\nSite 1 WFG:\n");
            display_wfg(site1_wfg, num_processes_site1);

            printf("\nSite 2 WFG:\n");
            display_wfg(site2_wfg, num_processes_site2);
        } else if (choice == 3) {
            combine_global_wfg();
            printf("\nGlobal WFG:\n");
            display_wfg(global_wfg, max_processes);
        } else if (choice == 4) {
            combine_global_wfg();
            if (detect_deadlock(global_wfg, max_processes)) {
                printf("Deadlock detected in the system!\n");
            } else {
                printf("No deadlock detected.\n");
            }
        } else if (choice == 5) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
