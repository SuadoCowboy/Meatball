#include "Serialization.h"

bool Meatball::Serialization::compressData(const std::vector<char>& data, std::vector<char>& compressedData) {
    uLongf compressedSize = compressBound(data.size());
    compressedData.resize(compressedSize);

    if (compress(reinterpret_cast<Bytef*>(compressedData.data()), &compressedSize,
                reinterpret_cast<const Bytef*>(data.data()), data.size()) != Z_OK)
        return false;

    compressedData.resize(compressedSize);  // resize to actual compressed size
    return true;
}

bool Meatball::Serialization::uncompressData(const std::vector<char>& compressedData, uLongf uncompressedSize, std::vector<char>& decompressedData) {
    decompressedData.resize(uncompressedSize);

    if (uncompress(reinterpret_cast<Bytef*>(decompressedData.data()), &uncompressedSize,
                reinterpret_cast<const Bytef*>(compressedData.data()), compressedData.size()) != Z_OK)
        return false;

    return true;
}