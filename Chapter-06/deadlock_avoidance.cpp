#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const int P = 5; // Number of processes: P0, P1, P2, P3, P4
const int R = 3; // Number of resource types: A, B, C

// Standard textbook dataset (Operating System Concepts - Silberschatz)
int allocation[P][R] = {
    {0, 1, 0}, // P0
    {2, 0, 0}, // P1
    {3, 0, 2}, // P2
    {2, 1, 1}, // P3
    {0, 0, 2}  // P4
};

int maxClaim[P][R] = {
    {7, 5, 3}, // P0
    {3, 2, 2}, // P1
    {9, 0, 2}, // P2
    {2, 2, 2}, // P3
    {4, 3, 3}  // P4
};

int available[R] = {3, 3, 2}; // Available instances of A, B, C
int need[P][R];

// Step 1: Calculate Need Matrix = Max - Allocation
void calculateNeed()
{
    for (int i = 0; i < P; i++)
    {
        for (int j = 0; j < R; j++)
        {
            need[i][j] = maxClaim[i][j] - allocation[i][j];
        }
    }
}

// Display the matrices clearly
void displayState()
{
    cout << "Process | Allocation |   Max   |  Need   | Available\n";
    cout << "--------+------------+---------+---------+----------\n";
    for (int i = 0; i < P; i++)
    {
        cout << "   P" << i << "   |   ";
        for (int j = 0; j < R; j++) cout << allocation[i][j] << " ";
        cout << "  |  ";
        for (int j = 0; j < R; j++) cout << maxClaim[i][j] << " ";
        cout << " |  ";
        for (int j = 0; j < R; j++) cout << need[i][j] << " ";
        cout << " | ";
        if (i == 0)
        {
            for (int j = 0; j < R; j++) cout << available[j] << " ";
        }
        cout << "\n";
    }
    cout << "----------------------------------------------------\n\n";
}

// Step 2: Banker's Safety Algorithm to find Safe Sequence
bool checkSafety()
{
    vector<int> work(available, available + R);
    vector<bool> finish(P, false);
    vector<int> safeSeq;

    int count = 0;
    while (count < P)
    {
        bool found = false;
        for (int p = 0; p < P; p++)
        {
            if (!finish[p])
            {
                // Check if Need[p] <= Work
                bool canAllocate = true;
                for (int j = 0; j < R; j++)
                {
                    if (need[p][j] > work[j])
                    {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate)
                {
                    // Allocate resources, simulate execution and release
                    for (int j = 0; j < R; j++)
                    {
                        work[j] += allocation[p][j];
                    }
                    safeSeq.push_back(p);
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }

        // If no process could proceed in this pass, system is unsafe
        if (!found)
        {
            cout << ">>> RESULT: UNSAFE STATE! Deadlock may occur. <<<\n";
            return false;
        }
    }

    // If all processes finished, system is in a safe state
    cout << ">>> RESULT: SAFE STATE! Deadlock is avoided. <<<\n";
    cout << ">>> Safe Sequence: < ";
    for (size_t i = 0; i < safeSeq.size(); i++)
    {
        cout << "P" << safeSeq[i] << (i + 1 < safeSeq.size() ? " -> " : " ");
    }
    cout << ">\n";
    return true;
}

int main()
{
    cout << "====================================================\n";
    cout << "  DEADLOCK AVOIDANCE: BANKER'S SAFETY ALGORITHM    \n";
    cout << "====================================================\n\n";

    calculateNeed();
    displayState();
    checkSafety();

    cout << "====================================================\n";
    return 0;
}
