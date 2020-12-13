#include "UserDefinition.h"

/**
 * UserDefinition: First reset all variables to false.
 */
UserDefinition::UserDefinition() {
    this->is_recursive_delete = false;
    this->is_verbose = false;
    this->is_force = false;
}