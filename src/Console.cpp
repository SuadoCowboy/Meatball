#include "Console.h"

#include <HayBCMD.h>

std::unordered_map<std::string, std::string> Meatball::Console::variables = {};

void Meatball::Console::init(std::function<void(const std::string&)> printFunction) {
    HayBCMD::Output::setPrintFunction(printFunction);
    HayBCMD::BaseCommands::init(&variables);
    print("Console initialized\n");
}

void Meatball::Console::run(const std::string& input) {
    HayBCMD::Lexer* lexer = new HayBCMD::Lexer(input);

    HayBCMD::Parser parser{ lexer, variables };
    parser.parse();

    delete lexer;
    lexer = nullptr;
}

template<typename ...Args>
void Meatball::Console::printf(const std::string &format, Args ...args) {
    HayBCMD::Output::printf(format, args...);
}