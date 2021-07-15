#ifndef GLU_REQUESTS_H
#define GLU_REQUESTS_H

#include "glu_globals.h"

void send_header(int sd, uint8 request, uint8 Command);
void get_header(int sd, glu_header * header);

void send_getattr_request(int sd, uint16 filename_size, char* filename);
void send_access_request(int sd, int32 mask, uint16 filename_size, char* filename);
void send_readlink_request(int sd, uint16 path_size, char* path, size_t size);
void send_opendir_request(int sd, uint16 dir_size, char* dir);
void send_readdir_request(int sd, uint16 dir_size, char* dir, int8 has_fstatat);
void send_mkdir_request(int sd, mode_t mode, uint16 path_size, char* path);
void send_unlink_request(int sd, uint16 path_size, char* path);
void send_rmdir_request(int sd, uint16 path_size, char* path);
void send_rename_request(int sd, uint16 from_size, uint16 to_size, char* from, char* to);
void send_chmod_request(int sd, mode_t mode, uint16 path_size, char* path);
void send_chown_request(int sd, uid_t uid, gid_t gid, uint16 path_size, char* path);
void send_truncate_request(int sd, off_t size, uint16 path_size, char* path);
void send_utimens_request(int sd, uint16 path_size, char* path, struct timespec ts[2]);
void send_create_request(int sd, int32 flags, mode_t mode, uint16 path_size, char* path);
void send_open_request(int sd, int32 flags, uint16 path_size, char* path);
void send_read_request(int sd, size_t size, off_t offset, uint16 path_size, char* path);
void send_write_request(int sd, size_t size, off_t offset, uint16 path_size, char* path, char* buffer);
void send_flush_request(int sd, uint16 path_size, char* path);
void send_release_request(int sd, uint16 pat_size, char* path);

void get_getattr_response(int sd, glu_header * header ,glu_getattr_response * res);
void get_access_response(int sd, glu_header * header ,glu_access_response * res);
void get_readlink_response(int sd, glu_header * header ,glu_readlink_response * res);
void get_opendir_response(int sd, glu_header * header ,glu_opendir_response * res);
void get_readdir_response(int sd, glu_header * header ,glu_readdir_response * res);
void get_mkdir_response(int sd, glu_header * header ,glu_mkdir_response * res);
void get_unlink_response(int sd, glu_header * header ,glu_unlink_response * res);
void get_rmdir_response(int sd, glu_header * header ,glu_rmdir_response * res);
void get_rename_response(int sd, glu_header * header ,glu_rename_response * res);
void get_chmod_response(int sd, glu_header * header ,glu_chmod_response * res);
void get_chown_response(int sd, glu_header * header ,glu_chown_response * res);
void get_truncate_response(int sd, glu_header * header ,glu_truncate_response * res);
void get_utimens_response(int sd, glu_header * header ,glu_utimens_response * res);
void get_create_response(int sd, glu_header * header ,glu_create_response * res);
void get_open_response(int sd, glu_header * header ,glu_open_response * res);
void get_read_response(int sd, glu_header * header ,glu_read_response * res);
void get_write_response(int sd, glu_header * header ,glu_write_response * res);
void get_flush_response(int sd, glu_header * header ,glu_flush_response * res);
void get_release_response(int sd, glu_header * header ,glu_release_response * res);

#endif