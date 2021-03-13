#include "parser.h"

using namespace workflow_executor;

std::shared_ptr<Worker> WorkflowParcer::createNewBlock(const WorkerDesc &block) {
    switch (block.id()) {
        case READFILE:
            if (block.countArg() == 1) {
                return std::shared_ptr<Worker>(new Readfile( block.argument(0)));
            } else
                throw std::invalid_argument("Extra or missed arguments found in readfile block!");

        case WRITEFILE:
            if (block.countArg() == 1) {
                return std::shared_ptr<Worker>(new Writefile(block.argument(0)));
            } else
                throw std::invalid_argument("Extra or missed arguments found in writefile block!");

        case GREP:
            if (block.countArg() == 1) {
                return std::shared_ptr<Worker>(new Grep(block.argument(0)));
            } else {
                throw std::invalid_argument("Invalid argument in grep block!");
            }

        case SORT:
            if (block.countArg() == 0) {
                return std::shared_ptr<Worker>(new Sort());
            } else {
                throw std::invalid_argument("Extra arguments found in sort block!");
            }

        case REPLACE:
            if (block.countArg() == 2) {
                return std::shared_ptr<Worker>(new Replace(block.argument(0), block.argument(1)));
            } else {
                throw std::invalid_argument("Invalid arguments in replace block!");
            }

        case DUMP:
            if (block.countArg() == 1) {
                return std::shared_ptr<Worker>(new Dump(block.argument(0)));
            } else {
                throw std::invalid_argument("Invalid argument in dump block!");
            }

        default:
            throw std::logic_error("Unknown block name!");
    }
}

WorkerDesc::WorkerDesc(const vector<string> &partsOfLines) {
    idBlock = getBlockID(partsOfLines[2]);
    for (size_t i = 3; i < partsOfLines.size(); ++i)
        arguments.emplace_back(partsOfLines[i]);
}

blockID WorkerDesc::getBlockID(const string &s) {
    if (s == "readfile")
        return READFILE;
    else if (s == "writefile")
        return WRITEFILE;
    else if (s == "grep")
        return GREP;
    else if (s == "sort")
        return SORT;
    else if (s == "replace")
        return REPLACE;
    else if (s == "dump")
        return DUMP;
    else
        return UNKNOWN;
}

size_t WorkerDesc::countArg() const {
    return arguments.size();
}

blockID WorkerDesc::id() const {
    return idBlock;
}

string WorkerDesc::argument(const size_t &index) const {
    if (index <= arguments.size()) {
        return arguments[index];
    } else {
        return "";
    }
}
