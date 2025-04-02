#include "Keys.h"

Keys* Keys::getInstance() {
    static Keys instance;  // Guaranteed to be created only once
    return &instance;  // Return raw pointer to the instance
}