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
// return copied count in this cp time.  return 0 means all file copied.
off_t cp_for_time(const int fd_src, const int fd_target, const int msec)
{
    //static off_t cur_cp_pos = 0;

    long long start_time = current_timestamp();
    off_t cp_count = 0;

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
            return 0;
        }
        
        ssize_t len = read_ret;
        cp_count += len;
        write_to_file(fd_target, read_buf, len);

        long long cur_time = current_timestamp();
        if ((cur_time - start_time) > msec)
        {
            fsync(fd_target);
            return cp_count;
        }
    }
}

void do_cp(const int fd_src, const int fd_target, const struct stat* pstat_buf)
{
    int each_cp_time = 500 * 0.5;
    const off_t file_size = pstat_buf->st_size;
    off_t all_cp_count = 0;

    for(;;)
    {
        long long start_time = current_timestamp();
        off_t copied_count = cp_for_time(fd_src, fd_target, each_cp_time);
        if (copied_count == 0)
            return;
        long long end_cur_cp_time = current_timestamp();

        if ((end_cur_cp_time - start_time) > 500)
            each_cp_time *= 0.8;
        else
            each_cp_time *= 1.1;
        all_cp_count += copied_count;
        printf("%ld%% cur_cp_time milliseconds: %ld  sleep: 500  next read_time:%d\n", (long)(all_cp_count * 100 / file_size), (long)(end_cur_cp_time - start_time), each_cp_time);

        usleep(500 * 1000);
    }
}
