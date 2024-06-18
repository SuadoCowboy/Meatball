#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <HayBCMD.h>
#include <Console.h>
#include <Config.h>
#include <Utils/MeatdataSerializer.h>

int main(int argc, char* argv[]) {
    Meatball::Console::init([](const HayBCMD::OutputLevel&, const std::string& out) {
        std::cout << out;
    });

    if (argc < 2) {
        Meatball::Console::printf(HayBCMD::OutputLevel::WARNING, "Usage: {} <path_to_meatdata> <output_path>\nOr: {} <path_to_compressed_meatdata>", argv[0], argv[0]);
        return 1;
    }

    std::unordered_map<std::string, Meatball::Config::ConfigData*> dataMap;
    std::string path = argv[1];
    if (path.size() >= 4 && path.substr(path.size()-4) == "cpmd") { // convert to .meatdata
        dataMap = Meatball::deserializeConfigDataMapFromFile(argv[1]);

        if (dataMap.size() == 0) {
            Meatball::Console::print(HayBCMD::OutputLevel::ERROR, "Something went wrong or compressed meatdata is actually empty\n");
            return 1;
        }

        std::stringstream outStream;
        for (auto& data : dataMap) {
            outStream << data.first << " ";
            switch (data.second->type) {
            case Meatball::Config::ConfigType::BOOL:
                outStream << "BOOL" << " " << Meatball::Config::getConfig<bool>(data.second)->value;
                break;
            case Meatball::Config::ConfigType::DOUBLE:
                outStream << "DOUBLE" << " " << Meatball::Config::getConfig<double>(data.second)->value;
                break;
            case Meatball::Config::ConfigType::FLOAT:
                outStream << "FLOAT" << " " << Meatball::Config::getConfig<float>(data.second)->value;
                break;
            case Meatball::Config::ConfigType::STRING:
                outStream << "STRING" << " " << Meatball::Config::getConfig<std::string>(data.second)->value;
                break;
            case Meatball::Config::ConfigType::INT:
                outStream << "INT" << " " << Meatball::Config::getConfig<int>(data.second)->value;
                break;
            case Meatball::Config::ConfigType::UNSIGNED_CHAR:
                outStream << "UNSIGNED_CHAR" << " " << (int)Meatball::Config::getConfig<unsigned char>(data.second)->value;
                break;
            case Meatball::Config::ConfigType::COLOR:
                Color color = Meatball::Config::getConfig<Color>(data.second)->value;
                outStream << "COLOR" << " "
                          << (int)color.r << ","
                          << (int)color.g << ","
                          << (int)color.b << ","
                          << (int)color.a;
                break;
            }
            outStream << "\n";
        }

        std::ofstream file(argc >= 3? argv[2] : path.substr(0, path.find_last_of('.'))+".meatdata");

        file << outStream.str();

        Meatball::Config::clearData(dataMap);
        return 0;
    }

    dataMap = Meatball::Config::loadData(argv[1]);
    if (dataMap.size() == 0) {
        Meatball::Console::print(HayBCMD::OutputLevel::ERROR, "No data to compress\n");
        Meatball::Config::clearData(dataMap);
        return 1;
    }
    
    std::string argv1 = std::string(argv[1]);
    std::string outputPath = argc > 2? argv[2] : argv1.substr(0, argv1.find_last_of('.'))+".cpmd";
    if (!Meatball::serializeConfigDataMapToFile(dataMap, outputPath)) { // compress to .cpmd
        Meatball::Console::print(HayBCMD::OutputLevel::ERROR, "Could not compress object into a file\n");
        Meatball::Config::clearData(dataMap);
        return 1;
    }

    Meatball::Config::clearData(dataMap);
}