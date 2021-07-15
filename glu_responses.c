#include "glu_responses.h"

extern int errno;

typedef void (*senders)(int client, glu_header* header);

static senders SENDERS[] = {

/*GETATTR*/		send_getattr_response,
/*ACCESS*/		send_access_response,
/*READLINK*/	send_readlink_response,
/*OPENDIR*/		send_opendir_response,
/*READDIR*/		send_readdir_response,
/*MKDIR*/		send_mkdir_response,
/*UNLINK*/		send_unlink_response,
/*RMDIR*/		send_rmdir_response,
/*RENAME*/		send_rename_response,
/*CHMOD*/		send_chmod_response,
/*CHOWN*/		send_chown_response,
/*TRUNCATE*/	send_truncate_response,
/*UITMENS*/		send_utimens_response,
/*CREATE*/		send_create_response,
/*OPEN*/		send_open_response,
/*READ*/		send_read_response,
/*WRITE*/		send_write_response,
/*FLUSH*/		send_flush_response,
/*RELEASE*/		send_release_response

};

int send_response(int client, glu_header * header){

    if(header->Command < GETATTR || header->Command > RELEASE)
        return -1;

	SENDERS[header->Command](client, header);

	return 1;

}

void send_header(int sd, uint8 request, uint8 Command, int32 Status){

	glu_header header;

	memcpy(header.Signature, "GLU", strlen("GLU"));

	header.StructureSize = sizeof(glu_header);
	header.Command = Command;
	header.Status = Status;
	header.Flags = request;
	header.MID = 0;

	write(sd, &header, sizeof(header));

}

void get_header(int sd, glu_header * header){

	read(sd, header, sizeof(glu_header));

}


void send_getattr_response(int client, glu_header * header){
	
	glu_getattr_request req;
	glu_getattr_response res;
	int32 Status = 0;

	get_getattr_request(client, &req);

 	glu_log("Filename: %s\n", req.filename);
 	glu_log("Filename size: %d\n", req.fixed.filename_size);

 	int r = lstat(req.filename, &(res.st));
 	if(r == -1) Status = errno;

 	send_header(client, GLU_RESPONSE, GETATTR, Status);
 	
 	if(!r)
 		write(client, &res, sizeof(glu_getattr_response));
 	glu_log("Sended getattr_response!\n");

}

void send_access_response(int client, glu_header * header){

	

}


void send_readlink_response(int client, glu_header * header){

	glu_readlink_request req;
	glu_readlink_response res;
	int32 Status = 0;

	get_readlink_request(client, &req);

	glu_log("READLINK request - path: [%s]\n", req.path);
	
	res.buf = (char*)malloc(req.fixed.size);
	int r = readlink(req.path, res.buf, req.fixed.size - 1);
	res.fixed.buf_size = r + 1;

	if(r == -1)
		Status = errno;

	send_header(client, GLU_RESPONSE, READLINK, Status);

	glu_log("READLINK request - status: [%d]\n", Status);

	if(r == -1){
		free(res.buf);
		return;
	}

	glu_log("READLINK request - buf_size: [%d]\n", res.fixed.buf_size - 1);

	res.buf[r] = '\0';

	glu_log("READLINK request - buf: [%s]\n", res.buf);

	write(client, &res, sizeof(res.fixed));
	write(client, res.buf, res.fixed.buf_size);
	free(res.buf);

}

void send_opendir_response(int client, glu_header * header){

	int32 Status = 0;
	glu_opendir_request req;
	
	get_opendir_request(client, &req);
	
	DIR* d = opendir(req.dir);

	if(d == NULL)
		Status = errno;
	closedir(d);

	glu_log("OPENDIR request - dir: [%s]\n", req.dir);
	glu_log("OPENDIR request - status: [%d]\n", Status);

	send_header(client, GLU_RESPONSE, OPENDIR, Status);

}

void send_readdir_response(int client, glu_header * header){

	glu_readdir_request req;
	glu_readdir_response res;

	get_readdir_request(client, &req);

	DIR* d = opendir(req.dir);
	struct dirent *ep;
	uint16 count = 0;

	if (d != NULL){
    	while (ep = readdir(d))
      		count++;
    	closedir(d);
  	}

  	glu_log("Received readdir request for dir: [%s] !\n", req.dir);

  	send_header(client, GLU_RESPONSE, READDIR, 0);

  	res.fixed.nr_of_entries = count;
  	res.fixed.has_fstatat = req.fixed.has_fstatat;

  	write(client, &res, sizeof(res.fixed));

  	d = opendir(req.dir);
  	uint16 cc = 0;

	if(d != NULL && count != 0){

		struct dirent *entry;
		struct stat st;

		while((entry = readdir(d)) != NULL){
			
			memset(&st, 0, sizeof(st));
			
			uint16 len = (uint16)(strlen(entry->d_name) + 1);
			
			write(client, &len, sizeof(len));

			if(req.fixed.has_fstatat == 1){

				int r = fstatat(dirfd(d), entry->d_name, &st, AT_SYMLINK_NOFOLLOW);

			}

			int len_size = write(client, &st, sizeof(st));
			int w_size = write(client, entry->d_name, len);

			glu_log("[%d][len:%d][len_size:%d][w_size:%d]fname: [%s]\n", cc, len, len_size, w_size, entry->d_name);
			cc++;
		}

	(void) closedir(d);

	}

}

