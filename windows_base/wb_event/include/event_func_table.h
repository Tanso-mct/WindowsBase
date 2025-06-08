#pragma once

#include "wb_interface/event.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#include "wb_utility/include/type_cast.h"
#pragma comment(lib, "wb_utility.lib")

#include <unordered_map>

namespace wb
{
    template <typename KEY, typename EVENT, typename... ARGS>
    class EventFuncTable : public IEventFuncTable<KEY, EVENT, ARGS...>
    {
    private:
        std::unordered_map<KEY, void (EVENT::*)(ARGS...)> funcMap_;

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
        /**************************************************************************************************************/
        
        EventFuncTable() = default;
        ~EventFuncTable() override = default;

        /***************************************************************************************************************
         * IEventTableインタフェースの実装
        /**************************************************************************************************************/

        void Clear() override
        {
            funcMap_.clear();
        }

        bool Has(KEY key) const override
        {
            return funcMap_.find(key) != funcMap_.end();
        }

        size_t GetSize() const override
        {
            return funcMap_.size();
        }

        /***************************************************************************************************************
         * IEventFuncTableインタフェースの実装
        /**************************************************************************************************************/

        void Add(KEY key, void (EVENT::*func)(ARGS...)) override
        {
            if (Has(key))
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventFuncTable Add : キーが既に存在します。"}
                );
                wb::ErrorNotify("WB_EVENT_FUNC_TABLE", err);
                wb::QuitProgram();
                return;
            }

            funcMap_[key] = func;
        }

        void (EVENT::*Remove(KEY key))(ARGS...) override
        {
            if (!Has(key))
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventFuncTable Remove : キーが存在しません。"}
                );
                wb::ErrorNotify("WB_EVENT_FUNC_TABLE", err);
                wb::QuitProgram();
                return nullptr;
            }

            void (EVENT::*removedFunc)(ARGS...) = funcMap_[key];
            funcMap_.erase(key);

            return removedFunc;
        }

        void (EVENT::*Get(KEY key))(ARGS...) override
        {
            if (!Has(key))
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventFuncTable Get : キーが存在しません。"}
                );
                wb::ErrorNotify("WB_EVENT_FUNC_TABLE", err);
                wb::QuitProgram();
                return nullptr;
            }

            return funcMap_[key];
        }

        void Set(KEY key, void (EVENT::*func)(ARGS...)) override
        {
            if (!Has(key))
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventFuncTable Set : キーが存在しません。"}
                );
                wb::ErrorNotify("WB_EVENT_FUNC_TABLE", err);
                wb::QuitProgram();
                return;
            }

            funcMap_[key] = func;
        }
    };

} // namespace wb