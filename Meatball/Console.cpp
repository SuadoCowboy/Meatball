#include "Console.h"

#include <HayBCMD.h>

#ifndef MEATBALL_CONSOLE_LOG
#define MEATBALL_CONSOLE_LOG 1
#endif

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
    output << message << "\n";
    // TODO: add the message to a output interface in the window
}
