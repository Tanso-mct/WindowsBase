#include "windows_base/src/pch.h"
#include "windows_base/include/scene.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"
#include "windows_base/include/type_cast.h"

#include "windows_base/include/asset_collection.h"
#include "windows_base/include/asset_factory_collection.h"
#include "windows_base/include/file_loader_collection.h"

#include "windows_base/include/container_impl.h"

void wb::SceneContext::SetEntityContainer(std::unique_ptr<IEntityContainer> entityCont)
{
    entityContainer_ = std::move(entityCont);
}

wb::IEntityContainer &wb::SceneContext::GetEntityContainer()
{
    if (entityContainer_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Entity container is not set."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *entityContainer_;
}

void wb::SceneContext::SetComponentContainer(std::unique_ptr<IComponentContainer> componentCont)
{
    componentContainer_ = std::move(componentCont);
}

wb::IComponentContainer &wb::SceneContext::GetComponentContainer()
{
    if (componentContainer_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Component container is not set."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *componentContainer_;
}

void wb::SceneContext::SetSystemContainer(std::unique_ptr<ISystemContainer> systemCont)
{
    systemContainer_ = std::move(systemCont);
}

wb::ISystemContainer &wb::SceneContext::GetSystemContainer()
{
    if (systemContainer_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"System container is not set."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *systemContainer_;
}

void wb::SceneContext::SetEntityIDView(std::unique_ptr<IEntityIDView> entityIDView)
{
    entityIDView_ = std::move(entityIDView);
}

wb::IEntityIDView &wb::SceneContext::GetEntityIDView()
{
    if (entityIDView_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Entity ID view is not set."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    return *entityIDView_;
}

void wb::SceneFacade::SetContext(std::unique_ptr<IContext> context)
{
    sceneContext_ = wb::UniqueAs<ISceneContext>(context);
    if (sceneContext_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Context is not of type ISceneContext."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }
}

bool wb::SceneFacade::CheckIsReady() const
{
    if (sceneContext_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Scene facade is not ready: ",
                "Scene context is not set."
            }
        );
        wb::ConsoleLogWrn(err);
        return false;
    }

    if (entitiesFactory_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Scene facade is not ready: ",
                "Entities factory is not set."
            }
        );

        wb::ConsoleLogWrn(err);
        return false;
    }

    if (entityIDViewFactory_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Scene facade is not ready: ",
                "Entity ID view factory is not set."
            }
        );

        wb::ConsoleLogWrn(err);
        return false;
    }

    if (systemsFactory_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Scene facade is not ready: ",
                "Systems factory is not set."
            }
        );

        wb::ConsoleLogWrn(err);
        return false;
    }

    if (assetGroup_ == nullptr)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {
                "Scene facade is not ready: ",
                "Asset group is not set."
            }
        );

        wb::ConsoleLogWrn(err);
        return false;
    }

    return true;
}

void wb::SceneFacade::SetEntitiesFactory(std::unique_ptr<IEntitiesFactory> entityFactory)
{
    entitiesFactory_ = std::move(entityFactory);
}

void wb::SceneFacade::SetEntityIDViewFactory(std::unique_ptr<IEntityIDViewFactory> entityIDViewFactory)
{
    entityIDViewFactory_ = std::move(entityIDViewFactory);
}

void wb::SceneFacade::SetSystemsFactory(std::unique_ptr<ISystemsFactory> systemsFactory)
{
    systemsFactory_ = std::move(systemsFactory);
}

void wb::SceneFacade::SetAssetGroup(std::unique_ptr<IAssetGroup> assetGroup)
{
    assetGroup_ = std::move(assetGroup);
}

void wb::SceneFacade::Load(IAssetContainer &assetCont)
{
    /*******************************************************************************************************************
     * Load assets
    /******************************************************************************************************************/

    if (CheckIsReady() == false)
    {
        std::string err = wb::CreateErrorMessage
        (
            __FILE__, __LINE__, __FUNCTION__,
            {"Scene facade is not ready for loading."}
        );

        wb::ConsoleLogErr(err);
        wb::ErrorNotify("WINDOWS_BASE", err);
        wb::ThrowRuntimeError(err);
    }

    // The file datas which is already loaded
    std::unordered_map<std::string, std::unique_ptr<IFileData>> fileDatas;

    // Load file datas and create assets
    for (const size_t &assetID : assetGroup_->GetAssetIDs())
    {
        std::string_view filePath = gAssetCollection.GetFilePath(assetID);
        if (fileDatas.find(filePath.data()) == fileDatas.end())
        {
            // Get the file loader
            const size_t &fileLoaderID = gAssetCollection.GetFileLoaderID(assetID);
            IFileLoader &fileLoader = gFileLoaderCollection.GetLoader(fileLoaderID);

            // Load the file data
            std::unique_ptr<IFileData> fileData = fileLoader.Load(filePath);

            // Store the file data
            fileDatas[filePath.data()] = std::move(fileData);
        }

        // Get the asset factory
        const size_t &assetFactoryID = gAssetCollection.GetFactoryID(assetID);
        IAssetFactory &assetFactory = gAssetFactoryCollection.GetFactory(assetFactoryID);

        // Create the asset using the factory and the loaded file data
        std::unique_ptr<IAsset> asset = assetFactory.Create(*fileDatas[filePath.data()]);

        // Add the asset to the asset container
        assetCont.Set(assetID, std::move(asset));
    }

    // Clean up the file datas
    fileDatas.clear();

    /*******************************************************************************************************************
     * Create component container
    /******************************************************************************************************************/

    sceneContext_->SetComponentContainer(std::make_unique<ComponentContainer>());


    
}

wb::SceneState wb::SceneFacade::Update
(
    ContainerStorage &contStorage, const double &deltaTime, 
    const size_t &belongWindowID, size_t &nextSceneID
){
    return wb::SceneState::Size;
}

void wb::SceneFacade::Release(IAssetContainer &assetCont)
{
}