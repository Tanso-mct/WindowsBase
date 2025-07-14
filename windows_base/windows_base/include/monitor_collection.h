#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/interfaces/collection.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

namespace wb
{
    class WINDOWS_BASE_API MonitorCollection : public IMonitorCollection
    {
    private:
        std::unordered_map<size_t, size_t> factoryIDs_;
        size_t maxId = 0;
        std::vector<size_t> keys_;

    public:
        MonitorCollection() = default;
        virtual ~MonitorCollection() = default;

        MonitorCollection(const MonitorCollection &) = delete;
        MonitorCollection &operator=(const MonitorCollection &) = delete;

        /***************************************************************************************************************
         * IMonitorCollection implementation
        /**************************************************************************************************************/

        void Add(size_t id, size_t factoryID) override;
        const size_t &GetFactoryID(size_t id) const override;

        size_t GetMaxID() const override;
        const std::vector<size_t> &GetKeys() const override;
    };

    extern WINDOWS_BASE_API MonitorCollection gMonitorCollection;

    class WINDOWS_BASE_API MonitorRegistrar
    {
    public:
        MonitorRegistrar(size_t monitorID, size_t factoryID);
    };

    #define WB_REGISTER_MONITOR(ID, FACTORY_ID) \
        static wb::MonitorRegistrar monitorRegistrar##T(ID, FACTORY_ID);

} // namespace wb