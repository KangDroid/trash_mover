#include <iostream>
#include <fstream>
#include <filesystem>
#include <unistd.h>

#include "UserDefinition.h"

using namespace std;

#define USERNAME_LIMIT 1024

/**
 * Get current logged-in username.
 * param: N/A
 * returns: Current user name in string
 */
string get_usr_name() {
    char* username_arr = new char[USERNAME_LIMIT];
    if (getlogin_r(username_arr, USERNAME_LIMIT) != 0) {
        perror("Error occured when getting username");
    }
    string ret_val = string(username_arr);
    delete[] username_arr;
    return ret_val;
}

/**
 * get_usr_name incldudes some newline at the end. Remove it.
 * param: The string contains newline.
 * returns: Newline - Removed string.
 */
string remove_newline(string& target) {
    string ret_val = "";
    for (int i = 0; i < target.length(); i++) {
        if (target.at(i) != '\n') {
            ret_val += target.at(i);
        }
    }
    return ret_val;
}

void move_to_trash(filesystem::path& target, UserDefinition& udf) {
    filesystem::path destination_target;
    // Get username
    string username_str = get_usr_name();
    filesystem::path trash_path("/Users/"+username_str+"/.Trash");

    // Check if NO trash directory found.
    if (!filesystem::exists(trash_path)) {
        cerr << "No such file or directory: " << trash_path.string() << endl;
        cerr << "Contact Developer with log" << endl;
        return;
    }

    // Return when type is folder, but -r option is not specified.
    if (filesystem::status(target).type() == filesystem::file_type::directory && !udf.isRecursive()) {
        cerr << "Recursive -r option is not specified, but folder is found." << endl;
        cerr << "Omitting directory: " << target << endl;
        return;
    }

    // Default Target directory
    destination_target = trash_path / target.filename();

    // Scan trash if same filename exists.
    for (auto& p : filesystem::directory_iterator(trash_path)) {
        if (target.filename() == p.path().filename()) {
            time_t cur_time = time(NULL);
            string tim_str = string(ctime(&cur_time));
            tim_str = remove_newline(tim_str);
            // In this case, rename target one with appending deletion date.
            destination_target = trash_path / (target.filename().string() + " " + tim_str);
        }
    }

    if (udf.isVerbose()) {
        cout << filesystem::absolute(target) << " --> " << destination_target << endl;
    }

    if (!udf.isForce()) {
        string really;
        cout << "Really delete " << filesystem::absolute(target) << "?[y/n] : ";
        getline(cin, really);
        if (really != "y" && really != "yes") {
            cout << "Abort." << endl;
            return;
        }
    }
    filesystem::rename(filesystem::absolute(target), destination_target);
}

int main(int argc, char** argv) {
    UserDefinition usr_de;
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
                move_to_trash(current_path, usr_de);
            }
        }
    }
    return 0;
}