#include "Console.h"

#include <iostream>

#ifndef MEATBALL_CONSOLE_LOG
#define MEATBALL_CONSOLE_LOG 1
#endif

std::stringstream Meatball::Console::output = {};
std::unordered_map<std::string, std::string> Meatball::Console::variables = {};

void Meatball::Console::init() {
    HayBCMD::Output::setPrintFunction(Meatball::Console::print);
    HayBCMD::BaseCommands::init(&variables);
#if MEATBALL_CONSOLE_LOG
    HayBCMD::Output::print("console initialized\n");
#endif
}

void Meatball::Console::run(const std::string& input) {
    HayBCMD::Lexer* lexer = new HayBCMD::Lexer(input);

    HayBCMD::Parser parser{ lexer, variables };
    parser.parse();

    delete lexer;
    lexer = nullptr;
}

void Meatball::Console::print(const std::string& message) {
    output << message;
    std::cout << message;
}

void Meatball::Console::printf(const std::string& format, const std::vector<HayBCMD::Data>& args) {
    print(HayBCMD::formatString(format, args));
}
