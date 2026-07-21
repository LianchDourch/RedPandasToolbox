#include "uniqueid.h"

size_t qHash(UniqueID id) {
    return qHash(id.getValue());
}
