#include "TrashData.h"
time_t TrashData::getDeletionTime() {
    return this->deletion_time;
}

string TrashData::getArgsList() {
    return this->args_list;
}

string TrashData::getExeDir() {
    return this->execution_directory;
}

string TrashData::getFileDir() {
    return this->file_directory;
}

string TrashData::getTrashDir() {
    return this->trash_directory;
}

void TrashData::setDeletionTime(time_t timeval) {
    this->deletion_time = timeval;
}

void TrashData::setArgsList(string arg_list) {
    this->args_list = arg_list;
}

void TrashData::setExeDir(string exdir) {
    this->execution_directory = exdir;
}

void TrashData::setFileDir(string filddir) {
    this->file_directory = filddir;
}

void TrashData::setTrashDir(string trashdir) {
    this->trash_directory = trashdir;
}
