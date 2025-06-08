#include "pch.h"

#include "wb_event/include/event_inst_table.h"
#include "wb_event/include/event_func_table.h"
#include "wb_event/include/event_invoker.h"

#include "wb_event_test/dummy.h"

TEST(EventInvoker, SetTablesAndInvoke)
{
    std::unique_ptr<wb::IEventInvoker<int, DummyEvent, bool, int, float>> invoker
    = std::make_unique<wb::EventInvoker<int, DummyEvent, bool, int, float>>();

    const int eventKey = 1;
    const int dummyEventId = 100;

    const bool funcKey1 = true;
    const bool funcKey2 = false;

    // テーブルを設定
    {
        std::unique_ptr<wb::IEventInstTable<int, DummyEvent>> instTable
        = std::make_unique<wb::EventInstTable<int, DummyEvent>>();

        // イベントを追加
        std::unique_ptr<DummyEvent> event = std::make_unique<DummyEvent>(dummyEventId);
        instTable->Add(eventKey, std::move(event));

        std::unique_ptr<wb::IEventFuncTable<bool, DummyEvent, int, float>> funcTable
        = std::make_unique<wb::EventFuncTable<bool, DummyEvent, int, float>>();

        // 関数を追加
        funcTable->Add(funcKey1, &DummyEvent::DummyFunction);
        funcTable->Add(funcKey2, &DummyEvent::OtherFunction);

        invoker->SetTables(std::move(instTable), std::move(funcTable));
    }

    // EventのfuncKey1に対応する関数を呼び出す
    invoker->Invoke(eventKey, funcKey1, 7, 3.14f);

    // EventのfuncKey2に対応する関数を呼び出す
    invoker->Invoke(eventKey, funcKey2, 42, 1.618f);
}

TEST(EventInvoker, InvokeWithNullptrEvent)
{
    std::unique_ptr<wb::IEventInvoker<int, DummyEvent, bool, int, float>> invoker
    = std::make_unique<wb::EventInvoker<int, DummyEvent, bool, int, float>>();

    const int eventKey = 1;
    const int dummyEventId = 100;

    const bool funcKey = true;

    // テーブルを設定
    {
        std::unique_ptr<wb::IEventInstTable<int, DummyEvent>> instTable
        = std::make_unique<wb::EventInstTable<int, DummyEvent>>();

        // イベントを追加
        std::unique_ptr<DummyEvent> event = std::make_unique<DummyEvent>(dummyEventId);
        instTable->Add(eventKey, std::move(event));

        // イベントをnullptrに設定
        instTable->Set(eventKey, nullptr);

        std::unique_ptr<wb::IEventFuncTable<bool, DummyEvent, int, float>> funcTable
        = std::make_unique<wb::EventFuncTable<bool, DummyEvent, int, float>>();

        // 関数を追加
        funcTable->Add(funcKey, &DummyEvent::DummyFunction);

        invoker->SetTables(std::move(instTable), std::move(funcTable));
    }

    // nullptrのイベントに対して関数を呼び出す
    invoker->Invoke(eventKey, funcKey, 7, 3.14f);
}

TEST(EventInvoker, InvokeWithNullptrFunction)
{
    std::unique_ptr<wb::IEventInvoker<int, DummyEvent, bool, int, float>> invoker
    = std::make_unique<wb::EventInvoker<int, DummyEvent, bool, int, float>>();

    const int eventKey = 1;
    const int dummyEventId = 100;

    const bool funcKey = true;

    // テーブルを設定
    {
        std::unique_ptr<wb::IEventInstTable<int, DummyEvent>> instTable
        = std::make_unique<wb::EventInstTable<int, DummyEvent>>();

        // イベントを追加
        std::unique_ptr<DummyEvent> event = std::make_unique<DummyEvent>(dummyEventId);
        instTable->Add(eventKey, std::move(event));

        std::unique_ptr<wb::IEventFuncTable<bool, DummyEvent, int, float>> funcTable
        = std::make_unique<wb::EventFuncTable<bool, DummyEvent, int, float>>();

        // nullptrの関数を追加
        funcTable->Add(funcKey, nullptr);

        invoker->SetTables(std::move(instTable), std::move(funcTable));
    }

    // nullptrの関数に対してイベントを呼び出す
    invoker->Invoke(eventKey, funcKey, 7, 3.14f);
}

TEST(EventInvoker, NotSetTables)
{
    std::unique_ptr<wb::IEventInvoker<int, DummyEvent, bool, int, float>> invoker
    = std::make_unique<wb::EventInvoker<int, DummyEvent, bool, int, float>>();

    const int eventKey = 1;
    const bool funcKey = true;

    // テーブルを設定せずに呼び出す
    invoker->Invoke(eventKey, funcKey, 7, 3.14f);
}