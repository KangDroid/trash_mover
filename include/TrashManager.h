#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>
#include <getopt.h>

#include "UserDefinition.h"
#include "TrashData.h"
#include "Configuration.h"

#define USERNAME_LIMIT 1024

using namespace std;

class TrashManager {
private:
    ifstream files_open;
    ofstream write_open;
    vector<TrashData> trash_list;
    string args_list;
    vector<string> file_to_remove;
    string trash_data_lists;
    filesystem::path trash_path;
    bool write_trashdata;

    string get_usr_name();
    string remove_newline(string& target);
    void get_new_filename(filesystem::path& target, filesystem::path& trash, filesystem::path& return_value);
    void init_trashdata();
    TrashData create_trashdata(vector<string> lists);
    TrashData create_trashdata(filesystem::path abstarget, filesystem::path trashdir);
    vector<string> split_string(string& input, char delim);
    void remove_duplicated_data();
    void empty_trash();
    void check_same_push(TrashData itrd);
    void restore_file(TrashData& trd);
    void print_help(string prog_name);
    void print_version();
public:
    TrashManager();
    ~TrashManager();
    void move_to_trash(UserDefinition& udf);
    int setargs(int argc, char** args, UserDefinition& usr_de);
    void show_trashinfo();
    void open_trashrestore();
};