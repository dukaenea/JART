#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>

namespace rt {

Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4) {
    this->r[0] = r1;
    this->r[1] = r2;
    this->r[2] = r3;
    this->r[3] = r4;
}

Float4& Matrix::operator[](int idx) {
    return r[idx];
}

Float4 Matrix::operator[](int idx) const {
     return r[idx];
}

Matrix Matrix::operator+(const Matrix& b) const {
    return Matrix(
        this->r[0] + b[0],
        this->r[1] + b[1],
        this->r[2] + b[2],
        this->r[3] + b[3]
    );
}

Matrix Matrix::operator-(const Matrix& b) const {
    return Matrix(
        this->r[0] - b[0],
        this->r[1] - b[1],
        this->r[2] - b[2],
        this->r[3] - b[3]
    );
}

Matrix Matrix::transpose() const {
    Matrix toReturn = Matrix::zero();


    for(int i = 0; i<4 ;i++){
        for(int j = 0; j<4;j++){
            toReturn[j][i] = this->r[i][j];
        }
    }

    return toReturn;
}

Matrix Matrix::invert() const {
        Matrix result = Matrix::zero();
        const Matrix &m = *this;

        //Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
        result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] +
                       m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
        result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] -
                       m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
        result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] +
                       m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
        result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] -
                       m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

        float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
        if (det == 0)
            return Matrix::zero();

        result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] -
                       m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
        result[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] +
                       m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
        result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] -
                       m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
        result[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] +
                       m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
        result[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] +
                       m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
        result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] -
                       m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
        result[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] +
                       m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
        result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] -
                       m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
        result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] -
                       m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
        result[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] +
                       m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
        result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] -
                       m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
        result[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] +
                       m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

        result = result * (1.0f / det);
        return result;
    }
bool Matrix::operator==(const Matrix& b) const {
    return this->r[0] == b[0] && this->r[1] == b[1] && this->r[2] == b[2] && this->r[3] == b[3];
}

bool Matrix::operator!=(const Matrix& b) const {
    return this->r[0] != b[0] || this->r[1] != b[1] || this->r[2] != b[2] || this->r[3] != b[3];
}

Matrix product(const Matrix& a, const Matrix& b) {
    Matrix pm = Matrix::zero();

    for(int i=0;i<4;i++){

        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                pm[i][j]+=a[i][k]*b[k][j];
            }
        }
   }

    return pm;
}

Matrix operator*(const Matrix& a, float scalar) {
    return Matrix(
        a[0] * scalar,
        a[1] * scalar,
        a[2] * scalar,
        a[3] * scalar
    );
}

Matrix operator*(float scalar, const Matrix& a) {
    return Matrix(
        a[0] * scalar,
        a[1] * scalar,
        a[2] * scalar,
        a[3] * scalar
    );
}

Float4 Matrix::operator*(const Float4& b) const {
    
    Float4 result=Float4::rep(0);

    for(int i=0;i<4;i++){
        
        float sum=0.0f;

        for(int k=0;k<4;k++){
            sum+= r[i][k]* b[k];
        }
                
        result[i]=sum;
        
   }

    return result;
}

Vector Matrix::operator*(const Vector& b) const {
    return Vector((*this) * Float4(b));
}

Point Matrix::operator*(const Point& b) const {
    return Point((*this) * Float4(b));
}

void Matrix::print(){
    this->r[0].print();
    this->r[1].print();
    this->r[2].print();
    this->r[3].print();
}


float Matrix::det() const {
    Matrix result;
    const Matrix& m = *this;

    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    return m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
}





Matrix Matrix::zero() {
    return Matrix(
        Float4::rep(0.0f),
        Float4::rep(0.0f),
        Float4::rep(0.0f),
        Float4::rep(0.0f)
    );
}

Matrix Matrix::identity() {
    return Matrix(
        Float4(1.0f, 0.0f, 0.0f, 0.0f),
        Float4(0.0f, 1.0f, 0.0f, 0.0f),
        Float4(0.0f, 0.0f, 1.0f, 0.0f),
        Float4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
    return Matrix(
        Float4(e1.x, e2.x, e3.x, 0.0f),
        Float4(e1.y, e2.y, e3.y, 0.0f),
        Float4(e1.z, e2.z, e3.z, 0.0f),
        Float4(0.0f, 0.0f, 0.0f, 1.0f)
    );
}

}