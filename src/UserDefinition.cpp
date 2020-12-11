#include "UserDefinition.h"

/**
 * UserDefinition: First reset all variables to false.
 */
UserDefinition::UserDefinition() {
    this->is_recursive_delete = false;
    this->is_verbose = false;
    this->is_force = false;
}

/** 
 * Set Functions
 */
void UserDefinition::setRecursive(bool val) {
    this->is_recursive_delete = val;
}
void UserDefinition::setForce(bool val) {
    this->is_force = val;
}
void UserDefinition::setVerbose(bool val) {
    this->is_verbose = val;
}

/**
 * Get Functions
 */
bool UserDefinition::isRecursive() {
    return this->is_recursive_delete;
}
bool UserDefinition::isForce() {
    return this->is_force;
}
bool UserDefinition::isVerbose() {
    return this->is_verbose;
}