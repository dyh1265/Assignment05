#include <core/matrix.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>
#include "assert.h"
namespace rt {
    Float4& Matrix::operator[](int idx){
        Assert(idx>=0 && idx<=3);
        return (&r1)[idx];
    }
    Float4 Matrix::operator[](int idx) const{
        Assert(idx>=0 && idx<=3);
        return (&r1)[idx];
    }
    Matrix Matrix::operator+(const Matrix& b) const{
        Matrix c;
        c.r1 = this->r1 + b.r1;
        c.r2 = this->r2 + b.r2;
        c.r3 = this->r3 + b.r3;
        c.r4 = this->r4 + b.r4;
        return c;
    }
    Matrix Matrix::operator-(const Matrix& b) const{
        Matrix c;
        c.r1 = this->r1 - b.r1;
        c.r2 = this->r2 - b.r2;
        c.r3 = this->r3 - b.r3;
        c.r4 = this->r4 - b.r4;
        return c;
    }
    Matrix Matrix::transpose() const{
    Matrix A;
       const Matrix& B = *this;
        for (int i=0; i<=3;i++){
            for (int j=0; j<=3;j++){
                A[i][j]=B[j][i];
            }
        }
        return A;
    }
    
    Matrix Matrix::invert() const {
        Matrix result;
        const Matrix& m = *this;

        //Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
        result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
        result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
        result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
        result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

        float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
        if (det == 0)
            return Matrix::zero();

        result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
        result[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
        result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
        result[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
        result[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
        result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
        result[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
        result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
        result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
        result[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
        result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
        result[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

        result = result*(1.0f/det);
        return result;
    }
    bool Matrix::operator==(const Matrix& b) const{
        if (this->r1 != b.r1) return false;
        if (this->r2 != b.r2) return false;
        if (this->r3 != b.r3) return false;
        if (this->r4 != b.r4) return false;
        return true;
    }
    bool Matrix::operator!=(const Matrix& b) const{
        if (this->r1 == b.r1) return false;
        if (this->r2 == b.r2) return false;
        if (this->r3 == b.r3) return false;
        if (this->r4 == b.r4) return false;
        return true;
    }
    
    Float4 Matrix::operator*(const Float4& b) const{
        Float4 A;
        const Matrix& B = *this;
        A.x = dot(B.r1, b);
        A.y = dot(B.r2, b);
        A.z = dot(B.r3, b);
        A.w = dot(B.r4,b);
        return A;
    }
    
    Vector Matrix::operator*(const Vector& b) const{
        Float4 a = Float4(b.x,b.y,b.z,0);
        return Vector(dot(this->r1,a), dot(this->r2,a), dot(this->r3,a));
    }
    
    Point Matrix::operator*(const Point& b) const{
        Float4 a = Float4(b.x,b.y,b.z,1);
        return Point(dot(this->r1,a), dot(this->r2,a), dot(this->r3,a));
    }

    float Matrix::det() const{
        Matrix result;
        const Matrix& m = *this;
        //Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
        result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
        result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
        result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
        result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];
        
        float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
        return det;
    }
    Matrix Matrix::zero(){
        return Matrix();
    }
    Matrix Matrix::identity(){
        return Matrix(Float4(1,0,0,0), Float4(0,1,0,0), Float4(0,0,1,0), Float4(0,0,0,1));
    }
    Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3){
        return Matrix(Float4(e1.x,e2.x,e3.x,0),
                      Float4(e1.y,e2.y,e3.y,0),
                      Float4(e1.z,e2.z,e3.z,0),
                      Float4(0,0,0,1));
    }
    
    Matrix product(const Matrix& a, const Matrix& b){
        Matrix c;
        Matrix bTrans=b.transpose();
        for (int i=0; i<=3;i++){
            for (int j=0; j<=3;j++){
                float k=c[i][j];
                c[i][j] = dot(a[i],bTrans[j]);
            }
        }
        return c;
    }
    Matrix operator*(const Matrix& a, float scalar){
        Matrix c;
        for (int i=0; i<=3;i++){
            for (int j=0; j<=3;j++){
                c[i][j] = a[i][j]*scalar;
            }
        }
        return c;
    }
    Matrix operator*(float scalar, const Matrix& a){
        Matrix c;
        for (int i=0; i<=3;i++){
            for (int j=0; j<=3;j++){
                c[i][j] = a[i][j]*scalar;
            }
        }
        return c;
    }
}


