#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/collection.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

namespace wb
{
    class WINDOWS_BASE_API SystemCollection : public ISystemCollection
    {
    private:
        std::unordered_map<size_t, std::unique_ptr<ISystemFactory>> systemFactories_;
        size_t maxId = 0;
        std::vector<size_t> keys_;

    public:
        SystemCollection() = default;
        virtual ~SystemCollection() = default;

        SystemCollection(const SystemCollection &) = delete;
        SystemCollection &operator=(const SystemCollection &) = delete;

        /***************************************************************************************************************
         * ISystemCollection implementation
        /**************************************************************************************************************/

        void AddFactory(size_t systemID, std::unique_ptr<ISystemFactory> systemFactory) override;
        ISystemFactory &GetFactory(size_t systemID) override;

        size_t GetMaxID() const override;
        const std::vector<size_t> &GetKeys() const override;
    };

    WINDOWS_BASE_API SystemCollection& GetSystemCollectionInstance();

    template <typename SYSTEM>
    class SystemFactory : public ISystemFactory
    {
    public:
        SystemFactory() = default;
        virtual ~SystemFactory() override = default;

        std::unique_ptr<ISystem> Create() const override
        {
            return std::make_unique<SYSTEM>();
        }
    };

    template <typename SYSTEM>
    class SystemRegistrar
    {
    public:
        SystemRegistrar(size_t systemID)
        {
            static bool registered = false;
            if (registered)
            {
                std::string err = CreateErrorMessage
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"System with ID ", std::to_string(systemID), " is already registered."}
                );

                ConsoleLogErr(err);
                ErrorNotify("WINDOWS_BASE", err);
                ThrowRuntimeError(err);
            }

            registered = true;

            SystemCollection &collection = GetSystemCollectionInstance();
            collection.AddFactory(systemID, std::make_unique<SystemFactory<SYSTEM>>());
        }
    };

    #define WB_REGISTER_SYSTEM(SYSTEM, ID) static wb::SystemRegistrar<SYSTEM> systemRegistrar##T(ID)

} // namespace wb