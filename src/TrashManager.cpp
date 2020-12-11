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
    filesystem::rename(filesystem::absolute(target), destination_target);
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