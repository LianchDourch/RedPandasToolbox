#ifndef GROUPS_H
#define GROUPS_H

#include <functional>
#include <QMap>
#include <QSet>
#include "rstringutils.h"
#include "RedPandasToolbox_global.h"

#define _(space) obj<typename std::remove_pointer_t<decltype(space)>::ObjType, space>
#define RREGINIT(method, type) virtual bool __##method() const { return false; } inline static bool __attr_##method = addDef(#method, &type::__##method);
#define RREG(method) virtual bool __##method() const override { return true; }
#define UNDEFINED(method, type) { throw NonsensActionRequested(); }; RREGINIT(method, type)
#define __rule_app const RuleApplication<ObjectType>*

namespace SpaceLaws {
inline const String GET_ZERO = "greaterOrEquals";
inline const String GET_ONE = "equals";
inline const String ADD_INTERN = "additionIntern";
inline const String MUL_INTERN = "multiplicationIntern";
inline const String MUL_EXTERN = "multiplicationExtern";
inline const String MAGNITUDE = "magnitude";
inline const String DISTANCE = "distance";
inline const String BGE = "greaterOrEquals";
inline const String EQUALS = "equals";
inline const String INVERSE = "inversion";

// Trivials
inline const String NEG = "opposite";
inline const String SUBSTRACT_INTERN = "substractIntern";
}

struct REDPANDASTOOLBOX_EXPORT NullType {
public:
    NullType(int i = 0) {

    }

    bool operator==(NullType other) {
        return true;
    }
};
template<typename ObjectType>
class Space;

class REDPANDASTOOLBOX_EXPORT NonsensActionRequested : public std::exception
{

};

template<typename ObjectType>
class REDPANDASTOOLBOX_EXPORT SpaceType {
public:
    using ObjType = ObjectType;

    Space<ObjectType>* SPACE;
};

template<typename ObjectType, Space<ObjectType>* SPACE = nullptr>
class obj;

template<typename ObjectType, typename ScalarType = NullType>
class NullGenericSpace;

template<typename ObjectType, typename ScalarType, typename TypeRestriction>
class VectorialSpace;

template<typename ObjectType>
class AbstractMathSet {
public:
    virtual bool contains(ObjectType obj) = 0;
};

template<typename ObjectType>
class StandardMathSet {
private:
    std::function<bool(ObjectType)> func;
public:
    StandardMathSet(const std::function<bool(ObjectType)>& func) : func(func) {}

    bool contains(ObjectType obj) {
        return func(obj);
    }
};

template<typename ObjectType>
class AbstractBasedStandardMathSet : public StandardMathSet<ObjectType> {
public:
    AbstractBasedStandardMathSet(AbstractMathSet<ObjectType>* set) : StandardMathSet<ObjectType>([set] (ObjectType o) { return set->contains(o); }) {

    }
};

template<typename ObjectType, Space<ObjectType>* SPACE = nullptr>
class REDPANDASTOOLBOX_EXPORT RuleApplication {
private:
    inline static QMap<String, bool (RuleApplication::*)()> definitions = {};
protected:
    mutable rpt::Array<String> KEYS = rpt::Array<String>();

    static bool addDef(String str, std::function<bool()> f) { definitions.insert(str, f); return true; }
public:
    virtual bool greaterOrEquals(ObjectType left, ObjectType right) const UNDEFINED(greaterOrEquals, RuleApplication<ObjectType>);
    virtual bool equals(ObjectType left, ObjectType right) const UNDEFINED(equals, RuleApplication<ObjectType>);
    virtual R magnitude(ObjectType o) const UNDEFINED(magnitude, RuleApplication<ObjectType>);
    virtual ObjectType additionIntern(ObjectType left, ObjectType right) const UNDEFINED(additionIntern, RuleApplication<ObjectType>);
    virtual ObjectType multiplicationIntern(ObjectType left, ObjectType right) const UNDEFINED(multiplicationIntern, RuleApplication<ObjectType>);
    virtual ObjectType inversion(ObjectType left) const UNDEFINED(inversion, RuleApplication<ObjectType>);

