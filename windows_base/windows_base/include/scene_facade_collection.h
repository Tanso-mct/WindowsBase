#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/interfaces/scene.h"
#include "windows_base/include/interfaces/collection.h"

#include "windows_base/include/scene.h"
#include "windows_base/include/system.h"
#include "windows_base/include/entity.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

namespace wb
{
    class WINDOWS_BASE_API SceneFacadeCollection : public ISceneFacadeCollection
    {
    private:
        std::unordered_map<size_t, std::unique_ptr<ISceneFacadeFactory>> sceneFacadeFactories_;
        size_t maxId = 0;
        std::vector<size_t> keys_;

    public:
        SceneFacadeCollection() = default;
        virtual ~SceneFacadeCollection() = default;

        SceneFacadeCollection(const SceneFacadeCollection &) = delete;
        SceneFacadeCollection &operator=(const SceneFacadeCollection &) = delete;

        /***************************************************************************************************************
         * ISceneFacadeCollection implementation
        /**************************************************************************************************************/

        void AddFactory(size_t id, std::unique_ptr<ISceneFacadeFactory> factory) override;
        ISceneFacadeFactory &GetFactory(size_t id) override;

        size_t GetMaxID() const override;
        const std::vector<size_t> &GetKeys() const override;
    };

    extern WINDOWS_BASE_API SceneFacadeCollection gSceneFacadeCollection;

    class WINDOWS_BASE_API SceneFacadeRegistrar
    {
    public:
        SceneFacadeRegistrar(size_t id, std::unique_ptr<ISceneFacadeFactory> factory);
    };

} // namespace wb

#define WB_REGISTER_SCENE_FACADE(ID_FUC, ENTITIES_FACTORY, ASSET_GROUP, SYSTEM_SCHEDULER) \
    static wb::SceneFacadeRegistrar sceneFacadeRegistrar##ID_FUC \
    ( \
        ID_FUC(), \
        std::make_unique<wb::SceneFacadeFactory \
        < \
            wb::SceneContext, \
            ENTITIES_FACTORY, \
            wb::EntityIDViewFactory<wb::EntityIDView>, \
            wb::SystemsFactory, \
            ASSET_GROUP, \
            SYSTEM_SCHEDULER \
        >>() \
    );