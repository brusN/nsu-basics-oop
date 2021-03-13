#ifndef LAB2_OOOP_PARSER_H
#define LAB2_OOOP_PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <list>

using std::string;
using std::vector;
namespace workflow_executor {

    enum blockID {
        READFILE, WRITEFILE, GREP, SORT, REPLACE, DUMP, UNKNOWN
    };

    class WorkerDesc {
    private:
        blockID idBlock;
        vector<string> arguments;
        blockID getBlockID(const string &s);

    public:
        WorkerDesc(const vector<string> &partsOfLines);
        string argument(const size_t &index) const;
        size_t countArg() const;
        blockID id() const;
    };

    struct FilePaths {
        string workflowFilePath;
        string defaultInputFilePath;
        string defaultOutputFilePath;
    };

    class Data;

    class Worker {
    public:
        virtual Data &doWork(Data &text) const = 0;
    };

    class Readfile : public Worker {
    private:
        string inputfilePath;
    public:
        Readfile(const string &filePath_);
        Data &doWork(Data &text) const override;
    };

    class Writefile : public Worker {
    private:
        string outputFilePath;
    public:
        Writefile(const string &filePath_);
        Data &doWork(Data &text) const override;
    };

    class Dump : public Worker {
    private:
        string newFilePath;
    public:
        Dump(const string &filePath_);
        Data &doWork(Data &text) const override;
    };

    class Grep : public Worker {
    private:
        string word;
    public:
        Grep(const string &word_);
        Data &doWork(Data &text) const override;
    };

    class Replace : public Worker {
    private:
        string word1;
        string word2;
    public:
        Replace(const string &word1_, const string &word2_);
        Data &doWork(Data &text) const override;
    };

    class Sort : public Worker {
    private:
    public:
        Data &doWork(Data &text) const override;
    };

    class Data {
    private:
        vector<string> data;
        size_t countRead;

    public:
        Data();
        vector<string> &getData();
        size_t getCountRead() const;
        void incRead();
        void decRead();
        void resetData();
    };

    class Parcer {
    public:
        static bool readProgramArguments(const int &argc, char **argv, FilePaths &filePaths);
    };

    class WorkflowParcer {
    private:
        static vector<string> splitLine(const string &line);
        static bool isNumber(const string &s);
        static bool isCommandBlock(const vector<string> &partsOfLine);
        static std::shared_ptr<Worker> createNewBlock(const WorkerDesc &element);
        static std::list<std::shared_ptr<Worker>> getToDoList(const std::map<int, WorkerDesc> &commandList, const string &line, const string &defaultInputFilePath, const string &defaultOutputFilePath);

    public:
        static std::list<std::shared_ptr<Worker>> getScheme(const FilePaths &filePaths);
    };
}

#endif //LAB2_OOOP_PARSER_H
