#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

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
// return true means file cp doesn't end.   false means all file copied.
bool cp_for_time(const int fd_src, const int fd_target, const int msec)
{
    static off_t cur_cp_pos = 0;

    char read_buf[1024 * 80] = "";
    while(1)
    {
        ssize_t read_ret = read(fd_src, read_buf, sizeof(read_buf));
        if (read_ret == -1)
        {
            //if (errno == EINTER)
            //    continue;
            error_exit("read error");
        }
        if (read_ret == 0)
        {
            fsync(fd_target);
            return false;
        }
        
        ssize_t len = read_ret;
        write_to_file(fd_target, read_buf, len);
    }

    return true;
}

void do_cp(const int fd_src, const int fd_target)
{
    while (cp_for_time(fd_src, fd_target, 500))
    {
        usleep(500 * 1000);
    }
}

// Useage: low_io_cp /data/file1 /date/file2
// only support copy one file. Do not support dir.
// only support file with full path name.
int main(int argc, char **argv)
{
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

    do_cp(fd_src, fd_target);
    exit(0);
}
