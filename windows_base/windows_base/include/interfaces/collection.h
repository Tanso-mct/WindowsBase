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
    using IAssetFactoryCollection = IFactoryCollection<IAssetFactory>;
    
    class IAssetCollection
    {
    public:
        virtual ~IAssetCollection() = default;

        virtual void Add(size_t id, size_t factoryID, size_t fileLoaderID, std::string_view filePath) = 0;
        virtual const size_t &GetFactoryID(size_t id) const = 0;
        virtual const size_t &GetFileLoaderID(size_t id) const = 0;
        virtual std::string_view GetFilePath(size_t id) const = 0;

        virtual size_t GetMaxID() const = 0;
        virtual const std::vector<size_t> &GetKeys() const = 0;
    };

    template <typename LOADER>
    class ILoaderCollection
    {
    public:
        virtual ~ILoaderCollection() = default;

        virtual void AddLoader(size_t id, std::unique_ptr<LOADER> loader) = 0;
        virtual LOADER &GetLoader(size_t id) = 0;

        virtual size_t GetMaxID() const = 0;
        virtual const std::vector<size_t> &GetKeys() const = 0;
    };

    using IFileLoaderCollection = ILoaderCollection<IFileLoader>;

    
    

} // namespace wb