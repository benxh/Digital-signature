#include <stdio.h>
#include <dirent.h>

#define FR_OK 1

#define FA_OPEN_EXISTING 1
#define FA_READ 2

typedef FILE FIL;
typedef struct{}FATFS;

typedef struct{
	char fname[1024];
}FILINFO;