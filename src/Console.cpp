#include "Console.h"

std::unordered_map<std::string, std::string> Meatball::Console::variables = {};

void Meatball::Console::init(void *printFuncData, const SweatCI::PrintFunction& printFunction) {
    SweatCI::Output::setPrintFunction(printFuncData, printFunction);
    SweatCI::BaseCommands::init(&variables);
}

void Meatball::Console::run(const std::string& input) {
    SweatCI::Lexer lexer = SweatCI::Lexer(input);
    SweatCI::Parser parser{&lexer, variables};
    parser.parse();
}