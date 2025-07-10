#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/collection.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

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

        /***************************************************************************************************************
         * IComponentCollection implementation
        /**************************************************************************************************************/

        void AddFactory(size_t componentID, std::unique_ptr<IComponentFactory> componentFactory) override;
        IComponentFactory &GetFactory(size_t componentID) override;

        size_t GetMaxID() const override;
        const std::vector<size_t> &GetKeys() const override;
    };

    WINDOWS_BASE_API ComponentCollection& GetComponentCollectionInstance();

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
    class ComponentRegistrar
    {
    public:
        ComponentRegistrar(size_t componentID)
        {
            static bool registered = false;
            if (registered)
            {
                std::string err = CreateErrorMessage
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"Component with ID ", std::to_string(componentID), " is already registered."}
                );

                ConsoleLogErr(err);
                ErrorNotify("WINDOWS_BASE", err);
                ThrowRuntimeError(err);
            }

            registered = true;

            ComponentCollection &collection = GetComponentCollectionInstance();
            collection.AddFactory(componentID, std::make_unique<ComponentFactory<COMPONENT>>());
        }
    };

    #define WB_REGISTER_COMPONENT(COMPONENT, ID) static wb::ComponentRegistrar<COMPONENT> componentRegistrar##T(ID)

} // namespace wb