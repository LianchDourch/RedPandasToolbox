#ifndef GEOMETRY_H
#define GEOMETRY_H

/**
 * Soit E un K-ev, on note ObjectType les éléments de E, ScalarType les éléments de K
 *
 */

#define VectorialSpace_macro(t) typename ObjectType = t, typename ScalarType = t
#define VectorialSpace_t VectorialSpace_macro(R)
#define SameVectorialSpace ObjectType, ScalarType
#define SAME_VS SameVectorialSpace

#include "array.h"
#include "groups.h"
#include "RedPandasToolbox_global.h"


template<typename ObjectType = R, typename ScalarType = R>
class REDPANDASTOOLBOX_EXPORT PUplet {
protected:
    rpt::Array<ObjectType> arr;

public:
    PUplet(rpt::Array<ObjectType> composants) : arr(composants) {}

    constexpr ObjectType getX() const { return arr[0]; }
    constexpr ObjectType getY() const { return arr[1]; }
    constexpr ObjectType getZ() const { return arr[2]; }
    constexpr rsize_t composantsCount() const { return arr.count(); }
    constexpr const ObjectType& get(int i) const { return arr.get(i); }

    PUplet& assertComposantsCount(int size) const {
        arr.assertLength(size);
        return *this;
    }
};

/**
 * @brief The HillbertianVector class
 * FIXME -> Devrait pas être comme ça, mais bon
 */
template<typename ObjectType = R, typename ScalarType = R>
class REDPANDASTOOLBOX_EXPORT HillbertianVector : public PUplet<ObjectType, ScalarType> {
    using super = PUplet<ObjectType, ScalarType>;
public:
    HillbertianVector(rpt::Array<ObjectType> composants) : super(composants) {}
    HillbertianVector(PUplet<ObjectType, ScalarType> v) : super(v.arr) {}



    ObjectType operator* (const HillbertianVector<ObjectType, ScalarType>& other) {
        assertComposantsCount(other.composantsCount());

        ObjectType res = 0;

        for (int i = 0; i < this->composantsCount(); i++) {
            res += other.get(i) * this->get(i);
        }
    }

    HillbertianVector<ObjectType, ScalarType> operator^(const HillbertianVector<ObjectType, ScalarType>& other) {
        this->assertComposantsCount(3);
        other.assertComposantsCount(3);

        rpt::Array<ObjectType> res = Array<ObjectType>(3, 0);

        for (int i = 0; i < 3; i++) {
            int j = (i + 1) % 3;
            int k = (i + 2) % 3;
            res[i] = this->get(j) * other.get(k) - this->get(k) * other.get(j);
        }

        return res;
    }
};

template<VectorialSpace_t>
class REDPANDASTOOLBOX_EXPORT Base {
private:
    rpt::Array<PUplet<ObjectType, ScalarType>> family;

public:
    Base(rpt::Array<PUplet<ObjectType, ScalarType>> family) : family(family) {}

    inline rpt::Array<PUplet<ObjectType, ScalarType>> getFamily() const { return family; }
};

template<VectorialSpace_t>
class REDPANDASTOOLBOX_EXPORT BasedVector : public PUplet<SAME_VS> {
    using super = PUplet<SAME_VS>;
private:
    Base<SAME_VS>* base;
public:
    BasedVector(rpt::Array<ObjectType> composants, Base<SAME_VS>* base) : super(composants), base(base) {

    }
};

typedef HillbertianVector<R, R> RVector;
typedef Base<R, R> RBase;
typedef BasedVector<R, R> RBasedVector;

class REDPANDASTOOLBOX_EXPORT Geometry
{
public:
    Geometry();
};

#endif // GEOMETRY_H
