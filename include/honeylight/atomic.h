/**
 * atomic.h
 *
 * @author Shame Boi
 */

#pragma once

#define MEMORY_ORDER __ATOMIC_SEQ_CST

#define ATOMIC_LOAD_N(ptr) __atomic_load_n((ptr), MEMORY_ORDER)

#define ATOMIC_LOAD(ptr, retValPtr) __atomic_load((ptr), (retValPtr), MEMORY_ORDER)

#define ATOMIC_STORE_N(ptr, val) __atomic_store_n((ptr), (val), MEMORY_ORDER)

#define ATOMIC_STORE(ptr, valPtr) __atomic_store((ptr), (valPtr), MEMORY_ORDER)

#define ATOMIC_EXCHANGE(ptr, valPtr) __atomic_exchange((ptr), (valPtr), MEMORY_ORDER)

#define ATOMIC_EXCHANGE_N(ptr, val) __atomic_exchange_n((ptr), (val), MEMORY_ORDER)

#define ATOMIC_COMPARE_EXCHANGE_N(ptr, expectedPtr, desiredValue) __atomic_compare_exchange_n((ptr), (expectedPtr), (desiredValue), true, MEMORY_ORDER, MEMORY_ORDER)

#define ATOMIC_ADD_FETCH(ptr, val) __atomic_add_fetch((ptr), (val), MEMORY_ORDER)

#define ATOMIC_CLEAR(ptrToBool) __atomic_clear((ptrToBool), MEMORY_ORDER)

#define ATOMIC_TEST_AND_SET(ptrToBool) __atomic_test_and_set((ptrToBool), MEMORY_ORDER)

struct atomic_flag_t {
private:
    volatile bool value;

public:
    explicit atomic_flag_t(bool const initialValue) noexcept : value(initialValue) {}

    atomic_flag_t() noexcept : value(false) {}

    inline bool getFlag() const volatile {
        return value;
    }

    inline bool setFlag() volatile {
        return ATOMIC_TEST_AND_SET(&value);
    }

    inline void clearFlag() volatile {
        ATOMIC_CLEAR(&value);
    }

    inline operator bool() const volatile {
        return getFlag();
    }
};

template<typename EnumT, EnumT DefaultValue>
struct atomic_enum_t {
private:
    EnumT value = DefaultValue;

public:
    inline EnumT get() const {
        return ATOMIC_LOAD_N(&value);
    }

    inline void set(EnumT const newValue) {
        return ATOMIC_STORE_N(&value, newValue);
    }

    inline bool tryAndSet(EnumT expectedValue, EnumT const newValue) {
        return ATOMIC_COMPARE_EXCHANGE_N(&value, &expectedValue, newValue);
    }

    inline void reset() {
        ATOMIC_STORE_N(&value, DefaultValue);
    }

    inline operator EnumT() const {
        return get();
    }
};
