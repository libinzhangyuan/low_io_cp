
// 获取程序位数（被编译为多少位的代码）
int GetProgramBits(void);

long long current_timestamp(void);

void error_exit(const char* error_text);

void write_to_file(const int fd_target, const char* read_buf, const size_t len);
