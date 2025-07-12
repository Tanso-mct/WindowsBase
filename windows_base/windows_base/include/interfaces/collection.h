#pragma once

#include "windows_base/include/interfaces/factory.h"
#include "windows_base/include/interfaces/file.h"

#include <unordered_map>
#include <memory>

namespace wb
{
    template <typename FACTORY>
    class IFactoryCollection
    {
    public:
        virtual ~IFactoryCollection() = default;

        virtual void AddFactory(size_t id, std::unique_ptr<FACTORY> factory) = 0;
        virtual FACTORY &GetFactory(size_t id) = 0;

        virtual size_t GetMaxID() const = 0;
        virtual const std::vector<size_t> &GetKeys() const = 0;
    };

    using IComponentCollection = IFactoryCollection<IComponentFactory>;
    using ISystemCollection = IFactoryCollection<ISystemFactory>;

    template <typename LOADER>
    class ILoaderCollection
    {
    public:
        virtual ~ILoaderCollection() = default;

        virtual void AddLoader(size_t id, std::unique_ptr<LOADER> factory) = 0;
        virtual LOADER &GetLoader(size_t id) = 0;

        virtual size_t GetMaxID() const = 0;
        virtual const std::vector<size_t> &GetKeys() const = 0;
    };

    using IFileLoaderCollection = ILoaderCollection<IFileLoader>;

    
    

} // namespace wb