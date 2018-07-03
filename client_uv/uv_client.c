#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<uv.h>
#include<ctype.h>
#include<math.h>
#include<pcap/pcap.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#include"dec.h"

#define PORT 7000

static void on_write_end(uv_write_t *write_req,int status)
{
	if(status < 0)
	{
		fprintf(stderr,"write failed\n");
		free(write_req);
		return;
	}
}


static void readData()
{
        memset(strbuf, 0, sizeof(strbuf));
        memcpy(strbuf, &databuf, sizeof(databuf));
        
	memset(databuf, 0, sizeof(databuf));
	
	uv_buf_t ubuff = uv_buf_init(strbuf, sizeof(strbuf));
        
        fprintf(stdout,"%s",ubuff.base);
//        fprintf(stdout,"len : %lu ",strlen(ubuff.base));

	write_req = (uv_write_t*)malloc(sizeof(uv_write_t));
	int buf_count=1;
        
	uv_write( write_req, tcp, &ubuff, 1, on_write_end);
	uv_fs_read(uv_default_loop(), &read_req, open_req.result, &ubuf, 1, -1, onRead);
}

static void onRead(uv_fs_t *req)
{
        uv_fs_req_cleanup(req);
        if (req->result < 0)
        {
                fprintf(stderr, "error: %s\n", uv_strerror(req->result));
        }
        else if (req->result == 0)
        {
                uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
        }
        else
        {
                readData();
        }
}

static void on_open(uv_fs_t *req)
{
        printf("%s : Begin\n",__func__);
        if(req->result < 0)
        {
                fprintf(stderr, "error: %s\n", uv_strerror(req->result));
        }
        else
        {
                ubuf = uv_buf_init(databuf, sizeof(databuf));
                uv_fs_read(uv_default_loop(), &read_req, req->result, &ubuf, 1, -1, onRead);
        }

        uv_fs_req_cleanup(req);

}


static void connect_client (uv_connect_t* connect, int status)
{
	pcap_t *handle;
        char errbuf[PCAP_ERRBUF_SIZE];

	if(status == -1)
	{
		fprintf(stderr,"connect failed\n");
		free(connect);
		exit(-1);
	}

	tcp = connect->handle;
        int fd =  uv_fs_open(uv_default_loop(),&open_req,"uv_client", O_RDONLY, 0, on_open );
	
}

int main()
{
	struct sockaddr_in *dest;

	uv_loop_t *loop = uv_default_loop();

	uv_socket = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
	if(!uv_socket)
	{
		fprintf(stderr,"socket Malloc failed\n");
		exit(-1);
	}
	uv_tcp_init(loop, uv_socket);

	uv_connect  = (uv_connect_t *)malloc(sizeof(uv_connect_t));
	if(!uv_connect)
	{
		fprintf(stderr,"connect Malloc failed\n");
		exit(-1);
	}
	
        dest = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
        if(!dest)
        {
               fprintf(stderr,"Addr malloc failed\n");
                exit(-1);
        }
	fp = fopen ("myfile.pcap" , "w+");
	int ret = uv_ip4_addr("127.0.0.1", PORT, dest);

	int ret1  = uv_tcp_connect(uv_connect, uv_socket, (const struct sockaddr *)dest,connect_client);
	
	uv_run(loop,UV_RUN_DEFAULT);
}
