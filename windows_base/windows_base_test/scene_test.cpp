#include "pch.h"

#include "windows_base/include/scene.h"
#pragma comment(lib, "windows_base.lib")

namespace
{
    class MockEntitiesFactory : public wb::IEntitiesFactory
    {
    public:
        void Create
        (
            wb::IAssetContainer &assetCont, wb::IEntityContainer &entityCont,
            wb::IComponentContainer &componentCont, wb::IEntityIDView &entityIDView
        ) const override
        {
            // Mock implementation
        }
    };
}

TEST(Scene, Create)
{
    std::unique_ptr<wb::ISceneFacade> sceneFacade = std::make_unique<wb::SceneFacade>();
    {
        std::unique_ptr<wb::ISceneContext> sceneContext = std::make_unique<wb::SceneContext>();
        sceneFacade->SetContext(std::move(sceneContext));
    }
}