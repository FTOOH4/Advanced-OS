#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISK_SIZE 5000

void printMovement(int path[], int count, int total, const char *algo) {
    printf("\n[%s] Head Movement Order: ", algo);
    for (int i = 0; i < count; i++) {
        printf("%d", path[i]);
        if (i != count - 1) printf(" -> ");
    }
    printf("\n[%s] Total Seek Time = %d\n", algo, total);
}

void fcfs(int requests[], int n, int head) {
    int total = 0;
    int current = head;
    int path[1000];
    int idx = 0;

    path[idx++] = current;

    for (int i = 0; i < n; i++) {
        total += abs(requests[i] - current);
        current = requests[i];
        path[idx++] = current;
    }

    printMovement(path, idx, total, "FCFS");
}

void scan(int requests[], int n, int head) {
    int total = 0;
    int current = head;
    int path[2000], idx = 0;
    int sorted[1000];

    for (int i = 0; i < n; i++) sorted[i] = requests[i];

    // Sort requests
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (sorted[i] > sorted[j]) {
                int tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }

    int pos;
    for (pos = 0; pos < n; pos++) {
        if (sorted[pos] > head) break;
    }

    path[idx++] = current;

    // Move right
    for (int i = pos; i < n; i++) {
        total += abs(sorted[i] - current);
        current = sorted[i];
        path[idx++] = current;
    }

    if (current != DISK_SIZE - 1) {
        total += abs((DISK_SIZE - 1) - current);
        current = DISK_SIZE - 1;
        path[idx++] = current;
    }

    // Move left
    for (int i = pos - 1; i >= 0; i--) {
        total += abs(sorted[i] - current);
        current = sorted[i];
        path[idx++] = current;
    }

    printMovement(path, idx, total, "SCAN");
}

void cscan(int requests[], int n, int head) {
    int total = 0;
    int current = head;
    int path[2000], idx = 0;
    int sorted[1000];

    for (int i = 0; i < n; i++) sorted[i] = requests[i];

    // Sort requests
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (sorted[i] > sorted[j]) {
                int tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }

    int pos;
    for (pos = 0; pos < n; pos++) {
        if (sorted[pos] > head) break;
    }

    path[idx++] = current;

    // Move right
    for (int i = pos; i < n; i++) {
        total += abs(sorted[i] - current);
        current = sorted[i];
        path[idx++] = current;
    }

    if (current != DISK_SIZE - 1) {
        total += abs((DISK_SIZE - 1) - current);
        current = DISK_SIZE - 1;
        path[idx++] = current;
    }

    // Jump to 0
    total += DISK_SIZE - 1;
    current = 0;
    path[idx++] = current;

    for (int i = 0; i < pos; i++) {
        total += abs(sorted[i] - current);
        current = sorted[i];
        path[idx++] = current;
    }

    printMovement(path, idx, total, "C-SCAN");
}

int main() {
    while (1) {
        int numOfRequests, head;
        char algo[10];

        printf("\n==========================================\n");
        printf("Enter the number of disk requests: ");
        scanf("%d", &numOfRequests);

        int requests[1000],temp;
        printf("Enter the disk requests (0-4999):\n");
        for (int i = 0; i < numOfRequests; i++) {
             scanf("%d", &temp);
             while (temp<0 || temp>4999)  {
                    printf("Enter value within range (0-4999):\n");
                    scanf("%d", &temp);
                    }
              requests[i] = temp;
        }
        printf("Enter the initial position of disk head: ");
        scanf("%d", &head);

        printf("Enter the algorithm to use (FCFS / SCAN / C-SCAN): ");
        scanf("%s", algo);

        if (strcmp(algo, "FCFS") == 0) {
            fcfs(requests, numOfRequests, head);
        } else if (strcmp(algo, "SCAN") == 0) {
            scan(requests, numOfRequests, head);
        } else if (strcmp(algo, "C-SCAN") == 0 || strcmp(algo, "CSCAN") == 0) {
            cscan(requests, numOfRequests, head);
        } else {
            printf("Invalid algorithm!\n");
        }

        char choice;
        printf("\nDo you want to try again? (y/n): ");
        scanf(" %c", &choice);  // Note the space before %c to consume any leftover newline

        if (choice == 'n' || choice == 'N') {
            printf("Exiting program. Goodbye!\n");
            break;
        }
    }

    return 0;
}

