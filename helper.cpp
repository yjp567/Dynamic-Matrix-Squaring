#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    srand(time(0));

    int n = 2048;
    int k = 2;
    int rowInc = 16;

    ofstream write("inp.txt");
    write << n << " " << k << " " << rowInc << endl;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            write << rand() % 10 << " ";
        write << endl;
    }
    

    return 0;
} 