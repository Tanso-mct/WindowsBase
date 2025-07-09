#pragma once

#include "windows_base/include/factory.h"

#include <unordered_map>
#include <memory>

namespace wb
{
    template <typename FACTORY>
    class ICollection
    {
    public:
        ICollection() = default;
        virtual ~ICollection() = default;

        virtual void AddFactory(size_t id, std::unique_ptr<FACTORY> factory) = 0;
        virtual FACTORY &GetFactory(size_t id) = 0;

        virtual size_t GetMaxID() const = 0;
        virtual const std::vector<size_t> &GetKeys() const = 0;
    };

    using IComponentCollection = ICollection<IComponentFactory>;
    using ISystemCollection = ICollection<ISystemFactory>;

} // namespace wb