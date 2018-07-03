static uv_loop_t *loop;
static uv_tcp_t server;
static uv_tcp_t *uv_socket;
static uv_connect_t *uv_connect;
static uv_tcp_t *handle;
static uv_stream_t *tcp;
typedef struct 
{
  ssize_t nread;
  ssize_t nwritten;
  ssize_t size;
  char *file_path;
  uv_pipe_t *file_pipe;
  uv_stream_t *tcp;

} file_tcp_pipe_t;

typedef struct
{
  uv_write_t req;
  uv_buf_t buf;

} write_req_t;

static FILE *fp;
static void on_req_read(uv_stream_t *, ssize_t , const uv_buf_t *);

static uv_loop_t *loop;
static uv_fs_t open_req;
static uv_fs_t read_req;
static uv_fs_t close_req;
static uv_write_t *write_req;

static uv_buf_t ubuf;
static unsigned char strbuf[1];
static unsigned char databuf[1];
//uv_buf_t* alloc_buffer(uv_handle_t *, ssize_t, uv_buf_t * );
static void onRead(uv_fs_t *);

