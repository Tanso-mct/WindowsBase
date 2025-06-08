#pragma once

#include "wb_interface/event.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#include "wb_utility/include/type_cast.h"
#pragma comment(lib, "wb_utility.lib")

#include <unordered_map>
#include <memory>

namespace wb
{
    template <typename KEY, typename EVENT>
    class EventInstTable : public IEventInstTable<KEY, EVENT>
    {
    private:
        std::unordered_map<KEY, std::unique_ptr<EVENT>> instMap_;
        std::unique_ptr<EVENT> empty_ = nullptr; // Dummyはunique_ptrの参照を返す際に使用

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
         * EventInstTableはunique_ptrデータを所持しているため、コピーコンストラクタと代入演算子は削除
        /**************************************************************************************************************/

        EventInstTable() = default;
        ~EventInstTable() override = default;

        EventInstTable(const EventInstTable&) = delete;
        EventInstTable& operator=(const EventInstTable&) = delete;

        /***************************************************************************************************************
         * IEventTableインタフェースの実装
        /**************************************************************************************************************/

        void Clear() override
        {
            instMap_.clear();
        }

        bool Has(KEY key) const override
        {
            return instMap_.find(key) != instMap_.end();
        }

        size_t GetSize() const override
        {
            return instMap_.size();
        }

        /***************************************************************************************************************
         * IEventInstTableインタフェースの実装
        /**************************************************************************************************************/

        void Add(KEY key, std::unique_ptr<EVENT> event) override
        {
            if (Has(key))
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventInstTable Add : キーが既に存在しています。"}
                );
                wb::ErrorNotify("WB_EVENT_INST_TABLE", err);
                wb::QuitProgram();
                return; // Key already exists
            }

            {
                wb::CastWithReturn<EVENT, IEvent> castedEvent(event);
                if (castedEvent() == nullptr)
                {
                    std::string err = wb::ConsoleLogErr
                    (
                        __FILE__, __LINE__, __FUNCTION__,
                        {"EventInstTable Add : イベントがIEventを継承していません。"}
                    );
                    wb::ErrorNotify("WB_EVENT_INST_TABLE", err);
                    wb::QuitProgram();
                    return; // Event does not inherit from IEvent
                }
            }

            instMap_[key] = std::move(event);
        }

        std::unique_ptr<EVENT> Remove(KEY key) override
        {
            if (!Has(key))
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventInstTable Remove : キーが存在しません。"}
                );
                wb::ErrorNotify("WB_EVENT_INST_TABLE", err);
                wb::QuitProgram();
                return nullptr; // Key does not exist
            }

            std::unique_ptr<EVENT> removedEvent = std::move(instMap_[key]);
            instMap_.erase(key);

            return removedEvent;
        }

        std::unique_ptr<EVENT>& Get(KEY key) override
        {
            if (!Has(key))
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventInstTable Get : キーが存在しません。"}
                );
                wb::ErrorNotify("WB_EVENT_INST_TABLE", err);
                wb::QuitProgram();
                return empty_; // Key does not exist, return dummy
            }

            return instMap_[key];
        }

        void Set(KEY key, std::unique_ptr<EVENT> event) override
        {
            if (!Has(key))
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventInstTable Set : キーが存在しません。"}
                );
                wb::ErrorNotify("WB_EVENT_INST_TABLE", err);
                wb::QuitProgram();
                return; // Key does not exist
            }

            instMap_[key] = std::move(event);
        }

    };
}