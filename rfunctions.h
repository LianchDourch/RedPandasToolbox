#ifndef RFUNCTIONS_H
#define RFUNCTIONS_H

#include "RedPandasToolbox_global.h"
#include "rmath.h"
#include "rpointers.h"
#include "robject.h"
#include "math.h"


#define R_LAMBDA_RUNNABLE(var) rpt::Runnable var; { class AnonymousRLambdaRunnable : public rpt::_Runnable { public: void run() override
#define R_END_LAMBDA_RUNNABLE(var) }; var = rpt::Runnable(new AnonymousRLambdaRunnable); }


class REDPANDASTOOLBOX_EXPORT StatLaws
{
public:
    static MathFunction<R, R> normalLaw(R mu, R sigma) { return MathFunction<double, double>([mu, sigma] (double x) {
            return ((1./sigma) * (1./std::numbers::sqrt2) * std::numbers::inv_sqrtpi) * std::exp(-.5 * std::pow((x-mu)/sigma, 2));
        });
    }
};

namespace rpt {
class REDPANDASTOOLBOX_EXPORT _Runnable : public RJavaObject
{
public:
    virtual void run() = 0;
};

class REDPANDASTOOLBOX_EXPORT _IRunnable : public _Runnable {
private:
    std::function<void()> function;
public:
    _IRunnable(std::function<void()> func) : _Runnable(), function(func) {

    }

    void run() override {
        function();
    }
};

using Runnable = JavaObject<_Runnable>;
class REDPANDASTOOLBOX_EXPORT IRunnable : public Runnable {
public:
    IRunnable() : IRunnable([] () {}) {}
    IRunnable(std::function<void()> func) : Runnable(new rpt::_IRunnable(func)) {
        bool test = (*this).instanceof<_IRunnable>();
        Util::println("test = ", test);
    }
};
class REDPANDASTOOLBOX_EXPORT _EmptyRunnable : public _Runnable {
public:
    void run() override {

    }
};

class REDPANDASTOOLBOX_EXPORT EmptyRunnable : public Runnable {
public:
    EmptyRunnable() : Runnable(new _EmptyRunnable) {}
};
}

#endif // RFUNCTIONS_H
