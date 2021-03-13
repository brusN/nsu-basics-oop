#include <iostream>
#include "parser.h"

using namespace workflow_executor;

int main(int argc, char **argv) {
    try {
        // Stores three paths: workflow, default input and output files
        FilePaths filePaths;
        // Reads and puts in filePaths arguments of command line
        Parcer::readProgramArguments(argc, argv, filePaths);

        // For storing read and in progress data
        Data text;
        // Get a list of actions for read data
        std::list <std::shared_ptr<Worker>> conveyor = WorkflowParcer::getScheme(filePaths);
        for (const auto& it : conveyor) {
            text = it->doWork(text);
        }
        std::cout << "Work has been done correctly!";
        return 0;
    }
    catch (const std::logic_error &ex) {
        std::cerr << ex.what() << std::endl;
    }
}