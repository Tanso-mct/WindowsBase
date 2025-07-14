#include "windows_base/src/pch.h"
#include "windows_base/include/window.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"
#include "windows_base/include/type_cast.h"

// void wb::DefaultWindowFacade::SetContext(std::unique_ptr<IContext> context)
// {
//     if (context_ != nullptr)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Context is already set for this window facade."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     context_ = wb::UniqueAs<IWindowContext>(context);
// }

// bool wb::DefaultWindowFacade::CheckIsReady() const
// {
//     if (context_ == nullptr)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Context is not set for this window facade."}
//         );

//         wb::ConsoleLogWrn(err);
//         return false;
//     }

//     isReady_ = true;
//     return true;
// }

// const HWND &wb::DefaultWindowFacade::GetHandle() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->Handle();
// }

// std::wstring_view wb::DefaultWindowFacade::GetName() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->Name();
// }

// const UINT &wb::DefaultWindowFacade::GetClientWidth() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->ClientWidth();
// }

// const UINT &wb::DefaultWindowFacade::GetClientHeight() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->ClientHeight();
// }

// bool wb::DefaultWindowFacade::IsCreated() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->IsCreated();
// }

// bool wb::DefaultWindowFacade::NeedsResize() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->NeedsResize();
// }

// bool wb::DefaultWindowFacade::IsFocusing() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->IsFocus();
// }

// bool &wb::DefaultWindowFacade::IsFocused()
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->IsFocused();
// }

// bool &wb::DefaultWindowFacade::IsUnFocused()
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->IsUnFocused();
// }

// bool wb::DefaultWindowFacade::IsMaximizing() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->IsMaximized();
// }

// bool wb::DefaultWindowFacade::IsMinimizing() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->IsMinimized();
// }

// bool wb::DefaultWindowFacade::IsFullScreen() const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return context_->IsFullScreen();
// }

// void wb::DefaultWindowFacade::AddMonitor(size_t monitorID)
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     monitorIDs_.push_back(monitorID);
//     // monitorTypeToIDMap_[monitorID] = monitorIDs_.size() - 1;
// }

// const size_t &wb::DefaultWindowFacade::GetMonitorID(size_t monitorTypeID) const
// {
//     if (!isReady_)
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Window facade is not ready."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     if (monitorTypeToIDMap_.find(monitorTypeID) == monitorTypeToIDMap_.end())
//     {
//         std::string err = wb::CreateErrorMessage
//         (
//             __FILE__, __LINE__, __FUNCTION__,
//             {"Monitor type ID ", std::to_string(monitorTypeID), " does not exist in this window facade."}
//         );

//         wb::ConsoleLogErr(err);
//         wb::ErrorNotify("WINDOWS_BASE", err);
//         wb::ThrowRuntimeError(err);
//     }

//     return monitorIDs_[monitorTypeToIDMap_.at(monitorTypeID)];
// }