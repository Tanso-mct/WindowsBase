#pragma once
#include "windows_base/include/dll_config.h"

#include "windows_base/include/interfaces/file.h"
#include "windows_base/include/interfaces/collection.h"

#include "windows_base/include/console_log.h"
#include "windows_base/include/error_handler.h"

namespace wb
{
    class WINDOWS_BASE_API FileLoaderCollection : public IFileLoaderCollection
    {
    private:
        std::unordered_map<size_t, std::unique_ptr<IFileLoader>> fileLoaders_;
        size_t maxId = 0;
        std::vector<size_t> keys_;

    public:
        FileLoaderCollection() = default;
        virtual ~FileLoaderCollection() = default;

        FileLoaderCollection(const FileLoaderCollection &) = delete;
        FileLoaderCollection &operator=(const FileLoaderCollection &) = delete;

        /***************************************************************************************************************
         * IFileLoaderCollection implementation
        /**************************************************************************************************************/

        void AddLoader(size_t type, std::unique_ptr<IFileLoader> loader) override;
        IFileLoader &GetLoader(size_t type) override;

        size_t GetMaxID() const override;
        const std::vector<size_t> &GetKeys() const override;
    };

    extern WINDOWS_BASE_API FileLoaderCollection gFileLoaderCollection;

    class WINDOWS_BASE_API FileLoaderRegistrar
    {
    public:
        FileLoaderRegistrar(size_t id, std::unique_ptr<IFileLoader> loader);
    };

    #define WB_REGISTER_FILE_LOADER(ID, LOADER) \
        static wb::FileLoaderRegistrar gFileLoaderRegistrar##LOADER(ID, std::make_unique<LOADER>());

} // namespace wb