void send_mkdir_response(int client, glu_header * header){

	glu_mkdir_request req;
	get_mkdir_request(client, &req);
	int32 Status = 0;

	int r = mkdir(req.path, req.fixed.mode);
	if(r == -1)
		Status = errno;

	glu_log("MKDIR request - path: [%s]\n", req.path);
	glu_log("MKDIR request - status: [%d]\n", Status);

	send_header(client, GLU_RESPONSE, MKDIR, Status);

}

void send_unlink_response(int client, glu_header * header){

	glu_unlink_request req;
	get_unlink_request(client, &req);
	int32 Status = 0;

	int r = unlink(req.path);
	if(r == -1)
		Status = errno;

	glu_log("UNLINK request - path: [%s]\n", req.path);
	glu_log("UNLINK request - status: [%d]\n", Status);

	send_header(client, GLU_RESPONSE, UNLINK, Status);

}

void send_rmdir_response(int client, glu_header * header){

	glu_rmdir_request req;
	get_rmdir_request(client, &req);
	int32 Status = 0;

	int r = rmdir(req.path);
	
	if(r == -1)
		Status = errno;

	glu_log("RMDIR request - path: [%s]\n", req.path);
	glu_log("RMDIR request - status: [%d]\n", Status);	

	send_header(client, GLU_RESPONSE, RMDIR, Status);

}

void send_rename_response(int client, glu_header * header){

	glu_rename_request req;
	get_rename_request(client, &req);
	int32 Status = 0;

	glu_log("RENAME request - from: [%s]\n", req.from);
	glu_log("RENAME request - to: [%s]\n", req.to);

	int r = rename(req.from, req.to);

	if(r == -1)
		Status = errno;

	send_header(client, GLU_RESPONSE, RENAME, Status);

}

void send_chmod_response(int client, glu_header * header){

	glu_chmod_request req;
	get_chmod_request(client, &req);
	int32 Status = 0;

	glu_log("CHMOD request - path: [%s]\n", req.path);

	int r = chmod(req.path, req.fixed.mode);

	if(r == -1)
		Status = errno;

	send_header(client, GLU_RESPONSE, CHMOD, Status);

}

void send_chown_response(int client, glu_header * header){

	glu_chown_request req;
	get_chown_request(client, &req);
	int32 Status = 0;

	glu_log("CHOWN request - path: [%s]\n", req.path);

	int r = lchown(req.path, req.fixed.uid, req.fixed.gid);

	if(r == -1)
		Status = errno;

	send_header(client, GLU_RESPONSE, CHOWN, Status);
}

void send_truncate_response(int client, glu_header * header){



}

void send_utimens_response(int client, glu_header * header){

	glu_utimens_request req;
	get_utimens_request(client, &req);

	int32 Status = 0;

	int r = utimensat(0, req.path, req.fixed.ts, AT_SYMLINK_NOFOLLOW);
	if (r == -1)
		Status = errno;
	glu_log("UTIMENS request - path: [%s]\n", req.path);
	glu_log("UTIMENS request - status: [%d]\n", Status);

	send_header(client, GLU_RESPONSE, UTIMENS, Status);

}

void send_create_response(int client, glu_header * header){

	glu_create_request req;
	glu_create_response res;

	get_create_request(client, &req);
	res.status = 0;

	send_header(client, GLU_RESPONSE, CREATE, 0);

	int fd = open(req.path, req.fixed.flags, req.fixed.mode);
	
	if(fd == -1)
		res.status = errno;

	glu_log("CREATE REQUEST - file: [%s]\n", req.path);
	glu_log("CREATE REQUEST - status: [%d]\n", res.status);

	close(fd);

	write(client, &res, sizeof(res));

}

void send_open_response(int client, glu_header * header){

	glu_open_request req;
	glu_open_response res;

	get_open_request(client, &req);
	res.status = 0;

	send_header(client, GLU_RESPONSE, OPEN, 0);

	int fd = open(req.path, req.fixed.flags);
	
	if(fd == -1)
		res.status = errno;

	glu_log("OPEN REQUEST - file: [%s]\n", req.path);
	glu_log("OPEN REQUEST - status: [%d]\n", res.status);

	close(fd);

	write(client, &res, sizeof(res));

}