    virtual ObjectType zero() const UNDEFINED(zero, RuleApplication<ObjectType>);
    virtual ObjectType one() const UNDEFINED(one, RuleApplication<ObjectType>);

    // deductible
    virtual ObjectType substractIntern(ObjectType left, ObjectType right) const UNDEFINED(substractIntern, RuleApplication<ObjectType>);
    virtual ObjectType opposite(ObjectType o) const UNDEFINED(opposite, RuleApplication<ObjectType>);

    inline virtual rpt::Array<String> getKeys() const {
        if (KEYS.isEmpty()) {
            ArrayList<String> work;
            work.reserve(definitions.count()/2);
            for (String key: definitions.keys()) {
                if (definitions[key]()) work.add(key);
            }
            KEYS = work.toArray();
        }
        return KEYS;
    }
};

template<typename ObjectType, typename ScalarType, Space<ObjectType>* SPACE = nullptr>
class REDPANDASTOOLBOX_EXPORT VSRuleApplication : public RuleApplication<ObjectType, SPACE> {
    using Type = VSRuleApplication<ObjectType, ScalarType, SPACE>;
    virtual double distance(ObjectType a, ObjectType b) const UNDEFINED(distance, Type);
    virtual ObjectType multiplicationExtern(ScalarType scal) const UNDEFINED(multiplicationExtern, Type);
};

template<typename ObjectType>
class Rules {
private:
    QMap<QString, __rule_app> rules = {};
public:
    Rules(QList<__rule_app> rules) {
        for (__rule_app ra: rules) addRule(ra);
    }

    Rules& addRule(__rule_app rule) {
        for (QString k: rule->getKeys()) {
            this->rules.insert(k, rule);
        }
    }

    const RuleApplication<ObjectType>* get(QString key) const { return rules.value(key); }
};

template<typename ObjectType>
class REDPANDASTOOLBOX_EXPORT Space : public AbstractMathSet<ObjectType> {
public:
    using ObjType = ObjectType;

    inline const RuleApplication<ObjectType>* getRule(QString key) const { return rules; }
private:
    Rules<ObjectType>* rules;
};

template<typename ObjectType, Space<ObjectType>* SPACE>
class REDPANDASTOOLBOX_EXPORT obj {
    ObjectType value;

public:
    obj(ObjectType obj) : value(obj) {
    }

    ObjectType operator->() { return value; }

    obj<ObjectType, SPACE> operator+(const obj<ObjectType, SPACE> &other) {
        return obj<ObjectType, SPACE>(SPACE->getRule(SpaceLaws::ADD_INTERN)->additionIntern(value, other.value));
    }


};

/**
 * @brief The VectorialSpace class
 * TypeRestriction: Probablement le type "final" lui même, mais ça définit quels sont les types compris dans la loi de composition interne
 */
template<typename ObjectType, typename ScalarType, typename TypeRestriction = ObjectType>
class REDPANDASTOOLBOX_EXPORT VectorialSpace : public Space<ObjectType> {
public:
    //! A redéfinir
    bool contains(const ObjectType& other) const {
        return false;
    }

    ObjectType operator+(const ObjectType& other) const {
        return {};
    }

    ObjectType operator-() const {
        return {};
    }

    ObjectType operator*(ScalarType scalar) const {
        return {};
    }
    //! Fin

    ObjectType operator+=(const ObjectType& other) {
        (*this) = (*this) + other;
    }

    ObjectType operator-(const ObjectType& other) const {
        return (*this) + (-other);
    }

    ObjectType operator-=(const ObjectType& other) {
        (*this) = (*this) - other;
    }

    VectorialSpace operator*=(ScalarType scalar) {
        (*this) = (*this) * scalar;
    }


};

template<typename ObjectType, typename ScalarType>
class REDPANDASTOOLBOX_EXPORT NullGenericSpace : public VectorialSpace<ObjectType, ScalarType> {

};


#endif // GROUPS_H
