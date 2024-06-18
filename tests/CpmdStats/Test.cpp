#include <iostream>
#include <sstream>
#include <chrono>

#include <HayBCMD.h>

#include <Console.h>
#include <Utils/MeatdataSerializer.h>

std::chrono::_V2::system_clock::time_point start;

inline void measureExecutionStart() {
    start = std::chrono::high_resolution_clock::now();
}

inline std::chrono::microseconds measureExecutionEnd() {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end-start);
}

int main(int, char**) {
    Meatball::Console::init([](const HayBCMD::OutputLevel&, const std::string& message) {
        std::cout << message;
    });

    measureExecutionStart();
    auto data = Meatball::Config::loadData("data/original.meatdata");
    auto loadOriginalDuration = measureExecutionEnd();

    measureExecutionStart();
    Meatball::serializeConfigDataMapToFile(data, "data/copy.cpmd");
    auto createCopyDuration = measureExecutionEnd();

    Meatball::Config::clearData(data);

    measureExecutionStart();
    data = Meatball::Config::loadData("data/copy.cpmd");
    auto loadCopyDuration = measureExecutionEnd();

    std::stringstream resultStream;
    resultStream << "Load \"original.meatdata\" - Time: " << loadOriginalDuration.count()
                 << "\nCreate \"copy.cpmd\" - Time: "     << createCopyDuration.count()
                 << "\nLoad \"copy.cpmd\" - Time: "       << loadCopyDuration.count();

    Meatball::Console::print(HayBCMD::ECHO, resultStream.str());

    Meatball::Config::clearData(data);
}