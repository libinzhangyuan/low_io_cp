#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/time.h>

#include "low_io_cp.h"
#include "util.h"

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
