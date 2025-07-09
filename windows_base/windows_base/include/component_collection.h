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

        // Singleton pattern
        static ComponentCollection &GetInstance();

        /***************************************************************************************************************
         * IComponentCollection implementation
        /**************************************************************************************************************/

        void AddFactory(size_t componentID, std::unique_ptr<IComponentFactory> componentFactory) override;
        IComponentFactory &GetFactory(size_t componentID) override;

        size_t GetMaxID() const override { return maxId; }
        const std::vector<size_t> &GetKeys() const override { return keys_; }
    };

    template <typename COMPONENT>
    class ComponentFactory : public IComponentFactory
    {
    public:
        ComponentFactory() = default;
        virtual ~ComponentFactory() override = default;

        std::unique_ptr<IComponent> Create() const override
        {
            return std::make_unique<COMPONENT>();
        }
    };

    template <typename COMPONENT>
    class ComponentStaticRegister
    {
    public:
        ComponentStaticRegister(size_t componentID)
        {
            ComponentCollection collection;
            collection.AddFactory(componentID, std::make_unique<ComponentFactory<COMPONENT>>());
        }
    };

} // namespace wb