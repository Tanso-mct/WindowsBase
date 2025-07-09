#pragma once

namespace wb
{
    template <typename PRODUCT>
    class IFactory
    {
    public:
        virtual ~IFactory() = default;
        virtual std::unique_ptr<PRODUCT> Create() const = 0;
    };

    class IComponent;
    using IComponentFactory = IFactory<IComponent>;

}