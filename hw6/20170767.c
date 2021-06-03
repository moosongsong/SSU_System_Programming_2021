#include "20170767.h"

// An example of a semphore for the mutual exclusion
semaphore_t * mutex;
FILE *fp = NULL;
char buffer[NUM_OF_READER][BUFFER_SIZE];
int isEOF;

void reader(void)
{
    while (1) {
        P(mutex);
        if (feof(fp)) {
            isEOF = 1;
            break;
        }
        fscanf(fp, "%s", buffer[get_thread_id() - 1]);
        printf("Thread %d is reading a line\n", get_thread_id());
        V(mutex);
    }
}

void writer(void)
{
    while (1) {
        P(mutex);
        printf("Thread %d is writing 3 lines\n", get_thread_id());
        for (int i = 0; i < NUM_OF_READER; ++i) {
            printf("%s\n", buffer[i]);
        }
        if (isEOF) {
            break;
        }
        V(mutex);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: 20170767 <file>\n");
        return 0;
    }
    fp = fopen(argv[1], "r");

    // example of initializing a semaphore
    mutex = sem_create(1);

    // create threads
    create_thread(reader);
    create_thread(reader);
    create_thread(reader);
    create_thread(writer);

    // Let's run
    run();

    // stupid while loop
    while (1) {
        sleep(1);
    }

}




