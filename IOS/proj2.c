#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAX_CARS 10000
#define MAX_CAPACITY 100
#define PORTS 2

// Shared memory structure
typedef struct {
  int action_counter;
  int cars_in_port[PORTS];
  int trucks_in_port[PORTS];
  int cars_on_ferry;
  int trucks_on_ferry;
  int cars_transported;
  int trucks_transported;
  int total_cars;
  int total_trucks;
  int ferry_port;
  bool loading_complete;
  int remaining_actions;
} SharedData;

// Semaphore operations
void sem_wait(int semid, int sem_num) {
  struct sembuf op = {sem_num, -1, 0};
  if (semop(semid, &op, 1) == -1) {
    perror("semop wait failed");
    exit(1);
  }
}

void sem_signal(int semid, int sem_num) {
  struct sembuf op = {sem_num, 1, 0};
  if (semop(semid, &op, 1) == -1) {
    perror("semop signal failed");
    exit(1);
  }
}

// Global variables for IPC
int shmid;
SharedData *shared;
int semid;

// Semaphore indices
enum {
  SEM_MUTEX,
  SEM_FERRY,
  SEM_PORT0_CAR,
  SEM_PORT0_TRUCK,
  SEM_PORT1_CAR,
  SEM_PORT1_TRUCK,
  SEM_COUNT
};

// Forward declaration of cleanup function
void cleanup(void) {
  if (shared != (void *)-1) {
    if (shmdt(shared) == -1) {
      perror("shmdt failed in cleanup");
    }
    shared = (void *)-1;
  }
  if (shmid != -1) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
      perror("shmctl IPC_RMID failed in cleanup");
    }
    shmid = -1;
  }
  if (semid != -1) {
    if (semctl(semid, 0, IPC_RMID) == -1) {
      perror("semctl IPC_RMID failed in cleanup");
    }
    semid = -1;
  }
}

// Function to write action to output file
void write_action(const char *format, ...) {
  FILE *f = fopen("proj2.out", "a");
  if (f == NULL) {
    perror("Failed to open output file");
    exit(1);
  }

  va_list args;
  va_start(args, format);

  fprintf(f, "%d: ", shared->action_counter++);
  vfprintf(f, format, args);
  fflush(f);

  va_end(args);
  fclose(f);
}

// Ferry process
void ferry_process(int K, int TP) {
  write_action("P: started\n");

  while (true) {
    // Simulate travel to port
    usleep(rand() % (TP + 1));

    int current_port = shared->ferry_port;
    write_action("P: arrived to %d\n", current_port);

    // Signal waiting trucks and cars that the ferry has arrived
    sem_wait(semid, SEM_MUTEX);
    for (int i = 0; i < shared->trucks_in_port[current_port]; i++) {
      sem_signal(semid, current_port == 0 ? SEM_PORT0_TRUCK : SEM_PORT1_TRUCK);
    }
    for (int i = 0; i < shared->cars_in_port[current_port]; i++) {
      sem_signal(semid, current_port == 0 ? SEM_PORT0_CAR : SEM_PORT1_CAR);
    }
    sem_signal(semid, SEM_MUTEX);

    // Unload vehicles
    while (shared->trucks_on_ferry > 0 || shared->cars_on_ferry > 0) {
      sem_wait(semid, SEM_MUTEX);
      if (shared->trucks_on_ferry > 0) {
        shared->trucks_on_ferry--;
        shared->trucks_transported++;
        sem_signal(semid, current_port == 0 ? SEM_PORT0_TRUCK : SEM_PORT1_TRUCK);
      } else if (shared->cars_on_ferry > 0) {
        shared->cars_on_ferry--;
        shared->cars_transported++;
        sem_signal(semid, current_port == 0 ? SEM_PORT0_CAR : SEM_PORT1_CAR);
      }
      sem_signal(semid, SEM_MUTEX);
    }

    // Load vehicles
    bool truck_turn = true;
    while ((shared->cars_in_port[current_port] > 0 ||
            shared->trucks_in_port[current_port] > 0) &&
           (shared->cars_on_ferry + (truck_turn ? 3 : 1) <= K)) {
      sem_wait(semid, SEM_MUTEX);
      if (truck_turn && shared->trucks_in_port[current_port] > 0 &&
          (shared->cars_on_ferry + 3) <= K) {
        shared->trucks_in_port[current_port]--;
        shared->trucks_on_ferry++;
        sem_signal(semid, current_port == 0 ? SEM_PORT0_TRUCK : SEM_PORT1_TRUCK);
      } else if (shared->cars_in_port[current_port] > 0 &&
                 (shared->cars_on_ferry + 1) <= K) {
        shared->cars_in_port[current_port]--;
        shared->cars_on_ferry++;
        sem_signal(semid, current_port == 0 ? SEM_PORT0_CAR : SEM_PORT1_CAR);
      }
      sem_signal(semid, SEM_MUTEX);

      truck_turn = !truck_turn;
    }

    write_action("P: leaving %d\n", current_port);

    // Switch to the next port
    shared->ferry_port = (current_port + 1) % 2;

    // Check if all vehicles have been transported
    if (shared->cars_transported >= shared->total_cars &&
        shared->trucks_transported >= shared->total_trucks &&
        shared->cars_on_ferry == 0 && shared->trucks_on_ferry == 0) {
      break;
    }
  }

  // Final synchronization
  sem_wait(semid, SEM_MUTEX);
  while (shared->remaining_actions > 0) {
    sem_signal(semid, SEM_MUTEX);
    usleep(100); // Wait for other processes to finish
    sem_wait(semid, SEM_MUTEX);
  }
  sem_signal(semid, SEM_MUTEX);

  // Return to port
  usleep(rand() % (TP + 1));
  write_action("P: finish\n");
}

