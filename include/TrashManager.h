#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <unistd.h>

#include "UserDefinition.h"
#include "TrashData.h"

#define USERNAME_LIMIT 1024

using namespace std;

class TrashManager {
private:
    ifstream files_open;
    ofstream write_open;
    vector<TrashData> trash_list;
    string args_list;

    string get_usr_name();
    string remove_newline(string& target);
    void get_new_filename(filesystem::path& target, filesystem::path& trash, filesystem::path& return_value);
    void init_trashdata();
    TrashData create_trashdata(vector<string> lists);
    TrashData create_trashdata(filesystem::path abstarget, filesystem::path trashdir);
    vector<string> split_string(string& input, char delim);
public:
    TrashManager();
    ~TrashManager();
    void move_to_trash(filesystem::path& target, UserDefinition& udf);
    void setargs(int argc, char** args);
    void show_trashinfo();
};