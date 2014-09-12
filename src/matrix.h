#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

const short FOUR_CORNERS = -1;

const short EX_MATRIX = 10;
const short EX_BOUNDS = 20;
const short EX_EIGEN = 30;

class Matrix
{
    public:
        Matrix(short length, short width);
        Matrix(short length, short width, long double** nums);
        ~Matrix();
        short getLength();
        short getWidth();
        long double getNumAt(short xPos, short yPos);
        void setNumAt(long double num, short xPos, short yPos);
        long double off();
        Matrix* transpose();
        void printMatrix();
        short* bestJacobiSubmatrixPos();
        Matrix* jacobiSubmatrix(short xPos, short yPos);

    private:
        short length, width;
        long double** nums;
};

Matrix* multiply(Matrix* m1, Matrix* m2);
Matrix* symmetric_matrix_rand(short side);
Matrix* i_matrix(short side);
Matrix* givens_matrix(long double angle, short side, short xPos, short yPos);
long double rotation_angle(Matrix* mat);
Matrix* jacobi(Matrix* mat, short side, short xPos, short yPos);

#endif // MATRIX_H_INCLUDED
