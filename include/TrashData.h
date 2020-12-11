#ifndef __TRASHDATA_H__
#define __TRASHDATA_H__

#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

/**
 * Trash Data.
 * 
 * This is a structure/class for storing 
 * deletion time, args list, execution directory, full file directory, 
 * trash location for each deleted file using this program.
 */
class TrashData {
private:
    time_t deletion_time; // When does it deleted?
    string args_list; // What was the argument to delete when file was deleted?
    string execution_directory; // Where was the binary-execution directory?
    string file_directory; // Original Full file directory
    string trash_directory; // Trash location

public:
    // Getter
    time_t getDeletionTime();
    string getArgsList();
    string getExeDir();
    string getFileDir();
    string getTrashDir();

    // Setter
    void setDeletionTime(time_t timeval);
    void setArgsList(string arg_list);
    void setExeDir(string exdir);
    void setFileDir(string filddir);
    void setTrashDir(string trashdir);
};
#endif // __TRASHDATA_H__