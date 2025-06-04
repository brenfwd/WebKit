#pragma once

// #include "wtf/TZoneMalloc.h"

#include "bytecode/ValueProfile.h"

namespace JSC::BuiltinProfiling {

class ArrayPrototypeFilterProfiling {
// WTF_MAKE_TZONE_ALLOCATED(ArrayPrototypeFilterProfiling);
public:
    ArrayPrototypeFilterProfiling() = default;

public: // TODO
    ValueProfile p;
};

struct ProfilingRegistry {
// WTF_MAKE_TZONE_ALLOCATED(ProfilingRegistry);
    ArrayPrototypeFilterProfiling arrayPrototypeFilter;
};

} // namespace JSC::BuiltinProfiling

