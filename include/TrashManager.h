#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>

#include "UserDefinition.h"

#define USERNAME_LIMIT 1024

using namespace std;

class TrashManager {
public:
    string get_usr_name();
    string remove_newline(string& target);
    void move_to_trash(filesystem::path& target, UserDefinition& udf);
    void get_new_filename(filesystem::path& target, filesystem::path& trash, filesystem::path& return_value);
};