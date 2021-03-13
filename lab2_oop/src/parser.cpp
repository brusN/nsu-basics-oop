#include "parser.h"

using namespace workflow_executor;

bool Parcer::readProgramArguments(const int &argc, char **argv, FilePaths &filePaths) {
    if (argc < 2 || argc > 6)
        throw std::invalid_argument("Invalid count command line arguments!");

    // For checking flag syntax
    std::regex flag_regex("-[ioIO]");
    // For checking file path syntax
    std::regex filePath_regex("[\\w]+\\.txt");
    std::cmatch result;

    if (!std::regex_match(argv[1], result, filePath_regex))
        throw std::logic_error("Wrong workflow file name path!");
    filePaths.workflowFilePath = argv[1]; // worker instructions file path

    if (argc > 2) { // if have any flags
        for (int i = 2; i < argc; i += 2) {
            // We have only two flags: -i -i (-I -O)
            if (!std::regex_match(argv[i], result, flag_regex))
                throw std::invalid_argument("Wrong flag syntax from command line!");
            // The file path should be like this: filepath.txt or /.../.../filepath.txt
            if (!std::regex_match(argv[i + 1], result, filePath_regex))
                throw std::invalid_argument("Wrong file path from command line!");

            if ((string) argv[i] == "-i" || (string) argv[i] == "-I") {
                filePaths.defaultInputFilePath = argv[i + 1];
            } else {
                filePaths.defaultOutputFilePath = argv[i + 1];
            }
        }
    }
    return true;
}

std::list<std::shared_ptr<Worker>> WorkflowParcer::getScheme(const FilePaths &filePaths) {
    std::ifstream workflowFile;
    // workflowFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    workflowFile.open(filePaths.workflowFilePath);

    std::map<int, WorkerDesc> commandList;
    std::list<std::shared_ptr<Worker>> conveyor;

    string line;
    vector<string> partsOfLine;

    while (getline(workflowFile, line)) {
        if (line == "desc")
            break;
    }

    if (line != "desc")
        throw std::logic_error("Desc block not found!");

    while (getline(workflowFile, line)) {
        if (!line.empty()) {
            if (line == "csed") {
                break;
            } else {
                partsOfLine = WorkflowParcer::splitLine(line); // example command: 1 = readfile in.txt (has 4 parts)
                if (!isCommandBlock(partsOfLine))
                    throw std::logic_error("Syntax error in block command!");
                int num = stoi(partsOfLine[0]);
                commandList.insert(std::pair<int, WorkerDesc>(num, WorkerDesc(partsOfLine)));
            }
        }
    }

    if (line != "csed")
        throw std::logic_error("Csed block not found!");


    while (getline(workflowFile, line)) {
        if (!line.empty())
            break;
    }

    if (line.empty() || line == "csed")
        throw std::logic_error("Scheme block not found!");

    conveyor = getToDoList(commandList, line, filePaths.defaultInputFilePath, filePaths.defaultOutputFilePath);
    workflowFile.close();

    return conveyor;
}


std::list<std::shared_ptr<Worker>>
WorkflowParcer::getToDoList(const std::map<int, WorkerDesc> &commandList, const string &line, const string &defaultInputFilePath, const string &defaultOutputFilePath) {
    std::list<std::shared_ptr<Worker>> conveyor;
    vector<string> partsOfLine = WorkflowParcer::splitLine(line);

    if (!WorkflowParcer::isNumber(partsOfLine[0]))
        throw std::logic_error("Syntax error! Undefined number of block!");

    int numBlock = stoi(partsOfLine[0]);
    auto it = commandList.find(numBlock);

    if (it == commandList.end())
        throw std::logic_error("Block with the given number not found! Check workflow your file");

    if (it->second.id() != READFILE) {
        if (defaultInputFilePath.empty())
            throw std::logic_error("Default input stream not found!");
        conveyor.push_back(std::shared_ptr<Worker> (new Readfile(defaultInputFilePath)));
    }
    conveyor.push_back(createNewBlock(it->second));
    bool wasArrow = false;

    for (size_t i = 1; i < partsOfLine.size(); ++i) {
        if (i % 2 == 1) {
            if (partsOfLine[i] != "->")
                throw std::logic_error("Syntax error! Arrow to next block not found!");
            wasArrow = true;
        } else {
            if (!wasArrow || !WorkflowParcer::isNumber(partsOfLine[i]))
                throw std::logic_error("Syntax error! Undefined block number or arrow expected!");

            numBlock = stoi(partsOfLine[i]);
            it = commandList.find(numBlock);

            if (it == commandList.end())
                throw std::logic_error("Block with the given number not found! Check workflow your file");

            conveyor.push_back(createNewBlock(it->second));
            wasArrow = false;
        }
    }

    // Consider the case: ... -> ... -> ? /* excepted number */
    if (wasArrow) {
        throw std::logic_error("Syntax error in block scheme line! Block number expected");
    }

    if (it->second.id() != WRITEFILE) {
        if (defaultOutputFilePath.empty())
            throw std::logic_error("Default output stream not found!");
        conveyor.push_back(std::shared_ptr<Worker> (std::shared_ptr<Worker> (new Writefile(defaultOutputFilePath))));
    }

    return conveyor;
}


bool WorkflowParcer::isNumber(const string &s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

bool WorkflowParcer::isCommandBlock(const vector<string> &partsOfLine) {
    if (partsOfLine.size() < 3 || partsOfLine.size() > 5)
        return false;

    if (!isNumber(partsOfLine[0]))
        return false;

    if (partsOfLine[1] != "=")
        return false;

    return true;
}

vector<string> WorkflowParcer::splitLine(const string &line) {
    vector<string> words;
    string word;

    for (char symbol : line) {
        if (symbol != ' ') {
            word += symbol;
        } else {
            if (!word.empty()) {
                words.emplace_back(word);
                word = "";
            }
        }
    }

    if (!word.empty())
        words.emplace_back(word);

    return words;
}

vector<string> &Data::getData() {
    return data;
}

void Data::resetData() {
    data.clear();
    --countRead;
}

void Data::incRead() {
    ++countRead;
}

void Data::decRead() {
    --countRead;
}

Data::Data() {
    countRead = 0;
}

size_t Data::getCountRead() const {
    return countRead;
}
