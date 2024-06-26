#include "Console.h"

std::unordered_map<std::string, std::string> Meatball::Console::variables = {};

void Meatball::Console::init(const HayBCMD::PrintFunction& printFunction) {
    HayBCMD::Output::setPrintFunction(printFunction);
    HayBCMD::BaseCommands::init(&variables);
}

void Meatball::Console::run(const std::string& input) {
    HayBCMD::Lexer lexer = HayBCMD::Lexer(input);
    HayBCMD::Parser parser{&lexer, variables};
    parser.parse();
}