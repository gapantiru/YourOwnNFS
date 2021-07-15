
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

extern int errno;

int port = 2024;
const char* addr = "127.0.0.1";
int sd;     // descriptorul de socket
char msg[100];


static const char *filepath = "/dummy_file";
static const char *filename = "dummy_file";
static const char *filecontent = "This is a test\n";

static int getattr_callback(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
  memset(stbuf, 0, sizeof(struct stat));

  if (strcmp(path, "/") == 0) {
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 2;
    return 0;
  }

  if (strcmp(path, filepath) == 0) {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    stbuf->st_size = strlen(filecontent);
    return 0;
  }

  return -ENOENT;
}

static int readdir_callback(const char *path, void *buf, fuse_fill_dir_t filler,
    off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
  (void) offset;
  (void) fi;
  printf("---PATH: [%s]\n", path);
  filler(buf, ".", NULL, 0, 0);
  filler(buf, "..", NULL, 0, 0);

  filler(buf, filename, NULL, 0, 0);

  return 0;
}

static int open_callback(const char *path, struct fuse_file_info *fi) {
  return 0;
}

static int read_callback(const char *path, char *buf, size_t size, off_t offset,
    struct fuse_file_info *fi) {

  if (strcmp(path, filepath) == 0) {
    size_t len = strlen(filecontent);
    if (offset >= len) {
      return 0;
    }
    printf("read_callback\n");
    if (offset + size > len) {
      memcpy(buf, filecontent + offset, len - offset);
      return len - offset;
    }

    memcpy(buf, filecontent + offset, size);
    return size;
  }

  return -ENOENT;
}

static struct fuse_operations fuse_ops = {
  .getattr = getattr_callback,
  .open = open_callback,
  .read = read_callback,
  .readdir = readdir_callback,
};

int main(int argc, char *argv[])
{

	//client-side
  struct sockaddr_in server;  // structura folosita pentru conectare 
  char msg[100];    // mesajul trimis

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
  server.sin_port = htons (port);

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1){
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  return fuse_main(argc, argv, &fuse_ops, NULL);
}
