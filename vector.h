#ifndef VECTOR_H
#define VECTOR_H
#include "types.h"
#include "array.h"
#include "RedPandasToolbox_global.h"

namespace rpt {

template<typename Scalar, rclassicsize SIZE>
class REDPANDASTOOLBOX_EXPORT Vector : public rpt::Array<Scalar>
{
private:


public:
    Vector() : Array<Scalar>(SIZE) {}
    Vector(std::initializer_list<Scalar> vals) : Vector() {
        uint i = 0;
        for (Scalar s: vals) {
            if (i >= SIZE) return;
            (*this)[i] = s;
            i += 1;
        }
    }

    void operator+=(const Vector<Scalar, SIZE>& other) {
        for (int i = 0; i < rpt::Array<Scalar>::length(); i++) {
            (*this)[i] += other.get(i);
        }
    }

    Vector<Scalar, SIZE> clone() const {
        return Vector<Scalar, SIZE>(Array<Scalar>::clone());
    }

    Vector<Scalar, SIZE> operator+(const Vector<Scalar, SIZE>& other) const {
        Vector<Scalar, SIZE> res(SIZE);
        for (int i = 0; i < SIZE; i++) {
            res[i] = this->get(i) + other.get(i);
        }
        return res;
    }

    void operator*=(Scalar scalar) {
        for (int i = 0; i < SIZE; i++) {
            (*this)[i] *= scalar;
        }
    }

    Vector<Scalar, SIZE> operator*(Scalar scalar) const {
        Vector<Scalar, SIZE> res(SIZE);
        for (int i = 0; i < SIZE; i++) {
            res[i] = this->get(i) * scalar;
        }
        return res;
    }

    void operator-=(const Vector<Scalar, SIZE>& other) {
        for (int i = 0; i < rpt::Array<Scalar>::length(); i++) {
            (*this)[i] += other.get(i);
        }
    }

    Vector<Scalar, SIZE> operator-(const Vector<Scalar, SIZE>& other) {
        Vector<Scalar, SIZE> res(SIZE);
        for (int i = 0; i < SIZE; i++) {
            res[i] = this->get(i) - other.get(i);
        }
        return res;
    }

    void operator/=(Scalar scalar) {
        for (int i = 0; i < SIZE; i++) {
            (*this)[i] /= scalar;
        }
    }

    Vector<Scalar, SIZE> operator/(Scalar scalar) const {
        Vector<Scalar, SIZE> res(SIZE);
        for (int i = 0; i < SIZE; i++) {
            res[i] = this->get(i) / scalar;
        }
        return res;
    }

    Vector<Scalar, SIZE> operator*(const Vector<Scalar, SIZE> &other) const {
        Scalar res = Scalar();
        for (int i = 0; i < SIZE; i++) res += this->get(i) * other.get(i);
        return res;
    }
};
}

#endif // VECTOR_H
