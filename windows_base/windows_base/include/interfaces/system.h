#pragma once

namespace wb
{
    class SystemArgument;

    class ISystem
    {
    public:
        virtual ~ISystem() = default;
        virtual size_t GetID() const = 0;
        virtual void Update(const SystemArgument &args) = 0;
    };

} // namespace wb