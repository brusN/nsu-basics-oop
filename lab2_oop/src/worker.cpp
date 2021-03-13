#include "parser.h"

using namespace workflow_executor;

// -------- Readfile class definition ---------- //
Readfile::Readfile(const string &filePath_) {
    inputfilePath = filePath_;
}

Data & Readfile::doWork(Data &text) const {
    std::ifstream inputFile;
    inputFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    inputFile.open(inputfilePath);

    if (text.getCountRead() != 0)
        throw std::logic_error("Double readfile!");

    // if file is empty but opened (empty file != error of opening/reading)
    if (inputFile.peek() == std::ifstream::traits_type::eof()) {
        text.incRead();
        return text;
    }

    vector <string> & pData = text.getData();

    string temp;
    while (!inputFile.eof() && getline(inputFile, temp))
        pData.emplace_back(temp);

    inputFile.close();
    text.incRead();
    return text;
}

// -------- Writefile class definition ---------- //
Writefile::Writefile(const string &filePath_) {
    outputFilePath = filePath_;
}

Data & Writefile::doWork(Data &text) const {
    if (text.getCountRead() == 0)
        throw std::logic_error("A read block was expected before write!");

    std::ofstream outputFile;
    outputFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    outputFile.open(outputFilePath);

    vector <string> & pData = text.getData();

    if (!pData.empty()) {
        for (size_t i = 0; i < pData.size() - 1; ++i)
            outputFile << pData[i] << std::endl;
        outputFile << pData[pData.size() - 1];
    }

    text.resetData();
    outputFile.close();
    return text;
}

// -------- Dump class definition ---------- //
Dump::Dump(const string &filePath_) {
    newFilePath = filePath_;
}

Data &Dump::doWork(Data &text) const {
    if (text.getCountRead() == 0)
        throw std::logic_error("A read block was expected before dump!");

    std::ofstream newFile;
    newFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    newFile.open(newFilePath);

    vector <string> & pData = text.getData();

    if (!pData.empty()) {
        for (size_t i = 0; i < pData.size() - 1; ++i)
            newFile << pData[i] << std::endl;
        newFile << pData[pData.size() - 1];
    }

    newFile.close();
    return text;
}

// -------- Grep class definition ---------- //
Grep::Grep(const string &word_) {
    word = word_;
}

Data &Grep::doWork(Data &text) const {
    if (text.getCountRead() == 0)
        throw std::logic_error("A read block was expected before grep!");

    vector <string> & pData = text.getData();

    if (!pData.empty()) {
        string searchString = word;
        std::transform(searchString.begin(), searchString.end(), searchString.begin(), ::toupper);

        string line;
        for (size_t i = 0; i < pData.size(); ++i) {
            line = pData[i];
            std::transform(line.begin(), line.end(), line.begin(), ::toupper);
            if (line.find(searchString) == string::npos)
                pData.erase(pData.begin() + i);
        }
    }

    return text;
}

// -------- Replace class definition ---------- //
Replace::Replace(const string &word1_, const string &word2_) {
    word1 = word1_;
    word2 = word2_;
}

Data &Replace::doWork(Data &text) const {
    if (text.getCountRead() == 0)
        throw std::logic_error("A read block was expected before replace!");

    vector <string> & pData = text.getData();

    if (!pData.empty()) {
        for (auto &line : pData) {
            line = std::regex_replace(line, std::regex(word1), word2);
        }
    }

    return text;
}

// -------- Sort class definition ---------- //
Data &Sort::doWork(Data &text) const {
    if (text.getCountRead() == 0)
        throw std::logic_error("A read block was expected before sort!");

    vector <string> & pData = text.getData();

    if (!pData.empty()) {
        std::sort(pData.begin(), pData.end());
    }

    return text;
}