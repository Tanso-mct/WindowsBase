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
     * DynamicContainer
    /******************************************************************************************************************/

    template <typename T>
    class DynamicContainer : public IDynamicContainer<T>
    {
    private:
        std::vector<std::unique_ptr<T>> datas_;
        std::unique_ptr<T> empty_ = nullptr; // ダミーは参照を返すために使用される

    public:
        /***************************************************************************************************************
         * コンストラクタ / デストラクタ
         * DynamicContainerはunique_ptrを持っているので、コピーや割り当ては不可
        /**************************************************************************************************************/

        DynamicContainer() = default;
        ~DynamicContainer() override = default;

        DynamicContainer(const DynamicContainer&) = delete;
        DynamicContainer& operator=(const DynamicContainer&) = delete;

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
                    {"DynamicContainer Create : コンテナはすでに作成されています"}
                );
                wb::ErrorNotify("WB_CONTAINER", err);
                wb::QuitProgram();

                return; // DynamicContainerはすでに作成されている
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
         * IDynamicContainerの実装
        /**************************************************************************************************************/

        std::unique_ptr<T>& Get(size_t index) override
        {
            if (index >= datas_.size())
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"DynamicContainer Get : 無効なインデックスです"}
                );
                wb::ErrorNotify("WB_CONTAINER", err);
                wb::QuitProgram();

                return empty_; // 無効なインデックスの場合はダミーを返す
            }

            return datas_[index];
        }

        void Set(size_t index, std::unique_ptr<T> data) override
        {
            if (index >= datas_.size())
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"DynamicContainer Set : 無効なインデックスです"}
                );
                wb::ErrorNotify("WB_CONTAINER", err);
                wb::QuitProgram();

                return; // 無効なインデックスの場合は何もしない
            }

            datas_[index] = std::move(data);
        }

        size_t Add(std::unique_ptr<T> data) override
        {
            datas_.push_back(std::move(data));
            return datas_.size() - 1; // 追加した要素のインデックスを返す
        }

        std::unique_ptr<T> Release(size_t index) override
        {
            if (index >= datas_.size())
            {
                std::string err = wb::ConsoleLogErr
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"DynamicContainer Release : 無効なインデックスです"}
                );
                wb::ErrorNotify("WB_CONTAINER", err);
                wb::QuitProgram();

                return nullptr; // 無効なインデックスの場合はnullptrを返す
            }

            std::unique_ptr<T> releasedData = std::move(datas_[index]);
            datas_[index] = nullptr; // 解放後はnullptrに設定

            return releasedData; // 解放したデータを返す
        }
    };

    

} // namespace wb