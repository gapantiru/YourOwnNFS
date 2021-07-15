#include "glu_requests.h"

void send_header(int sd, uint8 request, uint8 Command){

	glu_header header;

	memcpy(header.Signature, "GLU", strlen("GLU"));

	header.StructureSize = sizeof(glu_header);
	header.Command = Command;
	header.Status = 0;
	header.Flags = 0;
	header.MID = 0;

	write(sd, &header, sizeof(header));

}

void send_getattr_request(int sd, uint16 filename_size, char* filename){

	glu_getattr_request req;
	send_header(sd, GLU_REQUEST, GETATTR);

	req.filename = filename;
	req.fixed.filename_size = filename_size;

	write(sd, &req, sizeof(req.fixed));
	write(sd, req.filename, req.fixed.filename_size);

}

void send_access_request(int sd, int32 mask, uint16 filename_size, char* filename){

	glu_access_request req;
	send_header(sd, GLU_REQUEST, ACCESS);

	req.filename = filename;
	req.fixed.filename_size = filename_size;

	write(sd, &req, sizeof(req.fixed));
	write(sd, req.filename, req.fixed.filename_size);

}

void send_readlink_request(int sd, uint16 path_size, char* path, size_t size){

	glu_readlink_request req;
	send_header(sd, GLU_REQUEST, READLINK);
	req.fixed.path_size = path_size;
	req.fixed.size = size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));
	write(sd, req.path, req.fixed.path_size);

}

void send_opendir_request(int sd, uint16 dir_size, char* dir){

	glu_opendir_request req;
	send_header(sd, GLU_REQUEST, OPENDIR);

	req.fixed.dir_size = dir_size;
	req.dir = dir;

	write(sd, &req, sizeof(req.fixed));
	write(sd, req.dir, req.fixed.dir_size);
}

void send_readdir_request(int sd, uint16 dir_size, char* dir, int8 has_fstatat){

	glu_readdir_request req;
	send_header(sd, GLU_REQUEST, READDIR);

	req.fixed.dir_size = dir_size;
	req.fixed.has_fstatat = has_fstatat;
	req.dir = dir;

	write(sd, &req, sizeof(req.fixed));
	write(sd, req.dir, req.fixed.dir_size);

}

void send_mkdir_request(int sd, mode_t mode, uint16 path_size, char* path){

	glu_mkdir_request req;
	send_header(sd, GLU_REQUEST, MKDIR);

	req.fixed.mode = mode;
	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);

}

void send_unlink_request(int sd, uint16 path_size, char* path){

	glu_unlink_request req;
	send_header(sd, GLU_REQUEST, UNLINK);

	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);
}

void send_rmdir_request(int sd, uint16 path_size, char* path){

	glu_rmdir_request req;
	send_header(sd, GLU_REQUEST, RMDIR);

	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));
	write(sd, req.path, req.fixed.path_size);

}

void send_rename_request(int sd, uint16 from_size, uint16 to_size, char* from, char* to){

	glu_rename_request req;
	send_header(sd, GLU_REQUEST, RENAME);

	req.fixed.from_size = from_size;
	req.fixed.to_size = to_size;
	req.from = from;
	req.to = to;

	write(sd, &req, sizeof(req.fixed));
	write(sd, req.from, req.fixed.from_size);
	write(sd, req.to, req.fixed.to_size);

}

void send_chmod_request(int sd, mode_t mode, uint16 path_size, char* path){

	glu_chmod_request req;
	send_header(sd, GLU_REQUEST, CHMOD);

	req.fixed.mode = mode;
	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);

}

void send_chown_request(int sd, uid_t uid, gid_t gid, uint16 path_size, char* path){

	glu_chown_request req;
	send_header(sd, GLU_REQUEST, CHOWN);

	req.fixed.uid = uid;
	req.fixed.gid = gid;
	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);


}

void send_truncate_request(int sd, off_t size, uint16 path_size, char* path){

	glu_truncate_request req;
	send_header(sd, GLU_REQUEST, TRUNCATE);

	req.fixed.size = size;
	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);

}

void send_utimens_request(int sd, uint16 path_size, char* path, struct timespec ts[2]){

	glu_utimens_request req;
	send_header(sd, GLU_REQUEST, UTIMENS);

	req.fixed.path_size = path_size;
	memcpy(req.fixed.ts, ts, sizeof(struct timespec)*2);
	req.path = path;

	write(sd, &req, sizeof(req.fixed));
	write(sd, req.path, req.fixed.path_size);

}

void send_create_request(int sd, int32 flags, mode_t mode, uint16 path_size, char* path){

	glu_create_request req;
	send_header(sd, GLU_REQUEST, CREATE);

	req.fixed.flags = flags;
	req.fixed.mode = mode;
	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);

}

void send_open_request(int sd, int32 flags, uint16 path_size, char* path){

	glu_open_request req;
	send_header(sd, GLU_REQUEST, OPEN);

	req.fixed.flags = flags;
	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);

}

