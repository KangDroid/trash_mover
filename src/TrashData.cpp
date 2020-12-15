#include "TrashData.h"
time_t TrashData::getDeletionTime() {
    return this->deletion_time;
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

bool TrashData::getDeprecated() {
    return this->is_deprecated;
}

void TrashData::setDeletionTime(time_t timeval) {
    this->deletion_time = timeval;
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

void TrashData::setDeprecated(bool dep) {
    this->is_deprecated = dep;
}
