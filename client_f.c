#define FUSE_USE_VERSION 31

#include <fuse.h>
#include "glu_requests.h"

extern int errno;

int port = 2024;
//const char* addr = "192.168.0.105";
const char * addr = "127.0.0.1";
int sd;     // descriptorul de socket

static void *glu_init(struct fuse_conn_info *conn,
                       struct fuse_config *cfg){
         
	//(void) conn;

	cfg->use_ino = 0;
	cfg->nullpath_ok = 0;
	cfg->entry_timeout = 10;
	cfg->attr_timeout = 10;
	cfg->negative_timeout = 5;
	//cfg->kernel_cache = 1;
	//cfg->remember = -1;
	//cfg->direct_io = 0;

	conn->max_background = 0;
	conn->max_readahead = 0;
	conn->max_write = 4096 * 2;
	//conn->max_read = 20000;

	return NULL;

}

static int glu_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
	  (void) fi;

	memset(stbuf, 0, sizeof(struct stat));

	glu_header header;
	glu_getattr_response res;

	send_getattr_request(sd, strlen(path) + 1, path);
	glu_log("Sended getattr request!\n");

	get_getattr_response(sd, &header, &res);
	glu_log("Received getattr response!\n");

	print_header(header);

	if (header.Status != 0)
		return -header.Status;

	memcpy(stbuf, &(res.st), sizeof(struct stat));

	return 0;

	//return -ENOENT;
}

 static int glu_readlink(const char *path, char *buf, size_t size){
	 
 	glu_header header;
 	glu_readlink_response res;

 	send_readlink_request(sd, strlen(path)+1, path, size);
 	glu_log("READLINK request - path: [%s]\n", path);
 	glu_log("READLINK request - size: [%d]\n", size);

 	get_readlink_response(sd, &header, &res);
 	if(header.Status)
 		return -header.Status;

 	glu_log("READLINK request - buf: [%s]\n", res.buf);
 	glu_log("READLINK request - buf_size: [%d]\n", res.fixed.buf_size);

 	memcpy(buf, res.buf, res.fixed.buf_size);

	return 0;

 }

 static int glu_opendir(const char *path, struct fuse_file_info *fi){

	glu_header header;

	send_opendir_request(sd, strlen(path) + 1, path);

	glu_log("OPENDIR request - dir: [%s]\n", path);

	get_opendir_response(sd, &header, NULL);

    if(header.Status)
    	return -header.Status;

    return 0;
 }

static int glu_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
	(void) offset;
	(void) fi;

	int8 has_fstatat = 0;

	glu_header header;
	glu_readdir_response res;

	if(flags & FUSE_READDIR_PLUS){
		
		has_fstatat = 1;
		glu_log("<>READ_FSTATAT!\n");
		
	}	

	send_readdir_request(sd, strlen(path) + 1, path, has_fstatat);
	glu_log("READDIR request for dir: %s!\n", path);

	get_readdir_response(sd, &header, &res);
	glu_log("READDIR response!\n");

	if(!header.Status){

	if(res.fixed.nr_of_entries != 0){

		file_stat *fs = res.fs;
		int count = 0;
		
		while(count != res.fixed.nr_of_entries){

			filler(buf, fs[count].filename, &(fs[count].st), 0, 0);

			count++;
		}

	}

	}

	return 0;
}

 static int glu_mkdir(const char *path, mode_t mode){
    
    glu_header header;

    send_mkdir_request(sd, mode, strlen(path)+1, path);
    glu_log("MKDIR request - path: [%s]\n", path);

    get_mkdir_response(sd, &header, NULL);
    
    if(header.Status)
    	return -header.Status; 

	return 0;
 }

  static int glu_unlink(const char *path){

  	glu_header header;

    send_unlink_request(sd, strlen(path)+1, path);
    glu_log("UNLINK request - path: [%s]\n", path);

    get_unlink_response(sd, &header, NULL);
    
    if(header.Status)
    	return -header.Status; 

	return 0;
 }

  static int glu_rmdir(const char *path){
    
    glu_header header;

    send_rmdir_request(sd, strlen(path)+1, path); 
    glu_log("RMDIR request - path: [%s]\n", path);

    get_rmdir_response(sd, &header, NULL);

    if(header.Status)
    	return -header.Status;

    return 0;

 }

static int glu_rename(const char *from, const char *to, unsigned int flags){

	glu_header header;

	send_rename_request(sd, strlen(from)+1, strlen(to)+1, from, to);

	glu_log("RENAME request - from: [%s]\n", from);
	glu_log("RENAME request - to: [%s]\n", to);

	get_rename_response(sd, &header, NULL);

	if(header.Status)
		return -header.Status;

	return 0;

}


