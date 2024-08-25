#include "Console.h"

std::unordered_map<std::string, std::string> Meatball::Console::variables = {};

void Meatball::Console::init(void *printFuncData, const HayBCMD::PrintFunction& printFunction) {
    HayBCMD::Output::setPrintFunction(printFuncData, printFunction);
    HayBCMD::BaseCommands::init(&variables);
}

void Meatball::Console::run(const std::string& input) {
    HayBCMD::Lexer lexer = HayBCMD::Lexer(input);
    HayBCMD::Parser parser{&lexer, variables};
    parser.parse();
}