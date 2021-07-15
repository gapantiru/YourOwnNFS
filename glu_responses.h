#ifndef GLU_RESPONSES_H
#define GLU_RESPONSES_H

#include "glu_globals.h"

void send_header(int sd, uint8 request, uint8 Command, int32 Status);
void get_header(int sd, glu_header * header);

int send_response(int client, glu_header * header);

void send_getattr_response(int client, glu_header * header);
void send_access_response(int client, glu_header * header);
void send_readlink_response(int client, glu_header * header);
void send_opendir_response(int client, glu_header * header);
void send_readdir_response(int client, glu_header * header);
void send_mkdir_response(int client, glu_header * header);
void send_unlink_response(int client, glu_header * header);
void send_rmdir_response(int client, glu_header * header);
void send_rename_response(int client, glu_header * header);
void send_chmod_response(int client, glu_header * header);
void send_chown_response(int client, glu_header * header);
void send_truncate_response(int client, glu_header * header);
void send_utimens_response(int client, glu_header * header);
void send_create_response(int client, glu_header * header);
void send_open_response(int client, glu_header * header);
void send_read_response(int client, glu_header * header);
void send_write_response(int client, glu_header * header);
void send_flush_response(int client, glu_header * header);
void send_release_response(int client, glu_header * header);

void get_getattr_request(int client, glu_getattr_request * req);
void get_access_request(int client, glu_access_request * req);
void get_readlink_request(int client, glu_readlink_request * req);
void get_opendir_request(int client, glu_opendir_request * req);
void get_readdir_request(int client, glu_readdir_request * req);
void get_mkdir_request(int client, glu_mkdir_request * req);
void get_unlink_request(int client, glu_unlink_request * req);
void get_rmdir_request(int client, glu_rmdir_request * req);
void get_rename_request(int client, glu_rename_request * req);
void get_chmod_request(int client, glu_chmod_request * req);
void get_chown_request(int client, glu_chown_request * req);
void get_truncate_request(int client, glu_truncate_request * req);
void get_utimens_request(int client, glu_utimens_request * req);
void get_create_request(int client, glu_create_request * req);
void get_open_request(int client, glu_open_request * req);
void get_read_request(int client, glu_read_request * req);
void get_write_request(int client, glu_write_request * req);
void get_flush_request(int client, glu_flush_request * req);
void get_release_request(int client, glu_release_request * req);

#endif