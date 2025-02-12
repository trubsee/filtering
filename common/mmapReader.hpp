#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>
#include <string>

#include "common/assert.hpp"

namespace Common {

template <typename DataType>
class MMapReader {
   public:
    MMapReader(const std::string& filename) : mFilename{filename} {
        mFileDescriptor = shm_open(mFilename.c_str(), O_RDONLY, S_IRUSR);
        if (mFileDescriptor == -1)
            throw std::runtime_error("Failed to open shared memory");

        mMappedMemory = mmap(nullptr,
                             sizeof(DataType),
                             PROT_READ,
                             MAP_SHARED,
                             mFileDescriptor,
                             0);
        if (mMappedMemory == MAP_FAILED)
            throw std::runtime_error("Failes to map shared memory");
    }

    ~MMapReader() {
        if (mMappedMemory != MAP_FAILED)
            munmap(mMappedMemory, sizeof(DataType));

        if (mFileDescriptor != -1) close(mFileDescriptor);
    }

    DataType Read() const {
        auto data = static_cast<DataType*>(mMappedMemory);
        return *data;
    }

   private:
    std::string mFilename;
    int mFileDescriptor{-1};
    void* mMappedMemory{MAP_FAILED};
};

}  // namespace Common
