#ifndef FLAGMANAGER_H
#define FLAGMANAGER_H

#include <cstdint>
#include "RedPandasToolbox_global.h"

template<typename storagetype>
struct REDPANDASTOOLBOX_EXPORT FlagsArray {
    using Type = storagetype;
private:
    storagetype flags;

    static constexpr storagetype mask(int index) {
        return storagetype(1) << index;
    }
public:
    constexpr FlagsArray(storagetype value = 0) : flags(value) {
    }

    constexpr bool get(int index) const {
        return (flags & mask(index)) != 0;
    }

    constexpr void set(int index, bool value) {
        if (value)
            flags |= mask(index);
        else
            flags &= ~mask(index);
    }

    constexpr void toggle(int index) {
        flags ^= mask(index);
    }

    constexpr storagetype& raw() {
        return flags;
    }

    constexpr storagetype raw() const {
        return flags;
    }

    template<typename OutType>
    constexpr OutType subchain(int start, int length) const {
        storagetype mask = (length >= sizeof(storagetype) * 8)
        ? storagetype(~0)
        : (storagetype(1) << length) - 1;
        return static_cast<OutType>((flags >> start) & mask);
    }

    template<typename InType>
    constexpr FlagsArray<storagetype>& setchain(int start, int length, InType chain) {
        storagetype mask = (length >= sizeof(storagetype) * 8)
        ? storagetype(~0)
        : (storagetype(1) << length) - 1;

        flags &= ~(mask << start);

        flags |= (static_cast<storagetype>(chain) & mask) << start;

        return *this;
    }
};

typedef FlagsArray<uint8_t> FlagsArray1Byte;
typedef FlagsArray<uint16_t> FlagsArray2Bytes;
typedef FlagsArray<uint32_t> FlagsArray4Bytes;
typedef FlagsArray<uint64_t> FlagsArray8Bytes;

class FlagManager
{
public:
    FlagManager();
};

#endif // FLAGMANAGER_H
