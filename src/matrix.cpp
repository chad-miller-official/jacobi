#include <chrono>
#include <iostream>
#include <random>
#include "matrix.h"

using namespace std;
using namespace chrono;

Matrix::Matrix(short length, short width)
{
    this->length = length;
    this->width = width;
    nums = new long double*[length];

    for(int i = 0; i < length; i++)
        nums[i] = new long double[width];
}

Matrix::Matrix(short length, short width, long double** nums)
{
    this->width = width;
    this->length = length;
    this->nums = new long double*[length];

    for(int i = 0; i < length; i++)
    {
        this->nums[i] = new long double[width];

        for(int j = 0; j < width; j++)
            this->nums[i][j] = nums[i][j];
    }
}

Matrix::~Matrix()
{
    for(int i = 0; i < length; i++)
        delete[] nums[i];

    delete nums;
}

short Matrix::getLength()
{
    return length;
}

short Matrix::getWidth()
{
    return width;
}

long double Matrix::getNumAt(short xPos, short yPos)
{
    if(xPos < width && xPos >= 0 && yPos < length && yPos >= 0)
        return nums[yPos][xPos];
    else
        throw EX_BOUNDS;
}

void Matrix::setNumAt(long double num, short xPos, short yPos)
{
    if(xPos < width && xPos >= 0 && yPos < length && yPos >= 0)
        nums[yPos][xPos] = num;
    else
        throw EX_BOUNDS;
}

long double Matrix::off()
{
    long double toReturn = 0;

    for(int i = 0; i < length; i++)
        for(int j = 0; j < width; j++)
            if(i != j)
                toReturn += (nums[i][j] * nums[i][j]);

    return toReturn;
}

Matrix* Matrix::transpose()
{
    Matrix* m = new Matrix(width, length);

    for(int i = 0; i < width; i++)
        for(int j = 0; j < length; j++)
            m->setNumAt(nums[j][i], j, i);

    return m;
}

void Matrix::printMatrix()
{
    cout << "[";

    for(int i = 0; i < length; i++)
    {
        if(i > 0)
            cout << " ";

        cout << "[";

        for(int j = 0; j < width; j++)
        {
            cout << nums[i][j];

            if(j < width - 1)
                cout << " ";
        }

        cout << "]";

        if(i < length - 1)
            cout << endl;
    }

    cout << "]" << endl;
}

short* Matrix::bestJacobiSubmatrixPos()
{
    if(length == width && length >= 2)
    {
        short tempL = 0, tempW = 1;

        for(short i = 0; i < length - 1; i++)
            for(short j = i + 1; j < width; j++)
                if(abs(nums[i][j]) > abs(nums[tempL][tempW]))
                {
                    tempL = i;
                    tempW = j;
                }

        return new short[2]{ (short) tempL, (short) tempW };
    }
    else
        throw EX_MATRIX;
}

Matrix* Matrix::jacobiSubmatrix(short xPos, short yPos)
{
    Matrix* m = new Matrix(2, 2);

    m->setNumAt(nums[xPos][xPos], 0, 0);
    m->setNumAt(nums[yPos][xPos], 0, 1);
    m->setNumAt(nums[xPos][yPos], 1, 0);
    m->setNumAt(nums[yPos][yPos], 1, 1);

    return m;
}

Matrix* multiply(Matrix* m1, Matrix* m2)
{
    if(m1->getWidth() == m2->getLength())
    {
        Matrix* m = new Matrix(m1->getLength(), m2->getWidth());
        long double tempSum = 0;

        for(int i = 0, l = m1->getLength(); i < l; i++)
            for(int j = 0, w = m2->getWidth(); j < w; j++)
            {
                for(int t = 0, temp = m1->getWidth(); t < temp; t++)    //temp could just as easily be m2->getLength()
                    tempSum += (m1->getNumAt(t, i) * m2->getNumAt(j, t));

                m->setNumAt(tempSum, j, i);
                tempSum = 0;
            }

        return m;
    }
    else
        throw EX_MATRIX;
}

Matrix* symmetric_matrix_rand(short side)
{
    Matrix* m = new Matrix(side, side);
    minstd_rand random(system_clock::now().time_since_epoch().count());

    for(int i = 0; i < side; i++)
        for(int j = 0; j <= i; j++)
            m->setNumAt((random() % 9) + 1, j, i);

    for(int i = 1; i < side; i++)
        for(int j = i - 1; j >= 0; j--)
            m->setNumAt(m->getNumAt(j, i), i, j);

    return m;
}

Matrix* i_matrix(short side)
{
    Matrix* m = new Matrix(side, side);

    for(int i = 0; i < side; i++)
        for(int j = 0; j < side; j++)
            m->setNumAt(((i == j) ? 1 : 0), j, i);

    return m;
}

Matrix* givens_matrix(long double angle, short side, short xPos, short yPos)
{
    if(xPos >= 0 && xPos <= side && yPos >= 0 && yPos <= side)
    {
        Matrix* m = i_matrix(side);

        m->setNumAt(cos(angle), xPos, xPos);
        m->setNumAt(-sin(angle), xPos, yPos);
        m->setNumAt(sin(angle), yPos, xPos);
        m->setNumAt(cos(angle), yPos, yPos);

        return m;
    }
    else
        throw EX_BOUNDS;
}

long double rotation_angle(Matrix* mat)
{
    return (0.5 * atan(((2 * mat->getNumAt(0, 1))) / (mat->getNumAt(1, 1) - mat->getNumAt(0, 0))));
}

Matrix* jacobi(Matrix* mat, short side, short xPos, short yPos)
{
    if(xPos > (side - 2) || yPos > (side - 1))
        throw EX_BOUNDS;
    else
    {
        Matrix* b = mat->jacobiSubmatrix(xPos, yPos);
        long double angle = rotation_angle(b);
        Matrix* g = givens_matrix(angle, side, xPos, yPos);
        Matrix* gT = g->transpose();
        Matrix* j = multiply(mat, g);
        j = multiply(gT, j);

        delete b;
        delete g;
        delete gT;
        return j;
    }
}
