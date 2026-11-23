#include <iostream>
#include <thread>
#include <vector>
#include <semaphore.h>
#include <chrono>
#include <cstdio>

using namespace std;

// Shared resources
int shared_data = 0;   // The shared resource (dataset / buffer)
int read_count = 0;    // Number of active readers

// Semaphores
sem_t rw_mutex;  // Controls exclusive access to shared_data (used by both readers and writers)
sem_t mutex_sc;  // Protects critical section for updating read_count

// Reader function
void reader(int id)
{
    for (int i = 0; i < 3; i++)
    {
        // Entry Section
        sem_wait(&mutex_sc);
        read_count++;
        if (read_count == 1)
        {
            sem_wait(&rw_mutex); // First reader locks the writer
        }
        sem_post(&mutex_sc);

        // Critical Section (Reading)
        printf("[Reader %d] is READING shared_data = %d (Active Readers: %d)\n", id, shared_data, read_count);
        this_thread::sleep_for(chrono::milliseconds(600));

        // Exit Section
        sem_wait(&mutex_sc);
        read_count--;
        if (read_count == 0)
        {
            sem_post(&rw_mutex); // Last reader releases lock for writers
        }
        sem_post(&mutex_sc);

        // Remainder section
        this_thread::sleep_for(chrono::milliseconds(400));
    }
}

// Writer function
void writer(int id)
{
    for (int i = 0; i < 3; i++)
    {
        // Entry Section
        sem_wait(&rw_mutex); // Writer requests exclusive access

        // Critical Section (Writing)
        shared_data += 5;
        printf("  >>> [Writer %d] is WRITING. Updated shared_data to: %d <<<\n", id, shared_data);
        this_thread::sleep_for(chrono::milliseconds(800));

        // Exit Section
        sem_post(&rw_mutex); // Release exclusive access

        // Remainder section
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main()
{
    // Initialize semaphores:
    // sem_init(sem_t *sem, int pshared, unsigned int value)
    // pshared = 0: semaphore is shared between threads of the same process
    // value = 1: binary semaphore (initialized to 1)
    sem_init(&mutex_sc, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    const int NUM_READERS = 5;
    const int NUM_WRITERS = 2;

    vector<thread> readers;
    vector<thread> writers;

    // Start writer threads
    for (int i = 1; i <= NUM_WRITERS; i++)
    {
        writers.emplace_back(writer, i);
    }

    // Start reader threads
    for (int i = 1; i <= NUM_READERS; i++)
    {
        readers.emplace_back(reader, i);
    }

    // Wait for all threads to finish
    for (auto &r : readers)
    {
        r.join();
    }
    for (auto &w : writers)
    {
        w.join();
    }

    // Clean up semaphores
    sem_destroy(&mutex_sc);
    sem_destroy(&rw_mutex);

    printf("\nExecution finished. Final shared_data value = %d\n", shared_data);

    return 0;
}
