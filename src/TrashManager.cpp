#include "TrashManager.h"

void TrashManager::move_to_trash(filesystem::path& target, UserDefinition& udf) {
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
    get_new_filename(target, trash_path, destination_target);

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

    // Append some information just before delete.
    trash_list.push_back(create_trashdata(filesystem::absolute(target), destination_target));
    // move to trash!
    filesystem::rename(filesystem::absolute(target), destination_target);
}

void TrashManager::setargs(int argc, char** args) {
    args_list = "";
    for (int i = 0; i < argc; i++) {
        if (i == argc - 1) {
            args_list += string(args[i]);
        } else {
            args_list += string(args[i]) + " ";
        }
    }
}

void TrashManager::show_trashinfo() {
    for (int i = 0; i < trash_list.size(); i++) {
        time_t del_time = trash_list.at(i).getDeletionTime();
        string tim_str = string(ctime(&del_time));
        tim_str = remove_newline(tim_str);
        cout << "File " << i + 1 << ":" << endl;
        cout << "Original FileDirectory[before delete]: " << trash_list.at(i).getFileDir() << endl;
        cout << "Deletion Time: " << tim_str << endl;
        cout << "Deletion ARGS: " << trash_list.at(i).getArgsList() << endl;
        cout << "CWD when executed: " << trash_list.at(i).getExeDir() << endl;
        cout << "Current file location[in trash]: " << trash_list.at(i).getTrashDir() << endl;
        cout << endl;
    }
}

/**
 * Checks for any duplicated[same-name] file on trash.
 * param: target - the filename to check, trash - the trash location.
 * return: edited path
 */
void TrashManager::get_new_filename(filesystem::path& target, filesystem::path& trash, filesystem::path& return_value) {
    for (auto& p : filesystem::directory_iterator(trash)) {
        if (target.filename() == p.path().filename()) {
            time_t cur_time = time(NULL);
            string tim_str = string(ctime(&cur_time));
            tim_str = remove_newline(tim_str);
            // In this case, rename target one with appending deletion date.
            return_value = trash / (target.filename().string() + " " + tim_str);
        }
    }
}

/**
 * Load trashdata from file to memory.
 * 
 * The file format
 * 
 * deletion_date\targs_list\tecwd\tfilename\trashdir
 */
void TrashManager::init_trashdata() {
    // Need to handle if file is not opened.
    if (!files_open.is_open()) {
        cerr << "Trash data is NOT opened." << endl;
        return;
    }
    string buffer; // TEMP buffer
    while (getline(files_open, buffer)) {
        // parse its string.
        trash_list.push_back(create_trashdata(split_string(buffer, '\t'))); // split string first, create trashdata, and push back.
    }
}

/**
 * Create Trash data from input lists
 * The lists should be sorted by file-written type,
 * idx 0: deletion date
 * idx 1: args list
 * idx 2: execution cwd
 * idx 3: file name[dir]
 * idx 4: trash dir
 */
TrashData TrashManager::create_trashdata(vector<string> lists) {
    TrashData trash_data;
    trash_data.setDeletionTime(stoi(lists.at(0)));
    trash_data.setArgsList(lists.at(1));
    trash_data.setExeDir(lists.at(2));
    trash_data.setFileDir(lists.at(3));
    trash_data.setTrashDir(lists.at(4));

    return trash_data;
}

/**
 * Overloaded function. 
 * 
 * Two create_trashdata are completely differ, one is creating trashdata from file,
 * other one is creating trashdata from current run - time.
 */
TrashData TrashManager::create_trashdata(filesystem::path abstarget, filesystem::path trashdir) {
    char* buffer = new char[USERNAME_LIMIT];
    TrashData trash_data;
    trash_data.setDeletionTime(time(NULL));
    trash_data.setArgsList(args_list);
    trash_data.setExeDir(string(getcwd(buffer, USERNAME_LIMIT)));
    trash_data.setFileDir(abstarget.string());
    trash_data.setTrashDir(trashdir.string());
    delete[] buffer;

    return trash_data;
}

/**
 * Split strings
 */
vector<string> TrashManager::split_string(string& input, char delim) {
    vector<string> ret_val;
    string tmpbuffer = "";
    for (int i = 0; i < input.length(); i++) {
        if (input.at(i) == delim) {
            ret_val.push_back(tmpbuffer);
            tmpbuffer = "";
        } else {
            tmpbuffer += input.at(i);
        }
    }

    if (tmpbuffer.length() != 0) {
        ret_val.push_back(tmpbuffer);
    }

    return ret_val;
}

TrashManager::TrashManager() {
    files_open.open("/usr/local/kangdroid/trash_lists");
    if (!files_open.is_open()) {
        cerr << "Trash data open failed." << endl;
        return;
    }

    init_trashdata();
}

TrashManager::~TrashManager() {
    if (files_open.is_open()) {
        files_open.close();
    }

    write_open.open("/usr/local/kangdroid/trash_lists", ios::trunc);
    if (!write_open.is_open()) {
        cerr << "Writing trash data open failed" << endl;
        return;
    }

    for (int i = 0; i < trash_list.size(); i++) {
        write_open << trash_list.at(i).getDeletionTime() << "\t" << trash_list.at(i).getArgsList() << "\t" << trash_list.at(i).getExeDir() << "\t" << trash_list.at(i).getFileDir() << "\t" << trash_list.at(i).getTrashDir() << endl;
    }

    files_open.close();
}

/**
 * get_usr_name incldudes some newline at the end. Remove it.
 * param: The string contains newline.
 * returns: Newline - Removed string.
 */
string TrashManager::remove_newline(string& target) {
    string ret_val = "";
    for (int i = 0; i < target.length(); i++) {
        if (target.at(i) != '\n') {
            ret_val += target.at(i);
        }
    }
    return ret_val;
}

/**
 * Get current logged-in username.
 * param: N/A
 * returns: Current user name in string
 */
string TrashManager::get_usr_name() {
    char* username_arr = new char[USERNAME_LIMIT];
    if (getlogin_r(username_arr, USERNAME_LIMIT) != 0) {
        perror("Error occured when getting username");
    }
    string ret_val = string(username_arr);
    delete[] username_arr;
    return ret_val;
}