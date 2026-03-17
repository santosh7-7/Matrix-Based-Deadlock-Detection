#include <stdio.h>

#define MAX_P 10
#define MAX_R 10

int allocation[MAX_P][MAX_R];
int request[MAX_P][MAX_R];
int available[MAX_R];
int n_proc, n_res;

void print_matrix(char *name, int mat[MAX_P][MAX_R], int rows, int cols) {
    printf("\n%s Matrix:\n", name);
    printf("        ");
    for (int j = 0; j < cols; j++) printf("R%d  ", j);
    printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("  P%d  [ ", i);
        for (int j = 0; j < cols; j++) printf("%d   ", mat[i][j]);
        printf("]\n");
    }
}

void print_available(int avail[], int cols) {
    printf("\nAvailable Resources:\n");
    printf("        ");
    for (int j = 0; j < cols; j++) printf("R%d  ", j);
    printf("\n  [ ");
    for (int j = 0; j < cols; j++) printf("%d   ", avail[j]);
    printf("]\n");
}

void detect_deadlock() {
    int work[MAX_R];
    int finish[MAX_P];
    int deadlocked[MAX_P];
    int safe_seq[MAX_P];
    int seq_count = 0;

    for (int j = 0; j < n_res; j++) work[j] = available[j];
    for (int i = 0; i < n_proc; i++) finish[i] = 0;

    int found;
    do {
        found = 0;
        for (int i = 0; i < n_proc; i++) {
            if (finish[i]) continue;
            int can = 1;
            for (int j = 0; j < n_res; j++) {
                if (request[i][j] > work[j]) { can = 0; break; }
            }
            if (can) {
                for (int j = 0; j < n_res; j++) work[j] += allocation[i][j];
                finish[i] = 1;
                safe_seq[seq_count++] = i;
                found = 1;
            }
        }
    } while (found);

    int deadlock_count = 0;
    for (int i = 0; i < n_proc; i++)
        if (!finish[i]) deadlocked[deadlock_count++] = i;

    printf("\n==========================================\n");
    printf("         DEADLOCK DETECTION RESULT\n");
    printf("==========================================\n");

    if (deadlock_count == 0) {
        printf("\n  STATUS: NO DEADLOCK DETECTED\n");
        printf("\n  All processes can complete successfully.\n");
        printf("\n  Safe Execution Order: ");
        for (int i = 0; i < seq_count; i++) printf("P%d ", safe_seq[i]);
        printf("\n");
    } else {
        printf("\n  STATUS: DEADLOCK DETECTED!\n");
        printf("\n  Deadlocked Processes: ");
        for (int i = 0; i < deadlock_count; i++) printf("P%d  ", deadlocked[i]);
        printf("\n");
        if (seq_count > 0) {
            printf("\n  Processes completed before deadlock: ");
            for (int i = 0; i < seq_count; i++) printf("P%d  ", safe_seq[i]);
            printf("\n");
        }
        printf("\n  REASON: Circular wait - each process is waiting\n");
        printf("  for a resource held by another process.\n");
        printf("\n  Request of deadlocked processes:\n");
        printf("          ");
        for (int j = 0; j < n_res; j++) printf("R%d  ", j);
        printf("\n");
        for (int i = 0; i < deadlock_count; i++) {
            int p = deadlocked[i];
            printf("    P%d  [ ", p);
            for (int j = 0; j < n_res; j++) printf("%d   ", request[p][j]);
            printf("]\n");
        }
    }
    printf("\n==========================================\n");
}

void get_input() {
    printf("\nEnter number of processes (max %d): ", MAX_P);
    scanf("%d", &n_proc);
    printf("Enter number of resource types (max %d): ", MAX_R);
    scanf("%d", &n_res);

    printf("\nEnter Allocation Matrix (%d x %d):\n", n_proc, n_res);
    for (int i = 0; i < n_proc; i++) {
        printf("  P%d -> ", i);
        for (int j = 0; j < n_res; j++) scanf("%d", &allocation[i][j]);
    }

    printf("\nEnter Request Matrix (%d x %d):\n", n_proc, n_res);
    for (int i = 0; i < n_proc; i++) {
        printf("  P%d -> ", i);
        for (int j = 0; j < n_res; j++) scanf("%d", &request[i][j]);
    }

    printf("\nEnter Available Resources (1 x %d):\n  ", n_res);
    for (int j = 0; j < n_res; j++) scanf("%d", &available[j]);
}

int main() {
    printf("==========================================\n");
    printf("     DEADLOCK DETECTION ALGORITHM\n");
    printf("==========================================\n");

    get_input();

    print_matrix("Allocation", allocation, n_proc, n_res);
    print_matrix("Request",    request,    n_proc, n_res);
    print_available(available, n_res);

    printf("\nRunning deadlock detection...\n");
    detect_deadlock();

    return 0;
}