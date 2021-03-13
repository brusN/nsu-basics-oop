#include "cmdparser.h"
#include "gameview.h"
#include <algorithm>

int Parser::parseArguments(int &argc, char** argv, PlayerType &player1_type, PlayerType &player2_type, int &roundsAmount) {
    argc--;
    argv++;

    //option::Stats stats(usage, argc, argv);
    option::Option options[5];
    option::Option buffer[5];
    option::Parser parse(usage, argc, argv, options, buffer);

    if (parse.error()) {
        std::cerr << "Parsing arguments error! Check input arguments!";
        return 1;
    }

    if (options[HELP] || argc == 0) {
        std::cout << "The battleship game option presets:";
        option::printUsage(std::cout, usage);
        Gameview::printGameRules();
        return 1;
    }

    if (options[FIRST].count() == 0) {
        player1_type = PlayerType::RANDOM;
    } else {
        auto o = options[FIRST].last();
        if (o->arg == nullptr) {
            std::cerr << "Can't get first player type! Check input options!";
            return 1;
        }
        std::string word(o->arg);
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (word == "=random" || word == "random")
            player1_type = PlayerType::RANDOM;
        else if (word == "=console" || word == "console")
            player1_type = PlayerType::CONSOLE;
        else if (word == "=optimal" || word == "optimal")
            player1_type = PlayerType::OPTIMAL;
        else {
            std::cerr << "Can't get first player type! Check input options!";
            return 1;
        }
    }

    if (options[SECOND].count() == 0) {
        player2_type = PlayerType::RANDOM;
    } else {
        auto o = options[SECOND].last();
        if (o->arg == nullptr) {
            std::cerr << "Can't get second player type! Check input options!";
            return 1;
        }
        std::string word(o->arg);
        std::transform(word.begin(), word.end(),word.begin(), ::tolower);
        if (word == "=random" || word == "random")
            player2_type = PlayerType::RANDOM;
        else if (word == "=console" || word == "console")
            player2_type = PlayerType::CONSOLE;
        else if (word == "=optimal" || word == "optimal")
            player2_type = PlayerType::OPTIMAL;
        else {
            std::cerr << "Can't get second player type! Check input options!";
            return 1;
        }
    }

    if (options[COUNT].count() == 0) {
        roundsAmount = 1;
    } else {
        auto o = options[COUNT].last();
        if (o->arg == nullptr) {
            std::cerr << "Can't get game rounds amount! Check input options!";
            return 1;
        }
        std::string word(o->arg);
        if (word[0] == '=')
            word.erase(word.begin());
        if (Parser::isNumber(word)) {
            roundsAmount = atoi(word.c_str());
        } else {
            std::cerr << "Invalid game rounds amount argument! Check input options!";
            return 1;
        }
    }

    return 0;
}

bool Parser::isNumber(const std::string &s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
