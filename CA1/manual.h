// in the name of God

#define AUCTION_IP "192.168.21.255"
#define SELLER_IP "127.0.0.1"
#define INPUT_FD 0
#define AUCTION_PORT 8080
#define MAX_STRING_SIZE 1024
#define MAX_NAME_SIZE 256
#define EQUAL 0
#define MAX_OFFERS 50
#define NOT_CONNECTED -3
#define NOT_FOUND -1

//signal
#define CLIENT_RETURN_SIGNAL -1
#define TIME_TO_WAIT_UNTIL_DISCONNECT_FROM_SERVER 60
#define OFF 0

//colors
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//offer status
#define READY 0
#define BARGAIN 1
#define SOLD 2

//responses to client
const char* NOT_EXISTED ="\x1B[31m THIS PRODUCT DOES NOT EXIST \x1B[37m \n";
const char* SOLD_OUT ="\x1B[31m SO SORRY SOLD OUT! \x1B[37m \n";
const char* BARGAIN_MODE ="\x1B[31m SO SORRY ANOTHER COSTUMER HAS ALREADY SENT REQ! \x1B[37m \n";
const char* ACCEPTED ="\x1B[32m YOUR OFFER HAS BEEN SUBMITTED SUCCESSFULLY! \x1B[37m \n";

const char* SERVER_DOC_HEADER = "here you can find list of all your sales:\n\n";

struct Offer
{
    char name[MAX_NAME_SIZE];
    int status;
    int price;
    int buyer_fd;
    char buyer_name[MAX_NAME_SIZE];
};

void create_report_file(const char* name)
{
    char file_name[MAX_STRING_SIZE];
    sprintf(file_name, "%s_sales.txt", name);
    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0642);
    write(fd, SERVER_DOC_HEADER, strlen(SERVER_DOC_HEADER));
    close(fd);
}

void write_to_file(const char* name, const char* str)
{
    char file_name[MAX_STRING_SIZE];
    sprintf(file_name, "%s_sales.txt", name);
    int fd = open(file_name, O_WRONLY| O_APPEND, 0642);
    write(fd, str, strlen(str));
    close(fd);
}

void get_name(char* name)
{
    printf(" --- In the Name of God          --- \n");
    printf(" --- please inter your name :    --- \n");
    int last_char = read(0, name, MAX_STRING_SIZE);
    name[last_char - 1] = '\0';
    printf(" --- hello %s welcome to kalayab \n", name);
}

void show_client_manual(char* name)
{
    printf("%s --- Name : %s\n",KBLU, name);
    printf(" --- role : customer             --- \n");
    printf(" --- command list :              --- \n");
    printf(" --- buy <port> <goods_name>     --- \n");
    printf(" --- offer <price>               --- \n");
    printf(" --- return                      --- %s\n", KWHT);
}

void show_server_manual(char* name, int port)
{
    printf("%s --- Name : %s, port: %d \n", KBLU, name, port);
    printf(" --- role : salesman             --- \n");
    printf(" --- command list :              --- \n");
    printf(" --- add <goods_name>            --- \n");
    printf(" ---<customer_name> accept/reject--- %s\n", KWHT);
}

//returns socket fd
int create_udp_connection(struct sockaddr_in *bc_address)
{
    int sock, broadcast = 1, opt = 1;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    (*bc_address).sin_family = AF_INET; 
    (*bc_address).sin_port = htons(AUCTION_PORT); 
    (*bc_address).sin_addr.s_addr = inet_addr(AUCTION_IP);

    bind(sock, (struct sockaddr *)&(*bc_address), sizeof((*bc_address)));
    return sock;
}