void truck_process(int id, int port, int TA) {
  sem_wait(semid, SEM_MUTEX);
  write_action("N %d: started\n", id);
  sem_signal(semid, SEM_MUTEX);

  // Travel to port
  usleep(rand() % (TA + 1));

  sem_wait(semid, SEM_MUTEX);
  write_action("N %d: arrived to %d\n", id, port);
  shared->trucks_in_port[port]++;
  sem_signal(semid, SEM_MUTEX);

  // Wait for ferry to arrive
  sem_wait(semid, port == 0 ? SEM_PORT0_TRUCK : SEM_PORT1_TRUCK);

  sem_wait(semid, SEM_MUTEX);
  write_action("N %d: boarding\n", id);
  sem_signal(semid, SEM_MUTEX);

  // Wait to arrive at destination
  int dest_port = (port + 1) % 2;
  sem_wait(semid, dest_port == 0 ? SEM_PORT0_TRUCK : SEM_PORT1_TRUCK);

  sem_wait(semid, SEM_MUTEX);
  write_action("N %d: leaving in %d\n", id, dest_port);
  sem_signal(semid, SEM_MUTEX);

  exit(0);
}


// Car process
void car_process(int id, int port, int TA) {
  sem_wait(semid, SEM_MUTEX);
  write_action("O %d: started\n", id);
  sem_signal(semid, SEM_MUTEX);

  // Travel to port
  usleep(rand() % (TA + 1));

  sem_wait(semid, SEM_MUTEX);
  write_action("O %d: arrived to %d\n", id, port);
  shared->cars_in_port[port]++;
  sem_signal(semid, SEM_MUTEX);

  // Wait for ferry *after* arriving and incrementing count
  sem_wait(semid, port == 0 ? SEM_PORT0_CAR : SEM_PORT1_CAR);

  sem_wait(semid, SEM_MUTEX);
  write_action("O %d: boarding\n", id);
  sem_signal(semid, SEM_MUTEX);

  // Wait to arrive at destination
  int dest_port = (port + 1) % 2;
  sem_wait(semid, dest_port == 0 ? SEM_PORT0_CAR : SEM_PORT1_CAR);

  sem_wait(semid, SEM_MUTEX);
  write_action("O %d: leaving in %d\n", id, dest_port);
  sem_signal(semid, SEM_MUTEX);

  exit(0);
}


/// bla bla
int car_prc(int i, int TA) {
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork failed");
    cleanup();
    return 1;
  } else if (pid == 0) {
    srand(time(NULL) * getpid());
    int port = rand() % 2;
    car_process(i, port, TA);
  }
  return 0;
}

