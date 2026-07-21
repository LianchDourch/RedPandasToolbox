#ifndef RMATH_H
#define RMATH_H

#include <functional>
#include <type_traits>
#include "RedPandasToolbox_global.h"

/**
 * Soit E un F-Espace Vectoriel:
 * ObjectType: E (type des éléments de E) = l'objet qu'on manipule sans savoir ce qu'il est
 * VectorSpaceType = ScalarType: F (type des éléments de F)
 * FamilyDimension = VectorDim = VectorType: Type (dimension) des p-uplets (ObjectType, ObjectType, ...) (Ex: _Point2D<double>) (VectorType c'est quand je m'intéresse au puplet entier)
 */

#include "groups.h"
#include "types.h"

class  REDPANDASTOOLBOX_EXPORT ObjectNotInDomain : std::exception {

};

template <typename OT>
class _Base2D;

template<typename ArgType, typename ObjectType>
concept DerivedOrSame = std::is_base_of_v<ObjectType, ArgType>;

template<typename ObjectType = double> // dans R par défaut
class REDPANDASTOOLBOX_EXPORT Ensemble { // Par défaut c'est l'ensemble vide hein (si pas hérité)
public:

    template<typename ArgType>
        requires DerivedOrSame<ArgType, ObjectType>
    bool contains(ArgType o) {
        return contains((ObjectType) o);
    }

    virtual bool contains(ObjectType o) { return false; }
};
/*
template<typename ObjectType>
class REDPANDASTOOLBOX_EXPORT Space : public Ensemble<ObjectType> {

};
*/
namespace RSpaces {
extern _Base2D<R>* R2_CANNONICAL_BASE;
};

template<typename ObjectType = double>
struct REDPANDASTOOLBOX_EXPORT _Vector2D {
    ObjectType x = ObjectType(), y = ObjectType();

    constexpr ObjectType getX() const { return x; }
    constexpr ObjectType getY() const { return y; }

    inline ObjectType operator* (const _Vector2D<ObjectType>& other) {
        return x * other.x + y * other.y;
    }

    inline _Vector2D<ObjectType> normal() {
        return {-y, x};
    }
};
/*
template<typename ObjectType, typename ScalarType>
class REDPANDASTOOLBOX_EXPORT VectorialSpace : public Space<ObjectType> {
public:
    static ObjectType get0() {
        if constexpr (std::is_same_v<ObjectType, Z> || std::is_same_v<ObjectType, R>) {
            return 0;
        } else if constexpr (std::is_same_v<ObjectType, VectorialSpace>) {
            return 0;
        } else {
            return 0;
        }
    }
};*/

//! Normes:
R magnitude(R r);

template<typename ObjectType = double>
struct REDPANDASTOOLBOX_EXPORT _Base2D {
    _Vector2D<ObjectType> family[2] = {_Vector2D<ObjectType>(), _Vector2D<ObjectType>()};

    _Base2D(_Vector2D<ObjectType> v1, _Vector2D<ObjectType> v2) {
        family[0] = v1;
        family[1] = v2;
    }
};

template<typename ObjectType = double, typename ScalarType = double>
struct REDPANDASTOOLBOX_EXPORT _BasedVector2D : public _Vector2D<ObjectType> {
    _Base2D<ObjectType>* base = VectorialSpace<ObjectType, ScalarType>::getCanonicalBase();
};

template<typename ObjectType = double, typename ScalarType = double>
class REDPANDASTOOLBOX_EXPORT _Point2D {
    _BasedVector2D<ObjectType> coords = _BasedVector2D<ObjectType, ScalarType>();
};

template<typename ObjectType = R, typename PointType = _Point2D<ObjectType>>
class REDPANDASTOOLBOX_EXPORT _Segment : public Ensemble<PointType> {
private:
    PointType start, end;

public:
    _Segment(PointType start, PointType end) : start(start), end(end) {}

    constexpr PointType getFirstPoint() const { return start; }
    constexpr PointType getSecondPoint() const { return end; }
};

typedef _Vector2D<R> R2Vector;
typedef _Base2D<R> R2Base;
typedef _BasedVector2D<> R2BasedVector;
typedef Ensemble<double> REnsemble;
typedef _Point2D<double> R2Point;
typedef _Segment<R, _Point2D<R>> R2Segment;

template<typename dom, typename img>
class REDPANDASTOOLBOX_EXPORT MathFunction {
    std::function<img(dom)> func;

    StandardMathSet<dom> restriction = StandardMathSet<dom>([] (dom d) { return true; });
public:
    MathFunction(const std::function<img(dom)> &func) : func(func) {
    }

    img operator()(dom d) {
        if (restriction.contains(d)) return func(d);
        else throw ObjectNotInDomain();
    }

    MathFunction restrain(const StandardMathSet<dom>& restriction) {
        this->restriction = restriction;
    }
};

class REDPANDASTOOLBOX_EXPORT Maths {
public:
    inline static QMap<N, R> FACTORIALS = {};
public:
    template<typename OutType = R>
    static OutType t_factorial(OutType value, bool useTable = false) {
        if (value <= 1) return 1;
        if (!useTable) {
            return value * factorial(value - 1);
        } else {
            if (FACTORIALS.contains(value)) return FACTORIALS.value(value);
            else {
                OutType res = value * factorial(value - 1, true);
                FACTORIALS.insert(value, res);
                return res;
            }
        }
    }

    inline static R factorial(R value, bool useTable = false) {
        return t_factorial<R>(value, useTable);
    }
};

class REDPANDASTOOLBOX_EXPORT Denombrement {
public:
    template<typename OutType = N>
    inline static OutType pAmongN(N p, N n, bool useTable = true) {
        if (n < p) return 0.;
        return Maths::factorial(n, useTable) / (Maths::factorial(p, useTable) * Maths::factorial(n - p, useTable));
    }
};

#endif // RMATH_H
