#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
// 获取程序位数（被编译为多少位的代码）
int GetProgramBits()
{
    return sizeof(int*) * 8;
}


long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    return milliseconds;
}

void error_exit(const char* error_text)
{
    printf("\nError exit because %s",error_text);
    exit(-1);
}


void write_to_file(const int fd_target, const char* read_buf, const size_t len)
{
    write(fd_target, read_buf, len);
}

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

// Useage: low_io_cp /data/file1 /date/file2
// only support copy one file. Do not support dir.
// only support file with full path name.
int main(int argc, char **argv)
{
    printf("bits:\t%d\n", GetProgramBits());

    if (argc != 3)
    {
        printf("Wrong parameter\nPlease give source file name and target file name. \n Example: low_io_cp /data/file1 /date/file2\n");
        return -1;
    }

    char* pfile1_name = argv[1];
    char* pfile2_name = argv[2];

    int fd_src = open(pfile1_name, O_RDONLY);
    if (fd_src == -1)
    {
        error_exit("Can't open source file");
    }

    int fd_target = open(pfile2_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd_target == -1)
    {
        error_exit("Can't open target file");
    }

    const time_t begin_time = time(NULL);
    do_cp(fd_src, fd_target);
    const time_t end_time = time(NULL);
    printf("total seconds of copying file: %ld\n", end_time - begin_time);
    exit(0);
}
