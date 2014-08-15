#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>

#include "util.h"

// do cp but only for milliseconds then stop
// return true means all file copied.  true means file cp doesn't end.
bool cp_for_time(const int fd_src, const int fd_target, const int msec)
{
    //static off_t cur_cp_pos = 0;

    long long start_time = current_timestamp();

    char read_buf[1024 * 80] = "";
    for(;;)
    {
        ssize_t read_ret = read(fd_src, read_buf, sizeof(read_buf));
        if (read_ret == -1)
        {
            //if (errno == EINTER)
            //    continue;
            error_exit("read error");
        }

        // reading all done
        if (read_ret == 0)
        {
            fsync(fd_target); // force writting to file all.
            return true;
        }
        
        ssize_t len = read_ret;
        write_to_file(fd_target, read_buf, len);

        long long cur_time = current_timestamp();
        if ((cur_time - start_time) > msec)
        {
            fsync(fd_target);
            return false;
        }
    }
}

void do_cp(const int fd_src, const int fd_target)
{
    int each_cp_time = 500;

    for(;;)
    {
        long long start_time = current_timestamp();
        bool b_all_copied = cp_for_time(fd_src, fd_target, each_cp_time);
        if (b_all_copied)
            return;
        long long end_cur_cp_time = current_timestamp();

        if ((end_cur_cp_time - start_time) > 500)
            each_cp_time *= 0.8;
        else
            each_cp_time *= 1.1;
        printf("cur_cp_time milliseconds: %ld  sleep: 500  next cp_time:%d\n", (long)(end_cur_cp_time - start_time), each_cp_time);

        usleep(500 * 1000);
    }
}
