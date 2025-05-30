#pragma once

namespace JSC::BuiltinProfiling {

class ArrayPrototypeFilterProfiling {
private:
public:
    ArrayPrototypeFilterProfiling() = default;
};

struct ProfilingRegistry {
    ArrayPrototypeFilterProfiling arrayPrototypeFilter;
};

} // namespace JSC::BuiltinProfiling

