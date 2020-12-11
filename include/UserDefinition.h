#include <iostream>

using namespace std;

class UserDefinition {
public:
    UserDefinition();
    void setRecursive(bool val);
    void setForce(bool val);
    void setVerbose(bool val);

    bool isRecursive();
    bool isForce();
    bool isVerbose();
private:
    // if true, enable recursive delete, otherwise do not perform recursive-delete.
    bool is_recursive_delete;

    // if true, force delete, otherwise ask.
    bool is_force;

    // if true, show all information, like "move", otherwise do not show information.
    bool is_verbose;
};