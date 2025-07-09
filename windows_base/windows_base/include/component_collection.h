#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/collection.h"

namespace wb
{
    class WINDOWS_BASE_API ComponentCollection : public IComponentCollection
    {
    private:
        std::unordered_map<size_t, std::unique_ptr<IComponentFactory>> componentFactories_;
        size_t maxId = 0;
        std::vector<size_t> keys_;

    public:
        ComponentCollection() = default;
        virtual ~ComponentCollection() = default;

        ComponentCollection(const ComponentCollection &) = delete;
        ComponentCollection &operator=(const ComponentCollection &) = delete;

        // シングルトンパターン
        static ComponentCollection &GetInstance();

        /***************************************************************************************************************
         * IComponentCollectionの実装
        /**************************************************************************************************************/

        void AddFactory(size_t componentID, std::unique_ptr<IComponentFactory> componentFactory) override;
        IComponentFactory &GetFactory(size_t componentID) override;

        size_t GetMaxId() const override { return maxId; }
        const std::vector<size_t> &GetKeys() const override { return keys_; }
    };

} // namespace wb