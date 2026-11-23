#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

// ============================================================================
// SIMPLE DINING PHILOSOPHERS USING MONITOR
// (Silberschatz - Operating System Concepts Section 7.1.3)
//
// Deadlock Prevention:
//   A philosopher is allowed to pick up chopsticks ONLY if both neighbors
//   are NOT eating. This eliminates circular wait.
// ============================================================================

const int N = 5; // 5 philosophers (0 to 4)

class DiningPhilosophersMonitor
{
private:
    enum State { THINKING, HUNGRY, EATING };
    State state[N];
    condition_variable self[N]; // Condition variable for each philosopher
    mutex mtx;                  // Monitor lock for mutual exclusion

    // Check if philosopher i can start eating
    void test(int i)
    {
        // Left neighbor: (i + 4) % N, Right neighbor: (i + 1) % N
        if (state[i] == HUNGRY &&
            state[(i + 4) % N] != EATING &&
            state[(i + 1) % N] != EATING)
        {
            state[i] = EATING;
            self[i].notify_one(); // Signal philosopher i to start eating
        }
    }

public:
    DiningPhilosophersMonitor()
    {
        for (int i = 0; i < N; i++)
        {
            state[i] = THINKING;
        }
    }

    // Philosopher i requests both chopsticks
    void pickup(int i)
    {
        unique_lock<mutex> lock(mtx);

        state[i] = HUNGRY;
        cout << "[Philosopher " << i << "] is HUNGRY and wants to eat.\n";

        test(i); // Try to acquire both chopsticks

        // If either chopstick is taken by a neighbor, wait
        while (state[i] != EATING)
        {
            self[i].wait(lock);
        }

        cout << "  >>> [Philosopher " << i << "] acquired both chopsticks and is EATING! <<<\n";
    }

    // Philosopher i puts down both chopsticks
    void putdown(int i)
    {
        unique_lock<mutex> lock(mtx);

        state[i] = THINKING;
        cout << "[Philosopher " << i << "] put down chopsticks and is now THINKING.\n";

        // Check if left and right neighbors are hungry and can now eat
        test((i + 4) % N);
        test((i + 1) % N);
    }
};

DiningPhilosophersMonitor dpMonitor;

void philosopher(int id)
{
    for (int cycle = 1; cycle <= 2; cycle++)
    {
        // 1. Thinking
        this_thread::sleep_for(chrono::milliseconds(200 + (id * 50)));

        // 2. Pickup chopsticks (via Monitor)
        dpMonitor.pickup(id);

        // 3. Eating
        this_thread::sleep_for(chrono::milliseconds(300));

        // 4. Put down chopsticks (via Monitor)
        dpMonitor.putdown(id);
    }
}

int main()
{
    cout << "========================================================\n";
    cout << "        DINING PHILOSOPHERS PROBLEM USING MONITOR       \n";
    cout << "========================================================\n\n";

    thread philosophers[N];

    // Start 5 philosopher threads
    for (int i = 0; i < N; i++)
    {
        philosophers[i] = thread(philosopher, i);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < N; i++)
    {
        philosophers[i].join();
    }

    cout << "\n========================================================\n";
    cout << "Result: All philosophers finished dining without deadlock!\n";
    cout << "========================================================\n";
    return 0;
}
