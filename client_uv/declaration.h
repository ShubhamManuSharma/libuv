#define BUFSIZE 10240
#define PORT 7000
#define BACKLOG  100
#define MAX 10240
static int fd;
typedef struct 
{
  ssize_t nread;
  ssize_t nwritten;
  ssize_t size;
  char *file_path;
  uv_pipe_t *file_pipe;
  uv_stream_t *tcp;

} file_tcp_pipe_t;

static uv_loop_t *loop;
static uv_tcp_t server;
static uv_tcp_t *client;
static uv_fs_t open_req;
static uv_fs_t read_req;
static uv_fs_t close_req;

static uv_buf_t ubuf;
static char strbuf[10240];
static char databuf[10240];

static void onRead(uv_fs_t *);
static pcap_dumper_t *dumpfile;

