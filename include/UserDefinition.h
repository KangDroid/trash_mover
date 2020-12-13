#ifndef __USERDEFINITION_H__
#define __USERDEFINITION_H__

#include <iostream>

using namespace std;

class UserDefinition {
public:
    UserDefinition();
public:
    // if true, enable recursive delete, otherwise do not perform recursive-delete.
    int is_recursive_delete;

    // if true, force delete, otherwise ask.
    int is_force;

    // if true, show all information, like "move", otherwise do not show information.
    int is_verbose;
};
#endif // __USERDEFINITION_H__