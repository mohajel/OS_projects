// in the name of God

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string.h> 
#include <signal.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include<unistd.h>

#include "manual.h"

int server_fd = NOT_CONNECTED;
bool connected_to_server = false;
fd_set master_set, working_set;
char chosen_good[MAX_NAME_SIZE];
char name [MAX_NAME_SIZE];

int connectServer(int port, const char* ip) 
{
    int fd;
    struct sockaddr_in server_address;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(port); 
    server_address.sin_addr.s_addr = inet_addr(ip);

    if (connect(fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        printf("Error in connecting to server\n");  
    return fd;
}

void disconnect_from_server(int signum)
{   
    if (!connected_to_server)
    {
        if (signum == CLIENT_RETURN_SIGNAL)
            printf("%s***\tNOT connected to server\t****%s\n", KRED, KWHT);
        return;
    }
    if (signum != CLIENT_RETURN_SIGNAL)
        printf("%s***\tno response after %ds.disconnected\t***%s\n",
            KCYN, TIME_TO_WAIT_UNTIL_DISCONNECT_FROM_SERVER, KWHT);
    connected_to_server = false;
    FD_CLR(server_fd, &master_set);
    close(server_fd);
    server_fd = NOT_CONNECTED;
    memset(chosen_good, 0, MAX_NAME_SIZE);
    show_client_manual(name);
    alarm(OFF);
}

int main(int argc, char const *argv[]) 
{
    signal(SIGALRM, disconnect_from_server);
    get_name(name);
    show_client_manual(name);
    
    struct sockaddr_in bc_address;
    int auction_fd = create_udp_connection(&bc_address);
    int max_sd = auction_fd;
    FD_ZERO(&master_set);
    FD_SET(auction_fd, &master_set);
    FD_SET(INPUT_FD, &master_set);

    while (1) 
    {
        working_set = master_set;
        select(max_sd + 1, &working_set, NULL, NULL, NULL);
        for (int i = 0; i <= max_sd; i++) 
        {
            if (FD_ISSET(i, &working_set)) 
            {
                if (i == auction_fd)//auction info
                {       
                    if (connected_to_server) //dont show auction info when buying 
                        continue;
                    char info[MAX_STRING_SIZE] = {0};
                    int last_char = recv(auction_fd, info, MAX_STRING_SIZE, 0);
                    info[last_char - 1] = '\0';
                    printf("%s\n", info);                
                }     
                else if (i == INPUT_FD) //input buffer
                {
                    char command[MAX_STRING_SIZE] = {0};
                    int last_char = read(INPUT_FD, command, MAX_STRING_SIZE);
                    command[last_char - 1] = '\0';
                    char* first_arg = strtok(command, " ");

                    if (first_arg == NULL || first_arg[0] == '\n') //avoid crash when hitting \n
                        continue;

                    if (strcmp(first_arg, "return") == EQUAL)
                        disconnect_from_server(CLIENT_RETURN_SIGNAL);
                    else if (strcmp(first_arg, "buy") == EQUAL)
                    {
                        if (connected_to_server)
                        {
                            printf("%s***\tALREADY connected to server\t****%s\n", KRED, KWHT);
                            continue;
                        }
                        char* port = strtok(NULL, " ");
                        char* goods_name = strtok(NULL, " ");
                        if (port == NULL || goods_name == NULL)
                        {
                            printf("%s***\tinvalid input\t****%s\n", KRED, KWHT);
                            continue;
                        }
                        strcpy(chosen_good, goods_name);
                        server_fd = connectServer(atoi(port), SELLER_IP);
                        FD_SET(server_fd, &master_set);
                        if (server_fd > max_sd)
                            max_sd = server_fd;
                        connected_to_server = true;
                        alarm(TIME_TO_WAIT_UNTIL_DISCONNECT_FROM_SERVER);
                    }
                    else if (strcmp(first_arg, "offer") == EQUAL)
                    {
                        if (!connected_to_server)
                        {
                            printf("\n%s***\tNOT connected to buy\t****%s\n", KRED, KWHT);
                            continue;
                        }
                        char* price = strtok(NULL, " ");
                        if (atoi(price) == 0)//handle errors
                        {
                            printf("\n%s***\tPRICE MUST BE NUMBERR!\t****%s\n", KRED, KWHT);
                            continue;
                        } 
                        char output[MAX_STRING_SIZE] = {0};
                        sprintf(output, "request %s %s %s", name, chosen_good ,price);
                        send(server_fd, output, strlen(output), 0);
                        alarm(TIME_TO_WAIT_UNTIL_DISCONNECT_FROM_SERVER);
                    }
                    else
                    {
                        printf("%s***\tinvalid input\t****%s\n", KRED, KWHT);
                        show_client_manual(name);
                    }          
                }
                else if (i == server_fd) //server response
                {
                    char respone[MAX_STRING_SIZE];
                    int last_char = recv(server_fd, respone, MAX_STRING_SIZE, 0);
                    respone[last_char] = '\0';
                    printf("%s",respone);
                    alarm(TIME_TO_WAIT_UNTIL_DISCONNECT_FROM_SERVER);
                }
                else
                    printf("\n%s***\tproblem accurd :cant connect to server\t***%s\n", KRED, KWHT);
            }
        }
    }
    return 0;
}