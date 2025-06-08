#pragma once

namespace wb
{
    class ISceneFacade
    {
    public:
        virtual ~ISceneFacade() = default;
    };

    class ISceneUpdator
    {
    public:
        virtual ~ISceneUpdator() = default;
    };
    
} // namespace wb