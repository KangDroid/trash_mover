#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <vector>

#include "UserDefinition.h"
#include "TrashManager.h"

using namespace std;

int main(int argc, char** argv) {
    UserDefinition usr_de;
    TrashManager trash_manager;
    if (trash_manager.setargs(argc, argv, usr_de) == -1) {
        cerr << "Exiting program due to previous error." << endl;
        return -1;
    }
    trash_manager.move_to_trash(usr_de);
    return 0;
}