#include"headers.h"
#include"declaration.h"

static void on_close_cb(uv_handle_t *handle)
{
	printf("%s : Begin \n",__func__);
	fprintf(stderr,"Closeing..");
	free(handle);
}



static void alloc_mem(uv_handle_t* handle, size_t size, uv_buf_t* buf)    // memory allocate. This allows one to use a slab allocator
{
         buf->base = (char*) malloc(size);
	 buf->len = size;
}

static void on_read_data(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	uv_write_t *write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
	if(nread == -1)
	{
		fprintf(stderr,"error on client read\n");
		uv_close((uv_handle_t *)client, NULL);
		return;
	}
	printf("%s",buf->base);
	
	int ret = write(fd, buf->base, strlen(buf->base));
}


static void server_connection(uv_stream_t *server, int status)
{
	uv_loop_t *loop = uv_default_loop();
	
//	uv_buf_t *buf = malloc(sizeof(uv_buf_t));

	if(status < 0)
	{
		fprintf(stderr,"TCP_LITSEN failed : %s\n",uv_strerror(status));
		exit(-1);
	}
	client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t ));
	
	int r = uv_tcp_init(loop, client);
	r = uv_accept(server,(uv_stream_t *)client);
	if(r)
	{
		fprintf(stderr,"Error accepting: %d\n",errno);
		server->data=NULL;
		uv_close((uv_handle_t*)client, on_close_cb);
		return;
	}
	int ret = uv_read_start((uv_stream_t *)client, alloc_mem, on_read_data);
// 	memset(buf->base, 0, buf->len);
	
//	uv_close((uv_handle_t *)client, NULL);
}

int main()
{
	uv_tcp_t *server;
	struct sockaddr_in *addr=NULL;
	uv_loop_t *loop = uv_default_loop();
	
	server = (uv_tcp_t *)malloc(sizeof(uv_tcp_t ));
	uv_tcp_init(loop, server);
//	fp = fopen ("myfile.pcap" , "w+");                      // open for writing in binary mode
	fd = open("myfile.pcap", O_WRONLY | O_CREAT);
	addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	if(!addr)
	{
		fprintf(stderr,"Addr malloc failed\n");
		exit(-1);
	}
	
	int ret = uv_ip4_addr("127.0.0.1", PORT, addr);

	int r = uv_tcp_bind(server,(const struct sockaddr *) addr, 0);
	
	r = uv_listen((uv_stream_t *)server, BACKLOG, server_connection);          //backlog indicated the size of the connection queue mot the no of connections
	fprintf(stderr, "listening on http://localhost:%d\n", PORT);
	if(r)
	{
		fprintf(stderr,"TCP_LITSEN failed : %s\n",uv_strerror(r));
		exit(-1);
	}
	
	uv_run(loop, UV_RUN_DEFAULT);	
return 0;
}
