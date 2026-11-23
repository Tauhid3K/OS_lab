#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

// ============================================================================
// SIMPLE MONITOR IMPLEMENTATION (Bounded Buffer / Producer-Consumer)
//
// A Monitor encapsulates:
//   1. Shared data (buffer, count)
//   2. Mutual exclusion lock (only one thread active inside monitor)
//   3. Condition variables (threads wait/signal based on buffer state)
// ============================================================================

const int BUFFER_SIZE = 4;

class BoundedBufferMonitor
{
private:
    int buffer[BUFFER_SIZE];
    int in = 0;
    int out = 0;
    int count = 0;

    mutex mtx;                      // Monitor mutual exclusion lock
    condition_variable not_full;   // Condition: buffer has empty space
    condition_variable not_empty;  // Condition: buffer has items

public:
    // Monitor procedure to add an item
    void produce(int item, int id)
    {
        unique_lock<mutex> lock(mtx);

        // Wait while buffer is full
        while (count == BUFFER_SIZE)
        {
            cout << "  [Producer " << id << "] Buffer is FULL! Waiting...\n";
            not_full.wait(lock);
        }

        // Add item to buffer
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        cout << "[Producer " << id << "] Produced: " << item 
             << " | Buffer Count: " << count << "/" << BUFFER_SIZE << "\n";

        // Signal waiting consumer
        not_empty.notify_one();
    }

    // Monitor procedure to remove an item
    int consume(int id)
    {
        unique_lock<mutex> lock(mtx);

        // Wait while buffer is empty
        while (count == 0)
        {
            cout << "  [Consumer " << id << "] Buffer is EMPTY! Waiting...\n";
            not_empty.wait(lock);
        }

        // Remove item from buffer
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        cout << "[Consumer " << id << "] Consumed: " << item 
             << " | Buffer Count: " << count << "/" << BUFFER_SIZE << "\n";

        // Signal waiting producer
        not_full.notify_one();
        return item;
    }
};

BoundedBufferMonitor monitor;

void producer(int id)
{
    for (int i = 1; i <= 4; i++)
    {
        int item = id * 10 + i;
        monitor.produce(item, id);
        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

void consumer(int id)
{
    for (int i = 1; i <= 4; i++)
    {
        monitor.consume(id);
        this_thread::sleep_for(chrono::milliseconds(250));
    }
}

int main()
{
    cout << "========================================================\n";
    cout << "              SIMPLE MONITOR DEMONSTRATION              \n";
    cout << "         (Producer-Consumer with Bounded Buffer)        \n";
    cout << "========================================================\n\n";

    // 2 Producers producing 4 items each = 8 items total
    // 2 Consumers consuming 4 items each = 8 items total
    thread p1(producer, 1);
    thread p2(producer, 2);
    thread c1(consumer, 1);
    thread c2(consumer, 2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    cout << "\nResult: All items produced and consumed safely with Monitor!\n";
    cout << "========================================================\n";
    return 0;
}