void send_read_request(int sd, size_t size, off_t offset, uint16 path_size, char* path){

	glu_read_request req;
	send_header(sd, GLU_REQUEST, READ);

	req.fixed.size = size;
	req.fixed.offset = offset;
	req.fixed.path_size = path_size;

	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);

}


void send_write_request(int sd, size_t size, off_t offset, uint16 path_size, char* path, char* buffer){

	glu_write_request req;
	send_header(sd, GLU_REQUEST, WRITE);

	req.fixed.size = size;
	req.fixed.offset = offset;
	req.fixed.path_size = path_size;

	req.path = path;
	req.buffer = buffer;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);
	write(sd, req.buffer, req.fixed.size);

}

//review this

void send_flush_request(int sd, uint16 path_size, char* path){

	glu_flush_request req;
	send_header(sd, GLU_REQUEST, FLUSH);

	req.fixed.path_size = path_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);

}

void send_release_request(int sd, uint16 pat_size, char* path){

	glu_release_request req;
	send_header(sd, GLU_REQUEST, RELEASE);

	req.fixed.path_size = pat_size;
	req.path = path;

	write(sd, &req, sizeof(req.fixed));

	write(sd, req.path, req.fixed.path_size);

}

//GET RESPONSES

void get_header(int sd, glu_header * header){

	read(sd, header, sizeof(glu_header));

}

void get_getattr_response(int sd, glu_header * header ,glu_getattr_response * res){

	get_header(sd, header);

	if(!header->Status)
		read(sd, res, sizeof(glu_getattr_response));

}

void get_readlink_response(int sd, glu_header * header ,glu_readlink_response * res){

	get_header(sd, header);

	if(header->Status)
		return;

	read(sd, res, sizeof(res->fixed));
	res->buf = (char*)malloc(res->fixed.buf_size);
	read(sd, res->buf, res->fixed.buf_size);


}

void get_opendir_response(int sd, glu_header * header ,glu_opendir_response * res){

	get_header(sd, header);
	//if(!header->Status)

}

void get_readdir_response(int sd, glu_header * header ,glu_readdir_response * res){

	get_header(sd, header);

	if(!header->Status){

		read(sd, res, sizeof(res->fixed));
		
		glu_log("Nr of entries: %d\n", res->fixed.nr_of_entries);

		if(res->fixed.nr_of_entries != 0){

			res->fs = (file_stat*)malloc(res->fixed.nr_of_entries * sizeof(file_stat));
			int count = 0;
			
			while(count != res->fixed.nr_of_entries){

				int len_size = read(sd, &(res->fs[count].filename_size), sizeof(res->fs[count].filename_size));
				int st_size = read(sd, &(res->fs[count].st), sizeof(res->fs[count].st));
				res->fs[count].filename = (char*)malloc(res->fs[count].filename_size);
				int r_size = read(sd, res->fs[count].filename, res->fs[count].filename_size);

				glu_log("[%d][len_size:%d][len:%d][r_size:%d][st_size:%d]fname: [%s]\n", count, len_size, res->fs[count].filename_size, r_size, st_size, res->fs[count].filename);

				count++;
			}

		}
		else{
			res->fs = NULL;
		}

	}

}

void get_mkdir_response(int sd, glu_header * header ,glu_mkdir_response * res){
	
	(void) res;

	get_header(sd, header);	

}

void get_unlink_response(int sd, glu_header * header ,glu_unlink_response * res){

	(void) res;

	get_header(sd, header);	

}

void get_rmdir_response(int sd, glu_header * header ,glu_rmdir_response * res){

	(void) res;

	get_header(sd, header);

}


void get_rename_response(int sd, glu_header * header ,glu_rename_response * res){

    (void) res;

    get_header(sd, header);

}


void get_read_response(int sd, glu_header * header ,glu_read_response * res){

	get_header(sd, header);
	
	if(!header->Status){

		read(sd, res, sizeof(res->fixed));

		if(res->fixed.buf_size != 0){
			
			res->buf = (char*)malloc(res->fixed.buf_size);
			read(sd, res->buf, res->fixed.buf_size);

		}
	
	}

}

void get_write_response(int sd, glu_header * header ,glu_write_response * res){

	get_header(sd, header);

	if(!header->Status){

		read(sd, res, sizeof(glu_write_response));

	}

}

void get_create_response(int sd, glu_header * header ,glu_create_response * res){

	get_header(sd, header);
	read(sd, res, sizeof(res));

}

void get_open_response(int sd, glu_header * header ,glu_open_response * res){

	get_header(sd, header);
	read(sd, res, sizeof(res));

}

void get_utimens_response(int sd, glu_header * header ,glu_utimens_response * res){

	(void) res;

	get_header(sd, header);

}

void get_chmod_response(int sd, glu_header * header ,glu_chmod_response * res){

	(void) res;

	get_header(sd, header);

}

void get_chown_response(int sd, glu_header * header ,glu_chown_response * res){

	(void) res;

	get_header(sd, header);

}