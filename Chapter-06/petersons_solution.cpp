#include <iostream>
#include <thread>

using namespace std;

int counter = 5;

// Peterson's Solution variables
bool flag[2] = {false, false};
int turn = 0;

// Enter critical section
void enterCriticalSection(int i)
{
    int j = 1 - i;

    flag[i] = true;
    turn = j;

    while (flag[j] && turn == j)
    {
        // Wait
    }
}

// Exit critical section
void exitCriticalSection(int i)
{
    flag[i] = false;
}

// Increment function
void increment(int i)
{
    for (int j = 0; j < 100000; j++)
    {
        enterCriticalSection(i);

        // Critical Section
        counter++;

        exitCriticalSection(i);
    }
}

int main()
{
    thread t1(increment, 0);
    thread t2(increment, 1);

    t1.join();
    t2.join();

    cout << "Final counter value: " << counter << endl;

    return 0;
}