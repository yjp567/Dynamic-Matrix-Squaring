#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

using namespace std;

// Matrix multiplication function for a range of rows
void matrixMultiply(const vector<vector<int>> &A, vector<vector<int>> &result, int startRow, int endRow)
{
    int N = A.size();
    for (int i = startRow; i < endRow; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            result[i][j] = 0;
            for (int k = 0; k < N; ++k)
            {
                result[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}

// Mutex type for different mutual exclusion algorithms
enum MutexType { TAS, CAS, BoundedCAS, AtomicIncrement };

// Function to perform matrix multiplication using different mutual exclusion methods
void matrixSquareParallel(const vector<vector<int>> &A, vector<vector<int>> &result, int K, int rowInc, MutexType mutexType)
{
    int N = A.size();
    int currentRow = 0;
    atomic<int> currentRowAtomic(0);
    atomic_flag flag = ATOMIC_FLAG_INIT;
    vector<thread> threads(K);

    auto incrementRow = [&]()
    {
        int start, end;
        while (true)
        {
            if (mutexType == TAS)
            {
                // TAS (Test-And-Set) Mutex
                while (flag.test_and_set(memory_order_acquire)); // Spin until lock is acquired
                start = currentRow;
                currentRow += rowInc;
                end = min(currentRow, N);
                flag.clear(memory_order_release); // Release the lock
            }
            else if (mutexType == CAS)
            {
                // CAS (Compare-And-Swap) Mutex
                int expected = currentRowAtomic.load(memory_order_relaxed);
                while (!currentRowAtomic.compare_exchange_weak(expected, expected + rowInc, memory_order_acq_rel)); // Spin until CAS succeeds
                start = expected;
                end = min(expected + rowInc, N);
            }
            else if (mutexType == BoundedCAS)
            {
                // Bounded CAS Mutex
                int expected = currentRowAtomic.load(memory_order_relaxed);
                while (expected < N && !currentRowAtomic.compare_exchange_weak(expected, expected + rowInc, memory_order_acq_rel)); // Spin until CAS succeeds or reaches end of rows
                start = expected;
                end = min(expected + rowInc, N);
            }
            else if (mutexType == AtomicIncrement)
            {
                // C++ Atomic Increment Mutex
                start = currentRowAtomic.fetch_add(rowInc, memory_order_relaxed);
                end = min(start + rowInc, N);
            }

            if (start >= N)
                break;

            matrixMultiply(A, result, start, end);
        }
    };

    for (int i = 0; i < K; ++i)
    {
        threads[i] = thread(incrementRow);
    }

    for (auto &th : threads)
    {
        th.join();
    }
}

int main()
{
    // Read input from file
    ifstream infile("inp.txt");
    int N, K, rowInc;
    infile >> N >> K >> rowInc;

    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> result(N, vector<int>(N));

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            infile >> A[i][j];
        }
    }
    infile.close();

    // Perform matrix multiplication with different mutual exclusion methods
    vector<string> mutexNames = {"TAS", "CAS", "Bounded CAS", "Atomic Increment"};
    vector<MutexType> mutexTypes = {TAS, CAS, BoundedCAS, AtomicIncrement};

    ofstream outfile("out.txt");
    for (int i = 0; i < mutexNames.size(); ++i)
    {
        auto start = chrono::high_resolution_clock::now();
        matrixSquareParallel(A, result, K, rowInc, mutexTypes[i]);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

        outfile << "Using " << mutexNames[i] << ":\n";
        outfile << "Time taken: " << 1.0*duration/1000000 << " seconds\n";
        outfile << "Resulting matrix:\n";
        for (const auto &row : result)
        {
            for (int elem : row)
            {
                outfile << elem << " ";
            }
            outfile << "\n";
        }
        outfile << "\n";
    }
    outfile.close();

    return 0;
}