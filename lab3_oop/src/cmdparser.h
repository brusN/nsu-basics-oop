#ifndef NEWLAB3_CMDPARSER_H
#define NEWLAB3_CMDPARSER_H

#include "optionparser.h"
#include "player.h"
#include <iostream>

enum class PlayerType {
    CONSOLE, RANDOM, OPTIMAL
};

enum optionIndex { UNKNOWN, HELP, COUNT, FIRST, SECOND };
const option::Descriptor usage[] = {
        {UNKNOWN, 0, "", "", option::Arg::None, ""},
        {HELP, 0, "h", "-help", option::Arg::None, "-h/--help \tPrints game flags info"},
        {COUNT, 0, "c", "count", option::Arg::Optional, "-c/--count=n \tSets game rounds amount"},
        {FIRST, 0, "f", "first", option::Arg::Optional, "-f/--first=console/random/optimal \tSets the first player type"},
        {SECOND, 0, "s", "second", option::Arg::Optional, "-s/--second=console/random/optimal \tSets the second player type"},
        {0,0,nullptr,nullptr,nullptr,nullptr}
};

class Parser {
public:
    static int parseArguments(int &argc, char** argv, PlayerType &player1_type, PlayerType &player2_type, int &roundsAmount);
    static bool isNumber(const std::string &s);
};

#endif //NEWLAB3_CMDPARSER_H
