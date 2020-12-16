#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <unistd.h>
#include <getopt.h>

#include "UserDefinition.h"
#include "TrashData.h"
#include "Configuration.h"

#define USERNAME_LIMIT 1024
#define WORKER_PROCESS 8

using namespace std;

typedef struct remover_structure {
    vector<string> file_to_remove;
} RemoveStructure;

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
    unordered_map<string, filesystem::path> trashcan_lists;
    string cwd_string;
    RemoveStructure st_remove[WORKER_PROCESS];
    pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;

    string get_usr_name();
    string remove_newline(string& target);
    void get_new_filename(filesystem::path& target, filesystem::path& trash, filesystem::path& return_value);
    void init_trashdata();
    TrashData create_trashdata(vector<string> lists);
    TrashData create_trashdata(filesystem::path abstarget, filesystem::path trashdir);
    vector<string> split_string(string& input, char delim);
    void remove_duplicated_data();
    void empty_trash();
    void restore_file(TrashData& trd);
    void print_help(string prog_name);
    void print_version();
    void push_queue(string str) {
        static int itr_ctr = 0;
        st_remove[itr_ctr].file_to_remove.push_back(str);
        itr_ctr++;
        itr_ctr %= WORKER_PROCESS;
    }
public:
    TrashManager();
    ~TrashManager();
    void move_to_trash(UserDefinition* udf, int j);
    int setargs(int argc, char** args, UserDefinition& usr_de);
    void show_trashinfo();
    void open_trashrestore();
};