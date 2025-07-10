#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/id_factory.h"
#include "windows_base/include/optional_value.h"
#include "windows_base/include/container.h"
#include "windows_base/include/factory.h"

#include <vector>

namespace wb
{
    class IEntity
    {
    public:
        virtual ~IEntity() = default;

        virtual void Destroy(IComponentContainer &componentCont) = 0;

        virtual void SetID(std::unique_ptr<OptionalValue> id) = 0;
        virtual const OptionalValue &GetID() const = 0;

        virtual void AddComponent(size_t componentID, IComponentContainer &componentCont) = 0;
        virtual IComponent *GetComponent(size_t componentID, IComponentContainer &componentCont) = 0;

        virtual const std::vector<size_t> &GetComponentIDs() const = 0;
    };

    class WINDOWS_BASE_API Entity : public IEntity
    {
    private:
        std::unique_ptr<OptionalValue> id_;

        std::unique_ptr<IStaticContainer<OptionalValue>> componentIndicesInCont_;
        std::vector<size_t> componentIDs_;

    public:
        Entity();
        ~Entity() override = default;

        /***************************************************************************************************************
         * IEntity implementation
        /**************************************************************************************************************/

        void Destroy(IComponentContainer &componentCont) override;

        void SetID(std::unique_ptr<OptionalValue> id) override;
        const OptionalValue &GetID() const override;

        void AddComponent(size_t componentID, IComponentContainer &componentCont) override;
        IComponent *GetComponent(size_t componentID, IComponentContainer &componentCont) override;

        const std::vector<size_t> &GetComponentIDs() const override { return componentIDs_; }
    };

    class WINDOWS_BASE_API EntityIDView
    {
    private:
        // Container for entity IDs, indexed by component ID.
        // Outside vector's index is the component ID, and inside vector contains entity IDs.
        std::vector<std::vector<std::unique_ptr<OptionalValue>>> entityIDsPerComponent_;
        
    public:
        EntityIDView();
        ~EntityIDView() = default;

        EntityIDView(const EntityIDView &) = delete;
        EntityIDView &operator=(const EntityIDView &) = delete;

        void RegisterEntity(IEntity &entity);
        void UnregisterEntity(IEntity &entity);

        const std::vector<std::unique_ptr<OptionalValue>> &operator()(size_t componentID) const;
    };

    WINDOWS_BASE_API IEntity& CreateEntity(IEntityContainer &entityCont, EntityIDView &entityIDView);

    WINDOWS_BASE_API void DestroyEntity
    (
        IEntity *entity, 
        EntityIDView &entityIDView, IEntityContainer &entityCont, IComponentContainer &componentCont
    );

} // namespace wb