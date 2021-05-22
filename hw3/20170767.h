#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define BUFSIZE 100
#define NUMOFPID 4

#define READ_NUM 0
#define WRITE_NUM 1


#define SIZEOFCOMMAND 4

#define COMMAND_READ "READ"
#define COMMAND_FINISH "FIN"
#define COMMAND_EXIT "EXIT"
#define COMMAND_BYE "BYEE"

