all:
	gcc -Wall -O3 glu_requests.c client_f.c `pkg-config fuse3 --cflags --libs` -o client
	gcc -O3 glu_responses.c server.c -o server
clean:
	rm -f *~client server
