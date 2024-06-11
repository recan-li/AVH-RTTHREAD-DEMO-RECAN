
#ifndef __RPC_FS_H__
#define __RPC_FS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int RPC_FILE;

int rpc_fs_init(void);

int rpc_fs_deinit(void);

/* FILE *fopen(const char *pathname, const char *mode); */
RPC_FILE * rpc_fs_fopen(const char *pathname, const char *mode);

/*
int fseek(FILE *stream, long offset, int whence);
*/
int rpc_fs_fseek(RPC_FILE *stream, long offset, int whence);

/*
long ftell(FILE *stream);
*/
long rpc_fs_ftell(RPC_FILE *stream);

/*
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
*/
size_t rpc_fs_fread(void *ptr, size_t size, size_t nmemb, RPC_FILE *stream);

/*
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
*/
size_t rpc_fs_fwrite(const void *ptr, size_t size, size_t nmemb, RPC_FILE *stream);

/*
int fclose(FILE *stream);
*/
int rpc_fs_fclose(RPC_FILE *file);

int rpc_fs_fsize(const char *file_name);

#endif