int truck_prc(int i, int TA) {
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork failed");
    cleanup();
    return 1;
  } else if (pid == 0) {
    srand(time(NULL) * getpid());
    int port = rand() % 2;
    truck_process(i, port, TA);
    exit(0);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 6) {
    fprintf(stderr, "Usage: %s N O K TA TP\n", argv[0]);
    return 1;
  }

  int N = atoi(argv[1]);  // Number of trucks
  int O = atoi(argv[2]);  // Number of cars
  int K = atoi(argv[3]);  // Ferry capacity
  int TA = atoi(argv[4]); // Max car travel time
  int TP = atoi(argv[5]); // Max ferry travel time

  // Validate input
  if (N < 0 || N >= 10000 || O < 0 || O >= 10000 || K < 3 || K > 100 ||
      TA < 0 || TA > 10000 || TP < 0 || TP > 1000) {
    fprintf(stderr, "Invalid input parameters\n");
    return 1;
  }

  // printf("%d\n", rand());
  // printf("%d\n", rand());
  // printf("%d\n", rand());
  // printf("%d\n", rand());
  // printf("%d\n", rand());
  // printf("%d\n", rand());

  // Initialize random number generator
  srand(time(NULL));

  // Create or open shared memory
  shmid = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget failed");
    return 1;
  }

  shared = (SharedData *)shmat(shmid, NULL, 0);
  if (shared == (void *)-1) {
    perror("shmat failed");
    shmctl(shmid, IPC_RMID, NULL);
    return 1;
  }

  // Initialize shared data
  memset(shared, 0, sizeof(SharedData));
  shared->action_counter = 1;
  shared->total_cars = O;
  shared->total_trucks = N;
  shared->ferry_port = 0;

  // Create semaphores
  semid = semget(IPC_PRIVATE, SEM_COUNT, IPC_CREAT | 0666);
  if (semid == -1) {
    perror("semget failed");
    cleanup();
    return 1;
  }

  // Initialize semaphores
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  } arg;

  arg.val = 1;
  if (semctl(semid, SEM_MUTEX, SETVAL, arg) == -1) {
    perror("semctl SEM_MUTEX failed");
    cleanup();
    return 1;
  }

  arg.val = 0;
  for (int i = 1; i < SEM_COUNT; i++) {
    if (semctl(semid, i, SETVAL, arg) == -1) {
      perror("semctl failed");
      cleanup();
      return 1;
    }
  }

  // Create output file
  FILE *f = fopen("proj2.out", "w");
  if (f == NULL) {
    perror("Failed to create output file");
    cleanup();
    return 1;
  }
  fclose(f);

  // Fork ferry process
  pid_t ferry_pid = fork();
  if (ferry_pid == -1) {
    perror("fork failed");
    cleanup();
    return 1;
  } else if (ferry_pid == 0) {
    srand(time(NULL) * getpid());
    ferry_process(K, TP);
    exit(0);
  }

  // Fork car processes
  // for (int i = 1; i <= O; i++) {
  //   pid_t pid = fork();
  //   if (pid == -1) {
  //     perror("fork failed");
  //     cleanup();
  //     return 1;
  //   } else if (pid == 0) {
  //     srand(time(NULL) * getpid());
  //     int port = rand() % 2;
  //     printf("%d\n", rand());
  //     car_process(i, port, TA);
  //     exit(0);
  //   }
  // }

  int car_counter = O;
  int truck_counter = N;
  for (int j = 0; j < N + O; j++) {
    int r = rand() % 2;
    if (r == 0) {
      if (car_counter > 0) {
        car_prc(O - car_counter + 1, TA);
        car_counter--;
      } else {
        truck_prc(N - truck_counter + 1, TA);
        truck_counter--;
      }
    } else {
      if (truck_counter > 0) {
        truck_prc(N - truck_counter + 1, TA);
        truck_counter--;
      } else {
        car_prc(O - car_counter + 1, TA);
        car_counter--;
      }
    }
  }

  // Wait for all child processes to finish
  for (int i = 0; i < 1 + N + O; i++) {
    wait(NULL);
  }

  // Cleanup
    cleanup();
 return 0;
}