static int glu_chmod(const char *path, mode_t mode,
                      struct fuse_file_info *fi){

  	glu_header header;

  	send_chmod_request(sd, mode, strlen(path)+1, path);

    glu_log("CHMOD request - path: [%s]\n", path);

    get_chmod_response(sd, &header, NULL);

    if(header.Status)
    	return -header.Status;

    return 0;


  }

static int glu_chown(const char *path, uid_t uid, gid_t gid,
                      struct fuse_file_info *fi){

	glu_header header;

	send_chown_request(sd, uid, gid, strlen(path)+1, path);
	glu_log("CHOWN request - path: [%s]\n", path);

	get_chown_response(sd, &header, NULL);

	if(header.Status)
		return - header.Status;

	return 0;

}

static int glu_utimens(const char *path, const struct timespec ts[2],
                        struct fuse_file_info *fi){

 	//should do smth in case ts is null;
 	
 	glu_header header;
 	send_utimens_request(sd, strlen(path) + 1, path, ts);
 	glu_log("UTIMENS request - path[%s]\n", path);

 	get_utimens_response(sd, &header, NULL);

 	if(header.Status)
 		return -header.Status;

    return 0;
 }


 static int glu_create(const char *path, mode_t mode, struct fuse_file_info *fi){
    
    glu_header header;
	glu_create_response res;

	send_create_request(sd, fi->flags, mode, strlen(path) + 1, path);
	glu_log("CREATE REQUEST - file: [%s]\n", path);

	get_create_response(sd, &header, &res);

	if(res.status)
		return -res.status;

	return 0;
 }

static int glu_open(const char *path, struct fuse_file_info *fi) {
  	//(void) fi;

  	glu_header header;
	glu_open_response res;

	send_open_request(sd, fi->flags, strlen(path) + 1, path);
	glu_log("OPEN REQUEST - file: [%s]\n", path);

	get_open_response(sd, &header, &res);

	if(res.status)
		return -res.status;

	return 0;

}

static int glu_read(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi) {
	(void) fi;

	glu_header header;
  	glu_read_response res;

  	if(!path){
  		glu_log("\nREAD - Null path!\n");
  		return -1;
  	}
  	glu_log("READING request - from [%s]\n", path);

	send_read_request(sd, size, offset, strlen(path) + 1, path);

	get_read_response(sd, &header, &res);

	glu_log("READING request - status: [%d]\n", header.Status);

	
	if(!header.Status){

		glu_log("READING reqest - size: [%d]\n", res.fixed.buf_size);

		if(res.fixed.buf_size != 0)
			memcpy(buf, res.buf, res.fixed.buf_size);
		return res.fixed.buf_size;

	}

	return -header.Status;

}

 static int glu_write(const char *path, const char *buf, size_t size,
                      off_t offset, struct fuse_file_info *fi){

 	glu_header header;
 	glu_write_response res;

 	send_write_request(sd, size, offset, strlen(path)+1, path, buf);
 	glu_log("WRITE request - file:[%s] size[%d] offset[%d]\n", path, size, offset);

 	get_write_response(sd, &header, &res);
 	glu_log("WRITE request - status[%d]\n", header.Status);

 	if(header.Status)
 		return -header.Status;

 	return res.bytes;

 }

  static int glu_flush(const char *path, struct fuse_file_info *fi){

  	return 0;

  }

static struct fuse_operations glu_ops = {
	.init           	= glu_init,
	.getattr 			= glu_getattr,
	.readlink 			= glu_readlink,
	.opendir 			= glu_opendir,
	.mkdir 				= glu_mkdir,
	.unlink 			= glu_unlink,
	.rmdir 				= glu_rmdir,
	.rename 			= glu_rename,
	.chmod 				= glu_chmod,
	.chown				= glu_chown,
	.utimens 			= glu_utimens,
	.create    			= glu_create,
	.open 				= glu_open,
	.read 				= glu_read,
	.write 				= glu_write,
	.readdir 			= glu_readdir,
	.flush    			= glu_flush,
};

int main(int argc, char *argv[]){

	//client-side
  struct sockaddr_in server;  // structura folosita pentru conectare 

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
      perror ("Eroare la socket().\n");
      return errno;
  }
  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(addr);
  /* portul de conectare */
  server.sin_port = htons (GLU_PORT);

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1){
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

return fuse_main(argc, argv, &glu_ops, NULL);

}
