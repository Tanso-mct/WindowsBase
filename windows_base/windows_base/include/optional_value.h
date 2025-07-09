#pragma once
#include "windows_base/include/dll_config.h"

#include <shared_mutex>
#include <memory>

namespace wb
{
    class WINDOWS_BASE_API OptionalValue
    {
    private:
        std::weak_ptr<size_t> value_;
        mutable std::shared_mutex mutex_;

    public:
        OptionalValue(std::shared_ptr<size_t> value);
        ~OptionalValue() = default;

        // Delete default constructor. Must create with value and isValid parameters.
        OptionalValue() = delete;

        const size_t &operator()() const;
        bool IsValid() const;

        std::unique_ptr<OptionalValue> Clone() const;
    };

} // namespace wb