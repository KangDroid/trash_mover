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
    string execution_directory; // Where was the binary-execution directory?
    string file_directory; // Original Full file directory
    string trash_directory; // Trash location
    bool is_deprecated; // If trash bin is modified by other process[i.e Finder].

public:
    // Getter
    time_t getDeletionTime();
    string getExeDir();
    string getFileDir();
    string getTrashDir();
    bool getDeprecated();

    // Setter
    void setDeletionTime(time_t timeval);
    void setExeDir(string exdir);
    void setFileDir(string filddir);
    void setTrashDir(string trashdir);
    void setDeprecated(bool dep);
};
#endif // __TRASHDATA_H__