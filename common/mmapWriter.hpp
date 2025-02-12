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

struct DataType {
    int mInt;
    bool mBool;

    bool operator==(const DataType& other) const {
        return mInt == other.mInt && mBool == other.mBool;
    }
};

template <typename DataType>
class MMapWriter {
   public:
    MMapWriter(const std::string& filename) : mFilename{filename} {
        mFileDescriptor =
            shm_open(mFilename.c_str(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
        if (mFileDescriptor == -1)
            throw std::runtime_error("Failed to open shared memory");

        if (ftruncate(mFileDescriptor, sizeof(DataType)) == -1)
            throw std::runtime_error("Failed to set size of shared memory");

        mMappedMemory = mmap(nullptr,
                             sizeof(DataType),
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED,
                             mFileDescriptor,
                             0);
        if (mMappedMemory == MAP_FAILED)
            throw std::runtime_error("Failes to map shared memory");
    }

    ~MMapWriter() {
        if (mMappedMemory != MAP_FAILED)
            munmap(mMappedMemory, sizeof(DataType));

        if (mFileDescriptor != -1) close(mFileDescriptor);
    }

    void Write(const DataType& msg) {
        ASSERT(mMappedMemory != MAP_FAILED || mMappedMemory != nullptr);
        auto data = static_cast<DataType*>(mMappedMemory);
        std::memcpy(data, &msg, sizeof(DataType));
    }

   private:
    std::string mFilename;
    int mFileDescriptor{-1};
    void* mMappedMemory{MAP_FAILED};
};

}  // namespace Common
