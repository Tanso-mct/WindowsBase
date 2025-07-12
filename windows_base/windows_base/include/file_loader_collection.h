#pragma once
#include "windows_base/include/dll_config.h"

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

        void AddLoader(size_t id, std::unique_ptr<IFileLoader> loader) override;
        IFileLoader &GetLoader(size_t id) override;

        size_t GetMaxID() const override;
        const std::vector<size_t> &GetKeys() const override;
    };

    WINDOWS_BASE_API FileLoaderCollection& GetFileLoaderCollectionInstance();

    template <typename LOADER>
    class FileLoaderRegistrar
    {
    public:
        FileLoaderRegistrar(size_t loaderID)
        {
            static bool registered = false;
            if (registered)
            {
                std::string err = CreateErrorMessage
                (
                    __FILE__, __LINE__, __FUNCTION__,
                    {"File loader with ID ", std::to_string(loaderID), " is already registered."}
                );

                ConsoleLogErr(err);
                ErrorNotify("WINDOWS_BASE", err);
                ThrowRuntimeError(err);
            }

            registered = true;

            FileLoaderCollection &collection = GetFileLoaderCollectionInstance();
            collection.AddLoader(loaderID, std::make_unique<LOADER>());
        }

    };

    #define WB_REGISTER_FILE_LOADER(LOADER, ID) static wb::FileLoaderRegistrar<LOADER> g_fileLoaderRegistrar_##LOADER(ID);

} // namespace wb