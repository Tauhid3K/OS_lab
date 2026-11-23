#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

// Two shared resources
timed_mutex resource1;
timed_mutex resource2;

// ============================================================================
// PART 1: DEADLOCK SIMULATION (Circular Wait)
// Thread 1 locks Resource 1 -> tries to lock Resource 2
// Thread 2 locks Resource 2 -> tries to lock Resource 1
// ============================================================================
void thread1_deadlock()
{
    resource1.lock();
    cout << "[Thread 1] Acquired Resource 1. Holding it and waiting for Resource 2...\n";
    this_thread::sleep_for(chrono::milliseconds(100));

    // Try to acquire Resource 2 with a timeout (avoids freezing the terminal indefinitely)
    if (resource2.try_lock_for(chrono::seconds(1)))
    {
        cout << "[Thread 1] Acquired Resource 2.\n";
        resource2.unlock();
    }
    else
    {
        cout << "[Thread 1] >>> DEADLOCK DETECTED! Cannot acquire Resource 2 (held by Thread 2) <<<\n";
    }

    resource1.unlock();
}

void thread2_deadlock()
{
    resource2.lock();
    cout << "[Thread 2] Acquired Resource 2. Holding it and waiting for Resource 1...\n";
    this_thread::sleep_for(chrono::milliseconds(100));

    // Try to acquire Resource 1 with a timeout
    if (resource1.try_lock_for(chrono::seconds(1)))
    {
        cout << "[Thread 2] Acquired Resource 1.\n";
        resource1.unlock();
    }
    else
    {
        cout << "[Thread 2] >>> DEADLOCK DETECTED! Cannot acquire Resource 1 (held by Thread 1) <<<\n";
    }

    resource2.unlock();
}

// ============================================================================
// PART 2: DEADLOCK PREVENTION (Lock Ordering / Hierarchy)
// Both threads acquire locks in the exact SAME order: Resource 1 -> Resource 2
// This eliminates the Circular Wait condition!
// ============================================================================
void thread1_safe()
{
    resource1.lock();
    resource2.lock();
    cout << "[Thread 1] Acquired both Resource 1 and Resource 2 safely.\n";
    resource2.unlock();
    resource1.unlock();
}

void thread2_safe()
{
    resource1.lock();
    resource2.lock();
    cout << "[Thread 2] Acquired both Resource 1 and Resource 2 safely.\n";
    resource2.unlock();
    resource1.unlock();
}

int main()
{
    cout << "========================================================\n";
    cout << "              SIMPLE DEADLOCK DEMONSTRATION             \n";
    cout << "========================================================\n\n";

    // 1. Demonstrate Deadlock
    cout << "--- 1. Simulating Deadlock (Opposite Lock Ordering) ---\n";
    cout << "Thread 1 requests: R1 -> then R2\n";
    cout << "Thread 2 requests: R2 -> then R1\n\n";

    thread t1(thread1_deadlock);
    thread t2(thread2_deadlock);
    t1.join();
    t2.join();

    cout << "\nExplanation:\n";
    cout << "Each thread holds one resource while waiting for the other.\n";
    cout << "Circular wait occurred: (Thread 1 -> R2 -> Thread 2 -> R1 -> Thread 1).\n\n";

    // 2. Demonstrate Prevention via Lock Ordering
    cout << "--- 2. Deadlock Prevention (Consistent Lock Ordering) ---\n";
    cout << "Both threads now request: R1 -> then R2\n\n";

    thread t3(thread1_safe);
    thread t4(thread2_safe);
    t3.join();
    t4.join();

    cout << "\nResult: Both threads completed successfully without deadlock!\n";
    cout << "========================================================\n";

    return 0;
}
