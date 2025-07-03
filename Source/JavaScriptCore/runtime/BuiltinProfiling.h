#pragma once

#include <variant>

#include "ArrayAllocationProfile.h"
#include "ValueProfile.h"
#include "ConcurrentJSLock.h"
#include <wtf/TZoneMalloc.h>

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
    ValueProfile m_callbackResValueProfile;

    // ArrayAllocationProfile m_arrayAllocProfile;
    // ValueProfile m_thisArgValueProfile;
    // ValueProfile m_callbackReturnValueProfile;

    void updateValueProfiles(const ConcurrentJSLocker& locker) override {
        m_thisValueProfile.computeUpdatedPrediction(locker);
        m_toObjectValueProfile.computeUpdatedPrediction(locker);
        m_toLengthValueProfile.computeUpdatedPrediction(locker);
        m_getByValValueProfile.computeUpdatedPrediction(locker);
        m_callbackResValueProfile.computeUpdatedPrediction(locker);
    }
};

#define JSC_BUILTIN_PROFILE_TYPE_FOREACH(macro) \
    macro(ArrayPrototypeFilterProfile)

using BuiltinProfileInvalid = struct{};
using BuiltinProfileVariant = std::variant<
    BuiltinProfileInvalid
    #define X(name) , name
    JSC_BUILTIN_PROFILE_TYPE_FOREACH(X)
    #undef X
>;

template <typename T>
concept IsBuiltinProfileType =
    false
    #define X(name) || std::is_same_v<T, name>
    JSC_BUILTIN_PROFILE_TYPE_FOREACH(X)
    #undef X
    ;

class BuiltinProfile {
WTF_MAKE_TZONE_ALLOCATED(BuiltinProfile);
private:
    BuiltinProfileVariant m_variant;

public:
    template <IsBuiltinProfileType T, typename... Args>
    inline explicit BuiltinProfile(std::in_place_type_t<T>, Args&&... args) : m_variant(std::in_place_type<T>, std::forward<Args>(args)...) {}

    template <IsBuiltinProfileType T>
    inline explicit BuiltinProfile(std::in_place_type_t<T>) : m_variant(std::in_place_type<T>) {}

    inline BuiltinProfileVariant& variant() { return m_variant; }
    inline const BuiltinProfileVariant& variant() const { return m_variant; }
};



} // namespace JSC::BuiltinProfiling

