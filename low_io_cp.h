#ifndef LOW_IO_CP_H_
#define LOW_IO_CP_H_

#include <stdbool.h>


// do cp but only for milliseconds then stop
// return copied count in this cp time.  return 0 means all file copied.
off_t cp_for_time(const int fd_src, const int fd_target, const int msec);

void do_cp(const int fd_src, const int fd_target, const struct stat* pstat_buf);

#endif
