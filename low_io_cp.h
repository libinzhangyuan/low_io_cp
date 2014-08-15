#include <stdbool.h>


// do cp but only for milliseconds then stop
// return true means all file copied.  true means file cp doesn't end.
bool cp_for_time(const int fd_src, const int fd_target, const int msec);

void do_cp(const int fd_src, const int fd_target);
