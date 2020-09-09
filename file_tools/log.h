#include <stdio.h>
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

namespace qilimi{
    void set_log_debug_file(const char*file);
    void set_log_error_file(const char*file);
    int xn_write_log_debug(const char* log);
    int xn_write_log_error(const char* log);
};

