#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

// 获取程序位数（被编译为多少位的代码）
int GetProgramBits(void)
{
    return sizeof(int*) * 8;
}


long long current_timestamp(void)
{
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
