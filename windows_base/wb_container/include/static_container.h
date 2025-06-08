#pragma once

#include "wb_interface/container.h"

#include "wb_utility/include/console_log.h"
#include "wb_utility/include/error_handler.h"
#pragma comment(lib, "wb_utility.lib")

#include <vector>
#include <memory>

namespace wb
{
    /*******************************************************************************************************************
     * StaticContainer
    /******************************************************************************************************************/

    template <typename T, typename ID>
    class StaticContainer : public IStaticContainer<T, ID>
    {
    private:
        std::vector<std::unique_ptr<T>> datas_;
        std::unique_ptr<T> empty_ = nullptr; // ダミーは参照を返すために使用される

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
         * StaticContainerはunique_ptrを持っているので、コピーや割り当ては不可
        /**************************************************************************************************************/

        StaticContainer() = default;
        ~StaticContainer() override
        {
            size_t size = datas_.size();
            for (size_t i = 0; i < size; ++i)
            {
                if (datas_[i])
                {
                    datas_[i].reset(); // unique_ptrの解放
                }
            }
        }

        StaticContainer(const StaticContainer&) = delete;
        StaticContainer& operator=(const StaticContainer&) = delete;

        /***************************************************************************************************************
         * IContainerの実装
        /**************************************************************************************************************/

        void Create(size_t size) override
        {
            if (!datas_.empty())
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"StaticContainer Create : コンテナはすでに作成されています"}
                );
                wb::ErrorNotify("WB_CONTAINER", err);
                wb::QuitProgram();

                return; // StaticContainerはすでに作成されている
            }

            datas_.resize(size);
        }

        void Clear() override
        {
            datas_.clear();
        }

        size_t GetSize() const override
        {
            return datas_.size();
        }

        /***************************************************************************************************************
         * IStaticContainerの実装
        /**************************************************************************************************************/

        std::unique_ptr<T>& Get(ID index) override
        {
            size_t indexSizeT = static_cast<size_t>(index);
            if (indexSizeT >= datas_.size())
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"StaticContainer Get : 無効なインデックスです"}
                );
                wb::ErrorNotify("WB_CONTAINER", err);
                wb::QuitProgram();

                return empty_; // 無効なインデックスの場合はダミーを返す
            }

            return datas_[indexSizeT];
        }

        void Set(ID index, std::unique_ptr<T> data) override
        {
            size_t indexSizeT = static_cast<size_t>(index);
            if (indexSizeT >= datas_.size())
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"StaticContainer Set : 無効なインデックスです"}
                );
                wb::ErrorNotify("WB_CONTAINER", err);
                wb::QuitProgram();

                return; // 無効なインデックスの場合は何もしない
            }

            datas_[indexSizeT] = std::move(data);
        }

        std::unique_ptr<T> Release(ID index) override
        {
            size_t indexSizeT = static_cast<size_t>(index);
            if (indexSizeT >= datas_.size())
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"StaticContainer Release : 無効なインデックスです"}
                );
                wb::ErrorNotify("WB_CONTAINER", err);
                wb::QuitProgram();

                return nullptr; // 無効なインデックスの場合はnullptrを返す
            }

            std::unique_ptr<T> releasedData = std::move(datas_[indexSizeT]);
            datas_[indexSizeT] = nullptr; // 解放後はnullptrに設定

            return releasedData; // 解放したデータを返す
        }
    };

} // namespace wb