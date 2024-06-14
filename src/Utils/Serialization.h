#pragma once

#include <fstream>
#include <vector>
#include <cstring>  // For std::memcpy
#include <type_traits>  // For type traits

#include <zlib.h>

namespace Meatball { namespace Serialization {
    template<typename T>
    std::vector<char> serializeObject(const T& object) {
        std::vector<char> buffer(sizeof(T));
        std::memcpy(buffer.data(), &object, sizeof(T));
        return buffer;
    }

    template<typename T>
    T deserializeVector(const std::vector<char>& serializedVector) {
        T object;
        std::memcpy(&object, serializedVector.data(), sizeof(T));
        return object;
    }

    bool compressData(const std::vector<char>& data, std::vector<char>& compressedData);
    bool uncompressData(const std::vector<char>& compressedData, uLongf uncompressedSize, std::vector<char>& decompressedData);

    /// @brief serializes, compressed and writes the object into a binary file
    /// @tparam T copiable type
    /// @param object object to be serialized
    /// @return false if something went wrong
    template<typename T>
    bool compressObjectToFile(const T& object, const std::string& filename) {
        std::vector<char> serializedData = serializeObject(object);

        // Compress the serialized data
        std::vector<char> compressedData;
        if (!compressData(serializedData, compressedData)) {
            return false;
        }

        // Write the compressed data to a file
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs) return false;

        // Write the compressed data size
        uLongf compressedSize = compressedData.size();
        ofs.write(reinterpret_cast<const char*>(&compressedSize), sizeof(compressedSize));
        // Write the original data size
        uLongf originalSize = serializedData.size();
        ofs.write(reinterpret_cast<const char*>(&originalSize), sizeof(originalSize));
        // Write the compressed data
        ofs.write(compressedData.data(), compressedSize);

        ofs.close();
        return true;
    }

    /// @brief read the file, uncompresses, deserialize, and returns the data
    /// @tparam T copiable type
    /// @param output returned data
    /// @return 
    template<typename T>
    bool uncompressObjectToFile(const std::string& filename, T& output) {
        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs) {
            throw std::runtime_error("Failed to open file for reading");
        }

        // Read the compressed data size
        uLongf compressedSize;
        ifs.read(reinterpret_cast<char*>(&compressedSize), sizeof(compressedSize));
        // Read the original data size
        uLongf originalSize;
        ifs.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize));

        // Read the compressed data
        std::vector<char> compressedBuffer(compressedSize);
        ifs.read(compressedBuffer.data(), compressedSize);

        // Decompress the data
        std::vector<char> decompressedBuffer;
        if (!uncompressData(compressedBuffer, originalSize, decompressedBuffer))
            return false;

        // Deserialize the object
        T object = deserializeVector<T>(decompressedBuffer);

        ifs.close();
        return object;
    }
}}