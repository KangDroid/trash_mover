#include "TrashManager.h"

void TrashManager::move_to_trash(UserDefinition& udf) {
    for (int i = 0; i < file_to_remove.size(); i++) {
        filesystem::path target(file_to_remove.at(i));
        filesystem::path destination_target;
        filesystem::path trash_path;

        if (!strcmp(DEFAULT_TRASH_LOCATION, IS_DYN)) {
            // Get username
            string username_str = get_usr_name();
            // Determine Trash location dynamically.
            trash_path = "/Users/"+username_str+"/.Trash";
        } else {
            trash_path = string(DEFAULT_TRASH_LOCATION);
        }

        // Check if NO trash directory found.
        if (!filesystem::exists(trash_path)) {
            cerr << "No such file or directory: " << trash_path.string() << endl;
            cerr << "Contact Developer with log" << endl;
            return;
        }

        // Return when type is folder, but -r option is not specified.
        if (filesystem::status(target).type() == filesystem::file_type::directory && !udf.is_recursive_delete) {
            cerr << "Recursive -r option is not specified, but folder is found." << endl;
            cerr << "Omitting directory: " << target << endl;
            continue;
        }

        // Default Target directory
        destination_target = trash_path / target.filename();

        // Scan trash if same filename exists.
        get_new_filename(target, trash_path, destination_target);

        if (udf.is_verbose) {
            cout << filesystem::absolute(target) << " --> " << destination_target << endl;
        }

        if (!udf.is_force) {
            string really;
            cout << "Really delete " << filesystem::absolute(target) << "?[y/n] : ";
            getline(cin, really);
            if (really != "y" && really != "yes") {
                cout << "Abort." << endl;
                continue;
            }
        }

        // Append some information just before delete.
        check_same_push(create_trashdata(filesystem::absolute(target), destination_target));
        // move to trash!
        filesystem::rename(filesystem::absolute(target), destination_target);
    }
}

int TrashManager::setargs(int argc, char** args, UserDefinition& usr_de) {
    int is_show = 0;
    int is_version = 0;
    int is_clear = 0;
    int is_empty_trash = 0;
    int is_restore = 0;
    struct option argument_list[] = {
        {"recursive", no_argument, &usr_de.is_recursive_delete, 1},
        {"force", no_argument, &usr_de.is_force, 1},
        {"verbose", no_argument, &usr_de.is_verbose, 1},
        {"show", no_argument, &is_show, 1},
        {"version", no_argument, &is_version, 1},
        {"clear", no_argument, &is_clear, 1},
        {"empty-trash", no_argument, &is_empty_trash, 1},
        {"restore", no_argument, &is_restore, 1},
        {0}
    };
    if (argc < 2) {
        cerr << "Needs at least one argument to delete some files!" << endl;
        return -1;
    }
    /**
     * Supported Options
     * -r --recursive : Recursive
     * -f --force : Force
     * -v --verbose : verbose
     * -s --show : show trash data -- even every flag set, it does not remove anything.
     * --version : Show the version of this program.
     * -C --clear: Clear duplicated trash data
     * -E --empty-trash : Empty[Permanently remove] trash using trashdata.
     */
    char c;
    while ((c = getopt_long(argc, args, "RECrvfs", argument_list, NULL)) != -1) {
        switch(c) {
            case 'r':
                usr_de.is_recursive_delete = true;
            break;
            case 'f':
                usr_de.is_force = true;
            break;
            case 'v':
                usr_de.is_verbose = true;
            break;
            case 's':
                goto show_infonow;
            break;
            case 'C':
                goto clear_duplicated_data;
            break;
            case 'E':
                goto empty_trash_now;
            break;
            case 'R':
                goto restore_trash_now;
            break;
            case '?':
                cerr << "Unknown Argument" << endl;
                return -1;
            break;
        }
    }

    if (is_show) {
show_infonow:
        this->show_trashinfo();
        return 0;
    }

    if (is_clear) {
clear_duplicated_data:
        this->remove_duplicated_data();
        return 0;
    }

    if (is_version) {
        cout << "KangDroid Trash Mover Ver. " << KDR_TRASH_MOVER_VER << endl;
        cout << "Compiled with " << __VERSION__ << ",";
        cout << " on: " << __DATE__ << ", " << __TIME__ << endl;
        return 0;
    }

    if (is_empty_trash) {
empty_trash_now:
        this->empty_trash();
        return 0;
    }

    if (is_restore) {
restore_trash_now:
        this->open_trashrestore();
        return 0;
    }
    
    // Parse
    args_list = "";
    for (int i = 1; i < argc; i++) {
        if (i == argc - 1) {
            args_list += string(args[i]);
        } else {
            args_list += string(args[i]) + " ";
        }

        if (string(args[i]).at(0) == '-') {
            continue;
        } else {
            string path_tmp = string(args[i]);
            filesystem::path current_path(path_tmp);
            if (!filesystem::exists(current_path)) {
                cerr << "Path: " << current_path << " does not exists!" << endl;
                return -1;
            } else {
                // move to trash
                file_to_remove.push_back(current_path);
            }
        }
    }

    return 0;
}

