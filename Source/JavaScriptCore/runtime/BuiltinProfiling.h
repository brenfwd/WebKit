#pragma once

#include <variant>

#include "bytecode/ArrayAllocationProfile.h"
#include "bytecode/ValueProfile.h"
#include "runtime/ConcurrentJSLock.h"
#include "wtf/TZoneMalloc.h"

namespace JSC::BuiltinProfiling {

class BuiltinProfileBase {
public:
    virtual ~BuiltinProfileBase() = default;

    mutable ConcurrentJSLock m_lock;

    virtual void updateValueProfiles(const ConcurrentJSLocker&) = 0;
};

class ArrayPrototypeFilterProfile : public BuiltinProfileBase {
public:
    ArrayPrototypeFilterProfile() = default;

public:
    ValueProfile m_thisValueProfile;
    ValueProfile m_toObjectValueProfile;
    ValueProfile m_toLengthValueProfile;
    ValueProfile m_getByValValueProfile;
    // ArrayAllocationProfile m_arrayAllocProfile;
    // ValueProfile m_thisArgValueProfile;
    // ValueProfile m_callbackReturnValueProfile;

    void updateValueProfiles(const ConcurrentJSLocker& locker) override {
        m_thisValueProfile.computeUpdatedPrediction(locker);
        m_toObjectValueProfile.computeUpdatedPrediction(locker);
        m_toLengthValueProfile.computeUpdatedPrediction(locker);
        m_getByValValueProfile.computeUpdatedPrediction(locker);
    }
};

#define JSC_BUILTIN_PROFILE_TYPE_FOREACH(macro) \
    macro(ArrayPrototypeFilterProfile)

using BuiltinProfileVariant = std::variant<
    std::monostate
    #define X(name) , name
    JSC_BUILTIN_PROFILE_TYPE_FOREACH(X)
    #undef X
>;

class BuiltinProfile {
WTF_MAKE_TZONE_ALLOCATED(BuiltinProfile);
private:
    BuiltinProfileVariant m_variant;

public:
    template <typename... Args>
    inline explicit BuiltinProfile(Args&&... args) : m_variant(std::forward<Args>(args)...) {}

    inline BuiltinProfileVariant& variant() { return m_variant; }
    inline const BuiltinProfileVariant& variant() const { return m_variant; }
};

template <typename T>
concept IsBuiltinProfileType =
    false
    #define X(name) || std::is_same_v<T, name>
    JSC_BUILTIN_PROFILE_TYPE_FOREACH(X)
    #undef X
    ;

} // namespace JSC::BuiltinProfiling

