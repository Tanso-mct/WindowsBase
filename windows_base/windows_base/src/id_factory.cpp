#include "windows_base/src/pch.h"
#include "windows_base/include/id_factory.h"

size_t wb::IDFactory::CreateComponentID()
{
    static size_t idCounter = 0;

    size_t createdId = idCounter;
    idCounter++;

    return createdId;
}

size_t wb::IDFactory::CreateSystemID()
{
    static size_t idCounter = 0;

    size_t createdId = idCounter;
    idCounter++;

    return createdId;
}

size_t wb::IDFactory::CreateFileLoaderID()
{
    static size_t idCounter = 0;

    size_t createdId = idCounter;
    idCounter++;

    return createdId;
}