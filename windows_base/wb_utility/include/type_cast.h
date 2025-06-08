#pragma once

#include "wb_utility/include/dll_config.h"

#include <memory>
#include <Windows.h>
#include <string>

namespace wb
{
    /*******************************************************************************************************************
     * As
     * ポインター型Sのインスタンスを指定された型Tにキャストする。
    /******************************************************************************************************************/

    template <typename T, typename S>
    T* As(S* source)
    {
        T* target = dynamic_cast<T*>(source);
        return target;
    }

    /*******************************************************************************************************************
     * UniqueAs
     * S型のunique_ptrインスタンスを指定されたunique_ptr<T>型にキャストする
     * 所有権はキャスト先へ移動する
    /******************************************************************************************************************/

    template <typename T, typename S>
    std::unique_ptr<T> UniqueAs(std::unique_ptr<S>& source)
    {
        T* target = dynamic_cast<T*>(source.get());

        if (target) return std::unique_ptr<T>(static_cast<T*>(source.release()));
        else return nullptr;
    }

    /*******************************************************************************************************************
     * CastWithReturn
     * unique_ptr型のインスタンスを指定された型にキャストする
     * スコープを抜けると、キャスト元に所有権を返す
     * これを使用しキャストしたものを、さらにキャストすることは可能だが、推奨しない
    /******************************************************************************************************************/

    template <typename S, typename D>
    class WB_UTILITY_API CastWithReturn
    {
    private:
        std::unique_ptr<S>& src_;
        std::unique_ptr<D> casted_ = nullptr;

    public:
        CastWithReturn(std::unique_ptr<S>& src) : src_(src)
        {
            D* dstPtr = dynamic_cast<D*>(src.get());
            if (dstPtr) casted_.reset(dynamic_cast<D*>(src.release()));
        }

        ~CastWithReturn()
        {
            src_.reset(dynamic_cast<S*>(casted_.release()));
        }

        std::unique_ptr<D>& operator()() 
        {
            return casted_;
        }
    };

    /*******************************************************************************************************************
     * 特定の型キャスト関数ら
    /******************************************************************************************************************/

    WB_UTILITY_API void SpecifiedAs(HRESULT src, std::string& dst);
}