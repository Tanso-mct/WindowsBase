#pragma once

namespace wb
{
    class SystemArgument
    {
    public:
        SystemArgument() = default;
        virtual ~SystemArgument() = default;

        // TODO: Add any necessary data members or methods for system arguments
    };

    class ISystem
    {
    public:
        virtual ~ISystem() = default;
        virtual void Update(const SystemArgument &args) = 0;
    };

} // namespace wb