void send_read_response(int client, glu_header * header){

	glu_read_request req;
	glu_read_response res;
	int32 Status = 0;

	get_read_request(client, &req);

	int fd = open(req.path, O_RDONLY);

	if(fd == -1){
		Status = errno;
		glu_log("READ request - failed to open file: [%s]!", req.path);
		send_header(client, GLU_RESPONSE, READ, Status);
		return;

	}

	 //if(req.fixed.size == 0)

	res.buf = (char*)malloc(req.fixed.size);
	size_t r = pread(fd, res.buf, req.fixed.size, req.fixed.offset);
	glu_log("READ - from file [%s] - [%d] bytes!\n", req.path, r);
	close(fd);

	if(r != -1){

		res.fixed.buf_size = r;
		
		send_header(client, GLU_RESPONSE, READ, Status);

		write(client, &res, sizeof(res.fixed));
		
		if(r != 0){
			glu_log("READING reqest - writing: [%d] bytes - offset: [%d]!\n", res.fixed.buf_size, req.fixed.offset);
			write(client, res.buf, res.fixed.buf_size);
		}
		

	}else{
		Status = errno;
		glu_log("READING reqest - ERROR - status: [%d]\n", Status);
		send_header(client, GLU_RESPONSE, READ , Status);

	}
	free(res.buf);
}

void send_write_response(int client, glu_header * header){

	glu_write_request req;
	glu_write_response res;
	int32 Status = 0;

	get_write_request(client, &req);
	glu_log("WRITE request - file:[%s] size[%d] offset[%d]\n", req.path, req.fixed.size, req.fixed.offset);

	int fd = open(req.path, O_WRONLY);
	
	if(fd == -1){
		Status = errno;


		free(req.path);
		free(req.buffer);

		glu_log("WRITE - request : failed open!\n");

		send_header(client, GLU_RESPONSE, WRITE, Status);
		return;
	}

	int r = pwrite(fd, req.buffer, req.fixed.size, req.fixed.offset);
	if(r == -1)
		Status = errno;
	
	glu_log("WRITE - request : status:[%d]!\n", Status);

	send_header(client, GLU_RESPONSE, WRITE, Status);
	
	if(r != -1){
		res.bytes = r;
		write(client, &res, sizeof(glu_write_response));
		glu_log("WRITE - request : bytes:[%d]!\n", r);
	}

	close(fd);
	free(req.path);
	free(req.buffer);

}

void send_flush_response(int client, glu_header * header){



}

void send_release_response(int client, glu_header * header){



}

//GET REQUESTS

void get_getattr_request(int client, glu_getattr_request * req){

	read(client, req, sizeof(req->fixed));
	req->filename = (char*)malloc(req->fixed.filename_size);
	read(client, req->filename, req->fixed.filename_size);

}

void get_readlink_request(int client, glu_readlink_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);

}

void get_opendir_request(int client, glu_opendir_request * req){

	read(client, req, sizeof(req->fixed));
	req->dir = (char*)malloc(req->fixed.dir_size);
	read(client, req->dir, req->fixed.dir_size);

}

void get_readdir_request(int client, glu_readdir_request * req){

	read(client, req, sizeof(req->fixed));
	req->dir = (char*)malloc(req->fixed.dir_size);
	read(client, req->dir, req->fixed.dir_size);

}

void get_mkdir_request(int client, glu_mkdir_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);

}

void get_unlink_request(int client, glu_unlink_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);

}

void get_rmdir_request(int client, glu_rmdir_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);

}

void get_rename_request(int client, glu_rename_request * req){

	read(client, req, sizeof(req->fixed));
	req->from = (char*)malloc(req->fixed.from_size);
	req->to = (char*)malloc(req->fixed.to_size);
	read(client, req->from, req->fixed.from_size);
	read(client, req->to, req->fixed.to_size);

}

void get_read_request(int client, glu_read_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);

}

void get_write_request(int client, glu_write_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	req->buffer = (char*)malloc(req->fixed.size);
	read(client, req->path, req->fixed.path_size);
	read(client, req->buffer, req->fixed.size);

}

void get_create_request(int client, glu_create_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);

}

void get_open_request(int client, glu_open_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);

}

void get_utimens_request(int client, glu_utimens_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);	

}


void get_chmod_request(int client, glu_chmod_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);	

}


void get_chown_request(int client, glu_chown_request * req){

	read(client, req, sizeof(req->fixed));
	req->path = (char*)malloc(req->fixed.path_size);
	read(client, req->path, req->fixed.path_size);	

}