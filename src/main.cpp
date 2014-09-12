#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
#include "matrix.h"

#define SIDE 5

using namespace std;

vector<long double> jacobiAlgorithm(Matrix* a)
{
    long double threshold = pow(10.0, -9);
    vector<long double> offs;

    cout << "Starting matrix:" << endl;
    a->printMatrix();
    long double off = a->off();
    cout << "Offset: " << off << endl << endl;
    int m;

    for(m = 1; off > threshold; m++)
    {
        cout << "Step " << m << ":" << endl;

        short* pos = a->bestJacobiSubmatrixPos();
        a = jacobi(a, SIDE, pos[0], pos[1]);
        delete pos;

        a->printMatrix();
        off = a->off();
        cout << "Offset: " << off << endl << endl;
        offs.push_back(off);
    }

    cout << "Final matrix:" << endl;
    a->printMatrix();
    off = a->off();
    cout << "Offset: " << off << endl << endl;
    cout << "Took " << (m - 1) << " steps." << endl;

    return offs;
}

int main()
{
    Matrix* a = symmetric_matrix_rand(SIDE);

    try
    {
        vector<long double> offs = jacobiAlgorithm(a);
        delete a;
        cout << endl;
        cout << "Offsets tracked:" << endl;

        for(int i = 0, s = offs.size(); i < s; i++)
        {
            cout << offs[i];

            if(i < s - 1)
                cout << ", ";
        }

        cout << endl;
        cout << endl << "Press Enter to exit..." << endl;
        getchar();

        return 0;
    }
    catch(short s)
    {
        cout << "Exception " << s << " caught. Quitting..." << endl;
        return s;
    }
}
