#pragma once

#include <memory>

namespace wb
{
    class IContainer
    {
    public:
        virtual ~IContainer() = default;
        
        virtual void Create(size_t size) = 0;
        virtual void Clear() = 0;
        virtual size_t GetSize() const = 0;
    };

    template <typename T, typename ID>
    class IStaticContainer : public IContainer
    {
    public:
        virtual ~IStaticContainer() = default;

        virtual std::unique_ptr<T>& Get(ID index) = 0;
        virtual void Set(ID index, std::unique_ptr<T> data) = 0;
        virtual std::unique_ptr<T> Release(ID index) = 0;
    };

    template <typename T>
    class IDynamicContainer : public IContainer
    {
    public:
        virtual ~IDynamicContainer() = default;

        virtual std::unique_ptr<T>& Get(size_t index) = 0;
        virtual void Set(size_t index, std::unique_ptr<T> data) = 0;

        virtual size_t Add(std::unique_ptr<T> data) = 0;
        virtual std::unique_ptr<T> Release(size_t index) = 0;

    };

} // namespace WB