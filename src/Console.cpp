#include "Console.h"

#include <iostream>

#include <HayBCMD.h>

#ifndef MEATBALL_CONSOLE_LOG
#define MEATBALL_CONSOLE_LOG
#endif

std::stringstream Meatball::Console::output = {};
std::unordered_map<std::string, std::string> Meatball::Console::variables = {};

void Meatball::Console::init() {
    HayBCMD::Output::setPrintFunction(Meatball::Console::addToOutput);
    HayBCMD::BaseCommands::init(&variables);
#ifdef MEATBALL_CONSOLE_LOG
    HayBCMD::Output::print("initialized\n");
#endif
}

void Meatball::Console::run(const std::string& input) {
    HayBCMD::Lexer* lexer = new HayBCMD::Lexer(input);

    HayBCMD::Parser parser{ lexer, variables };
    parser.parse();

    delete lexer;
    lexer = nullptr;
}

void Meatball::Console::addToOutput(const std::string& message) {
    output << message;
    std::cout << message; // TODO: remove me! --> THIS IS TEMPORARY
}