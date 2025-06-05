#pragma once

// #include "wtf/TZoneMalloc.h"

#include <variant>

#include "bytecode/ValueProfile.h"

namespace JSC::BuiltinProfiling {

class ArrayPrototypeFilterProfile {
public:
    ArrayPrototypeFilterProfile() = default;

public:
    ValueProfile p;
};

using BuiltinProfile = std::variant<
    ArrayPrototypeFilterProfile
>;

} // namespace JSC::BuiltinProfiling

