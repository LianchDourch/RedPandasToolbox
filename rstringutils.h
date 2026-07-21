#ifndef RSTRINGUTILS_H
#define RSTRINGUTILS_H

#include "arraylist.h"
#include <QChar>
#include <QString>
#include "RedPandasToolbox_global.h"

typedef QString String;

class REDPANDASTOOLBOX_EXPORT StringBuilder : ArrayList<QChar>
{
public:

    StringBuilder(rsize size = 0) : ArrayList<QChar>(size, ' ') {
    }

    StringBuilder(const char* c_str) : StringBuilder() {
        append(c_str);
    }

    StringBuilder& append(const char* c_str) {
        String work = c_str;

        for (QChar c: work) {
            add(c);
        }

        return *this;
    }

    String toString() const {
        String res(length(), ' ');
        for (int i = 0; i < length(); i++) res[i] = get(i);
        return res;
    }
};

#endif // RSTRINGUTILS_H