void TrashManager::show_trashinfo() {
    int counter = 0;
    for (TrashData& trd : trash_list) {
        time_t del_time = trd.getDeletionTime();
        string tim_str = string(ctime(&del_time));
        tim_str = remove_newline(tim_str);
        cout << "File " << counter + 1 << ":" << endl;
        cout << "Original FileDirectory[before delete]: " << trd.getFileDir() << endl;
        cout << "Deletion Time: " << tim_str << endl;
        cout << "Deletion ARGS: " << trd.getArgsList() << endl;
        cout << "CWD when executed: " << trd.getExeDir() << endl;
        if (trd.getDeprecated()) {
            cout << "Current file location[in trash] - NOT EXISTS[Empty Trash bin from other process?]: " << trd.getTrashDir() << endl;
        } else {
            cout << "Current file location[in trash]: " << trd.getTrashDir() << endl;
        }
        cout << endl;
        counter++;
    }
}

void TrashManager::open_trashrestore() {
    int idx_input;
    
    cout << "Restore Menu: " << endl;
    this->show_trashinfo();

    cout << "Enter file index number to restore! : ";
    cin >> idx_input;

    if (idx_input < 1 || idx_input > trash_list.size()) {
        cout << "Wrong input. Input range is: 1 ~ " << trash_list.size() << "." << endl;

        // do nothing and exit.
        return;
    }

    // The real index on vector array.
    idx_input--;

    // restore!
    restore_file(trash_list.at(idx_input));
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
        check_same_push(create_trashdata(split_string(buffer, '\t')));
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

    /**
     * Since FILE does not exists in trash can,
     * because User emptied trash bin from other process[i.e Finder] therefore
     * the user file defined on our trashdata might not acutally existss.
     * So, set some flag[deprecated value] if actual file is not on trash can.
     */
    if (!filesystem::exists(filesystem::path(trash_data.getTrashDir()))) {
        trash_data.setDeprecated(true);
    } else {
        trash_data.setDeprecated(false);
    }

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

    // For this case[new-trash data] deprecated field should be false though.
    trash_data.setDeprecated(false);

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

/**
 * After this call, program must exit therefore Destructor is called.
 */
void TrashManager::remove_duplicated_data() {
    for (int i = 0; i < trash_list.size(); i++) {
        if (trash_list.at(i).getDeprecated()) {
            trash_list.erase(trash_list.begin() + i);
        }
    }
}

/**
 * So this is actually BIG important, because we need to ask user for verification.
 * Emptying trash means files are gonna permanently deleted!
 */
void TrashManager::empty_trash() {

    /**
     * INIT trash path
     */
    filesystem::path trash_path;
    if (!strcmp(DEFAULT_TRASH_LOCATION, IS_DYN)) {
        // Get username
        string username_str = get_usr_name();
        // Determine Trash location dynamically.
        trash_path = "/Users/"+username_str+"/.Trash";
    } else {
        trash_path = string(DEFAULT_TRASH_LOCATION);
    }

    string remove_verification = "";
    int counter = 0;
    for (TrashData& trd : trash_list) {
        if (!trd.getDeprecated()) {
            counter++;
            remove_verification += trd.getTrashDir() + "\n";
        }
    }

    if (counter == 0) {
        cerr << "Nothing to erase!" << endl;
        return;
    }

    string really;
    cout << "Are you really sure to empty trashcan[" << trash_path.string() << "]?" << endl;
    cout << "Confirm these files are removed from trashcan![" << counter << " Files!]" << endl;
    cout << remove_verification << endl;
    cout << "Confirm?[y/n] : ";
    getline(cin, really);
    if (really != "y" && really != "yes") {
        cout << "Abort." << endl;
        return;
    }

    // remove process goes on.
    for (TrashData& trd : trash_list) {
        if (!trd.getDeprecated()) {
            filesystem::remove_all(trd.getTrashDir());
        }
    }
}

void TrashManager::check_same_push(TrashData itrd) {
    // Check any same entry on trash list.
    for (TrashData& trd : trash_list) {
        if (trd.getTrashDir() == itrd.getTrashDir()) {
            return;
        }
    }

    // If this state is reached, that means there is no same entries.
    trash_list.push_back(itrd);
}

/**
 * Restore file with argument trd.
 * 
 * This function will check existance in original location,
 * and it will restore it!
 * 
 * Shell - command would be:
 * mv $TRASH_LOCATION_FILE $ORIGINAL_LOCATOIN
 */
void TrashManager::restore_file(TrashData& trd) {
    if (filesystem::exists(trd.getFileDir())) {
        cerr << "Tried to restore file: " << trd.getFileDir() << " on " << trd.getTrashDir() << "," << endl;
        cerr << "But a file already exists on " << trd.getFileDir() << "." << endl;
        cerr << "Aborting.." << endl;
        return;
    }

    // restore!
    filesystem::rename(trd.getTrashDir(), trd.getFileDir());
    cout << "File restored from " << trd.getTrashDir() << " to " << trd.getFileDir() << "." << endl;
}

TrashManager::TrashManager() {
    
    // Dynamic
    if (!strcmp(DEFAULT_TRASH_DATA_LOCATION, IS_DYN)) {
        /**
         * The thing: What if we need to use "sudo"?
         * TODO: Relying username might not ideal if we need to use "sudo"
         */
        string usr_name = this->get_usr_name();
        trash_data_lists = "/Users/" + usr_name + "/.trash_lists";
    } else {
        trash_data_lists = string(DEFAULT_TRASH_DATA_LOCATION);
    }
    files_open.open(trash_data_lists);
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

    write_open.open(trash_data_lists, ios::trunc);
    if (!write_open.is_open()) {
        cerr << "Writing trash data open failed" << endl;
        return;
    }

    for (TrashData& trd : trash_list) {
        write_open << trd.getDeletionTime() << "\t" << trd.getArgsList() << "\t" << trd.getExeDir() << "\t" << trd.getFileDir() << "\t" << trd.getTrashDir() << endl;
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