#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/interfaces/collection.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

namespace wb
{
    class WINDOWS_BASE_API MonitorFactoryCollection : public IMonitorFactoryCollection
    {
    private:
        std::unordered_map<size_t, std::unique_ptr<IMonitorFactory>> monitorFactories_;
        size_t maxId = 0;
        std::vector<size_t> keys_;

    public:
        MonitorFactoryCollection() = default;
        virtual ~MonitorFactoryCollection() = default;

        MonitorFactoryCollection(const MonitorFactoryCollection &) = delete;
        MonitorFactoryCollection &operator=(const MonitorFactoryCollection &) = delete;

        /***************************************************************************************************************
         * IMonitorFactoryCollection implementation
        /**************************************************************************************************************/

        void AddFactory(size_t monitorID, std::unique_ptr<IMonitorFactory> monitorFactory) override;
        IMonitorFactory &GetFactory(size_t monitorID) override;

        size_t GetMaxID() const override;
        const std::vector<size_t> &GetKeys() const override;
    };

    extern WINDOWS_BASE_API MonitorFactoryCollection gMonitorFactoryCollection;

    template <typename MONITOR>
    class MonitorFactory : public IMonitorFactory
    {
    public:
        MonitorFactory() = default;
        virtual ~MonitorFactory() override = default;

        std::unique_ptr<IMonitor> Create() const override
        {
            return std::make_unique<MONITOR>();
        }
    };

    class WINDOWS_BASE_API MonitorFactoryRegistrar
    {
    public:
        MonitorFactoryRegistrar(size_t monitorID, std::unique_ptr<IMonitorFactory> monitorFactory);
    };

    #define WB_REGISTER_MONITOR_FACTORY(MONITOR, ID) \
        static wb::MonitorFactoryRegistrar monitorFactoryRegistrar##T(ID, std::make_unique<wb::MonitorFactory<MONITOR>>());


} // namespace wb