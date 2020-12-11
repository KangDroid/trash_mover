#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>

#include "UserDefinition.h"
#include "TrashManager.h"

using namespace std;

int main(int argc, char** argv) {
    UserDefinition usr_de;
    TrashManager trash_manager;
    if (argc < 2) {
        cerr << "Needs at least one argument to delete some files!" << endl;
        return -1;
    }

    /**
     * Supported Options
     * -r : Recursive
     * -f : Force
     * -v : verbose
     */
    char c;
    while ((c = getopt(argc, argv, "rvf")) != -1) {
        switch(c) {
            case 'r':
                usr_de.setRecursive(true);
            break;
            case 'f':
                usr_de.setForce(true);
            break;
            case 'v':
                usr_de.setVerbose(true);
            break;
            case '?':
                cerr << "Unknown Argument" << endl;
            break;
        }
    }
    
    // Parse
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]).at(0) == '-') {
            continue;
        } else {
            string path_tmp = string(argv[i]);
            filesystem::path current_path(path_tmp);
            if (!filesystem::exists(current_path)) {
                cerr << "Path: " << current_path << endl;
            } else {
                // move to trash
                trash_manager.move_to_trash(current_path, usr_de);
            }
        }
    }
    return 0;
}