#ifndef GLU_GLOBALS_H
#define GLU_GLOBALS_H

#define FUSE_USE_VERSION 31

#define GLU_PORT 50000

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>

#define GLU_DEBUG
//#define GLU_DEBUG_HEADER


typedef char 					int8;
typedef unsigned char 			uint8;
typedef short 					int16;
typedef unsigned short 			uint16;
typedef int 					int32;
typedef unsigned int 			uint32;
typedef long long int 			int64;
typedef unsigned long long int 	uint64;

#define FILENAME_MAX_SIZE 255
#define GLU_REQUEST 1
#define GLU_RESPONSE 0

#ifdef GLU_DEBUG
#define glu_log(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

#ifdef GLU_DEBUG_HEADER
#define print_header(header) \
	glu_log("\n\nHEADER\n"); \
	glu_log("Signature:[%s]\n",header.Signature); \
	glu_log("StructureSize:[%d]\n", header.StructureSize); \
	glu_log("Command:[%d]\n",header.Command); \
	glu_log("Flags:[%d]\n", header.Flags); \
	glu_log("Status:[%d]\n", header.Status); \
	glu_log("MID:[%d]\n", header.MID);
#else
#define print_header(...)
#endif

#else
#define glu_log(format, ...)
#endif

enum glu_commands{
	 GETATTR,
	 ACCESS,
	 READLINK,
	 OPENDIR,
	 READDIR,
	 MKDIR,
	 UNLINK,
	 RMDIR,
	 RENAME,
	 CHMOD,
	 CHOWN,
	 TRUNCATE,
	 UTIMENS,
	 CREATE,
	 OPEN,
	 READ,
	 WRITE,
	 FLUSH,
	 RELEASE
};
#pragma pack(push,1)

typedef struct{

	int8 Signature[4]; //will be set to "GLU"
	uint8 StructureSize;
	uint8 Command;
	uint8 Flags;
	int32 Status;
	uint64 MID; //to match request-response

}glu_header;

typedef struct{

	uint16 filename_size;
	struct stat st;
	char* filename;

}file_stat;


typedef struct{
	struct{
		uint16 filename_size;
	}fixed;
	char* filename;

}glu_getattr_request;
typedef struct{

	struct stat st;

}glu_getattr_response;


typedef struct{

	struct{
		int32 mask;
		uint16 filename_size;
	}fixed;
	char* filename;

}glu_access_request;
typedef struct{

	int32 status;
//will use status from header

}glu_access_response;


typedef struct{

	struct{
		uint16 path_size;
		size_t size;
	}fixed;
	char* path;

}glu_readlink_request;
typedef struct{

	struct{
		uint16 buf_size;
	}fixed;
	char* buf;

}glu_readlink_response;


typedef struct{

	struct{
		uint16 dir_size;
	}fixed;
	char* dir;

}glu_opendir_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_opendir_response;


typedef struct{

	struct{
		int8 has_fstatat;
		uint16 dir_size;
	}fixed;
	char* dir;

}glu_readdir_request;
typedef struct{

	struct{
		int8 has_fstatat; /* 1 - has, 0 - doesn't */
		uint16 nr_of_entries;
	}fixed;
	file_stat* fs;

}glu_readdir_response;


typedef struct{

	struct{
		mode_t mode;
		uint16 path_size;
	}fixed;
	char* path;

}glu_mkdir_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_mkdir_response;


typedef struct{

	struct{
		uint16 path_size;
	}fixed;
	char* path;

}glu_unlink_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_unlink_response;

typedef struct{

	struct{
		uint16 path_size;
	}fixed;
	char* path;

}glu_rmdir_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_rmdir_response;


typedef struct{

	struct{
		uint16 from_size;
		uint16 to_size;
	}fixed;
	char* from;
	char* to;

}glu_rename_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_rename_response;


typedef struct{

	struct{
		mode_t mode;
		uint16 path_size;
	}fixed;
	char* path;

}glu_chmod_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_chmod_response;


typedef struct{

	struct{
		uid_t uid;
		gid_t gid;
		uint16 path_size;
	}fixed;
	char* path;

}glu_chown_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_chown_response;


typedef struct{

	struct{
		off_t size;
		uint16 path_size;
	}fixed;
	char* path;

}glu_truncate_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_truncate_response;


typedef struct{

	struct{
		uint16 path_size;
		struct timespec ts[2];
	}fixed;
	char* path;

}glu_utimens_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_utimens_response;


typedef struct{

	struct{
		int32 flags;
		mode_t mode;
		uint16 path_size;
	}fixed;
	char* path;

}glu_create_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_create_response;

typedef struct{

	struct{
		int32 flags;
		uint16 path_size;
	}fixed;
	char* path;

}glu_open_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_open_response;


typedef struct{

	struct{
		size_t size;
		off_t offset;
		uint16 path_size;
	}fixed;
	char* path;

}glu_read_request;
typedef struct{

	struct{
		size_t buf_size;
	}fixed;
	char* buf;

}glu_read_response;


typedef struct{

	struct{
		size_t size;
		off_t offset;
		uint16 path_size;
	}fixed;
	char* path;
	char* buffer;

}glu_write_request;
typedef struct{

	int32 bytes; //nr of bytes written

}glu_write_response;


typedef struct{

	struct{
		uint16 path_size;
	}fixed;
	char* path;

}glu_flush_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_flush_response;


typedef struct{

	struct{
		uint16 path_size;
	}fixed;
	char* path;

}glu_release_request;
typedef struct{

	int32 status;
//will use status from header;

}glu_release_response;

#pragma pack(pop)

#endif