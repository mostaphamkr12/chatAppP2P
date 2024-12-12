#define MSG_LEN 1024
#define SERV_PORT "8086"
#define SERV_ADDR "127.0.0.1"
#define MAX_CLIENTS 30
#define MAX_CHANNEL 30 
#define RED_COLOR   "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define BLACK_COLOR "\x1b[30m"
struct info_client{
  int fd;
  int port;
  char adresse[INET_ADDRSTRLEN];
  char nickname[128];
  char channel_name[MSG_LEN];
  struct info_client *next;
};

struct channel{
  char name[1024];
  struct channel *next;
  int nbr_user;
};
