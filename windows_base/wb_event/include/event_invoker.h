#pragma once

#include "wb_interface/event.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#include "wb_utility/include/type_cast.h"
#pragma comment(lib, "wb_utility.lib")

#include <memory>

namespace wb
{
    template <typename EVENT_KEY, typename EVENT, typename FUNC_KEY, typename... ARGS>
    class EventInvoker : public IEventInvoker<EVENT_KEY, EVENT, FUNC_KEY, ARGS...>
    {
    private:
        std::unique_ptr<IEventInstTable<EVENT_KEY, EVENT>> instTable_ = nullptr;
        std::unique_ptr<IEventFuncTable<FUNC_KEY, EVENT, ARGS...>> funcTable_ = nullptr;

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
         * EventInvokerはunique_ptrデータを所持しているため、コピーコンストラクタと代入演算子は削除
        /**************************************************************************************************************/

        EventInvoker() = default;
        ~EventInvoker() override = default;

        EventInvoker(const EventInvoker&) = delete;
        EventInvoker& operator=(const EventInvoker&) = delete;

        /***************************************************************************************************************
         * IEventInvokerインタフェースの実装
        /**************************************************************************************************************/

        void SetTables
        (
            std::unique_ptr<IEventInstTable<EVENT_KEY, EVENT>> instTable,
            std::unique_ptr<IEventFuncTable<FUNC_KEY, EVENT, ARGS...>> funcTable
        ) override
        {
            instTable_ = std::move(instTable);
            funcTable_ = std::move(funcTable);
        }

        void Invoke(EVENT_KEY eventKey, FUNC_KEY funcKey, ARGS... args) override
        {
            if (instTable_ == nullptr)
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventInvoker: インスタンステーブルが設定されていません。"}
                );
                wb::ErrorNotify("WB_EVENT_INVOKER", err);
                wb::QuitProgram();
                return;
            }

            if (funcTable_ == nullptr)
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventInvoker: 関数テーブルが設定されていません。"}
                );
                wb::ErrorNotify("WB_EVENT_INVOKER", err);
                wb::QuitProgram();
                return;
            }

            if (!instTable_->Has(eventKey)) return; // イベントが存在しない場合は何もしない
            if (!funcTable_->Has(funcKey)) return; // 関数が存在しない場合は何もしない

            std::unique_ptr<EVENT>& event = instTable_->Get(eventKey);
            if (event == nullptr)
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventInvoker: イベントがnullptrです。"}
                );
                wb::ErrorNotify("WB_EVENT_INVOKER", err);
                wb::QuitProgram();
                return;
            }

            void (EVENT::*func)(ARGS...) = funcTable_->Get(funcKey);
            if (func == nullptr)
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"EventInvoker: 関数がnullptrです。"}
                );
                wb::ErrorNotify("WB_EVENT_INVOKER", err);
                wb::QuitProgram();
                return;
            }

            // イベントの関数を呼び出す
            (event.get()->*func)(args...);

            // イベントが破棄されるかどうかを確認
            if (event->IsDestroying())
            {
                // イベントが破棄される場合は、インスタンステーブルから削除
                instTable_->Remove(eventKey);
            }
        }
    };
}
