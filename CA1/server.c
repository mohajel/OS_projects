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

int offercount = -1;
struct Offer offers[MAX_OFFERS];

//broudcast_info
struct sockaddr_in bc_address;
int auction_fd;

void broudcast(char* output)
{
    int status = sendto(auction_fd, output, strlen(output), 0,
        (struct sockaddr *)&bc_address, sizeof(bc_address));
    if (status == -1)
        perror("cannot broudcast");
}

int setupServer(int port) 
{
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SELLER_IP);;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 4);
    return server_fd;
}

int acceptClient(int server_fd) 
{
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);

    return client_fd;
}

int find_offer_buyer_name(char* buyer_name)
{
    for (int i = 0; i <= offercount; i++)
        if (strcmp(offers[i].buyer_name, buyer_name) == EQUAL)
            return i;  
    return NOT_FOUND;
}

int find_offer_goods_name(char* name)
{
    for (int i = 0; i <= offercount; i++)
        if (strcmp(offers[i].name, name) == EQUAL)
            return i;  
    return NOT_FOUND;
}

int find_offer_in_bargain()
{
    for (int i = 0; i <= offercount; i++)
        if (offers[i].status == BARGAIN)
            return i;  
    return NOT_FOUND;
}

int main(int argc, char const *argv[]) 
{
    if (argc != 2)
    {
        perror("invalid port");
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);
    char name [MAX_NAME_SIZE];
    get_name(name);
    show_server_manual(name, port);
    create_report_file(name);

    auction_fd = create_udp_connection(&bc_address);
    int server_fd = setupServer(port);
    int max_sd = server_fd;
    
    fd_set master_set, working_set;  
    FD_ZERO(&master_set);
    FD_SET(server_fd, &master_set);
    FD_SET(INPUT_FD, &master_set);

    while (1) 
    {
        working_set = master_set;
        select(max_sd + 1, &working_set, NULL, NULL, NULL);

        for (int i = 0; i <= max_sd; i++) 
        {
            if (FD_ISSET(i, &working_set)) 
            {    
                if (i == server_fd) // new clinet
                {  
                    int new_client = acceptClient(server_fd);
                    FD_SET(new_client, &master_set);
                    if (new_client > max_sd)
                        max_sd = new_client;
                    printf("%s***\tNew costumer connected.fd=%d\t***%s\n",KCYN, new_client, KWHT);
                }

                else if (i == INPUT_FD) //input buffer
                {
                    char command[MAX_STRING_SIZE] = {0};
                    int last_char = read(INPUT_FD, command, MAX_STRING_SIZE);
                    command[last_char - 1] = '\0';

                    char* first_arg = strtok(command, " ");
                    char* second_arg = strtok(NULL, " ");

                    if (first_arg == NULL || second_arg == NULL)
                    {
                        printf("%s***\tinvalid input\t****%s\n", KRED, KWHT);
                        show_server_manual(name, port);
                    }
                    else if (strcmp(first_arg, "add") == EQUAL)
                    {
                        int id = find_offer_in_bargain();
                        if (id != NOT_FOUND)
                        {
                            printf("%s***\tHANDLE OFFER IN BARGAIN FIRST\t***%s\n",KCYN,KWHT);
                            printf("%s***\t%s offerd %d$ for %s\t***%s\n",KCYN, offers[id].buyer_name,
                                     offers[id].price, offers[id].name,KWHT);
                            continue;
                        }
                        
                        char output[MAX_STRING_SIZE] = {0};
                        offercount ++;
                        strcpy(offers[offercount].name, second_arg);
                        offers[offercount].status = READY;

                        sprintf(output, "%sESPESIAL OFFER:%s from %s,item: %s,port: %d\n", 
                                    KGRN, KWHT, name, second_arg, port);
                        broudcast(output);
                        continue;       
                    }
                    else if (strcmp(second_arg, "accept") == EQUAL)
                    {
                        int id = find_offer_buyer_name(first_arg);
                        if (id == NOT_FOUND || offers[id].status != BARGAIN)
                        {
                            printf("%s***\tOFFER NOT FOUND\t***%s\n",KRED,KWHT);
                            continue;
                        }
                        if (offers[id].status == BARGAIN)
                        {
                            offers[id].status = SOLD;
                            char output[MAX_STRING_SIZE] = {0};
                            sprintf(output, "%sSOLD:%s %s sold %s to %s. price:%d$\n", 
                                    KRED, KWHT, name, offers[id].name, offers[id].buyer_name, offers[id].price);
                            printf("%s***\toffer accepted\t***%s\n",KCYN,KWHT);
                            broudcast(output);
                            write_to_file(name, output);

                            char accept_note[MAX_STRING_SIZE] = {0};
                            sprintf(accept_note, "%scongrats! %s accpted your offer for %s%s\n",
                                    KGRN, name, offers[id].name, KWHT);
                            send(offers[id].buyer_fd, accept_note, strlen(accept_note), 0);
                        }
                    }
                    else if (strcmp(second_arg, "reject") == EQUAL)
                    {
                        int id = find_offer_buyer_name(first_arg);
                        if (id == NOT_FOUND || offers[id].status != BARGAIN)
                        {
                            printf("%s***\tOFFER NOT FOUND\t***%s\n",KRED,KWHT);
                            continue;
                        }
                        if (offers[id].status == BARGAIN)
                        {
                            offers[id].status = READY;
                            char output[MAX_STRING_SIZE] = {0};
                            char reject_note[MAX_STRING_SIZE] = {0};
                            printf("%s***\toffer rejected\t***%s\n",KCYN,KWHT);
                            sprintf(output, "%sAVAILABLE AGAIN:%s from %s,item: %s,port: %d\n", 
                                    KGRN, KWHT, name, offers[id].name, port);
                            sprintf(reject_note, "%s %s rejected yout offer for %s %s\n",
                                    KRED, name, offers[id].name, KWHT);
                            broudcast(output);
                            send(offers[id].buyer_fd, reject_note, strlen(reject_note), 0);
                        }
                    }
                    else
                    {
                        printf("%s***\tinvalid input\t****%s\n", KRED, KWHT);
                        show_server_manual(name, port);
                    }
                }
                
                else // client sending msg
                { 
                    char massege[MAX_STRING_SIZE] = {0};
                    if (recv(i , massege, MAX_STRING_SIZE, 0) == 0) 
                    { // EOF
                        printf("%s***\tCostumer fd = %d closed\t***%s\n",KCYN, i, KWHT);
                        close(i);
                        FD_CLR(i, &master_set);
                        continue;
                    }
                    char* request = strtok(massege, " ");
                    char* buyer_name = strtok(NULL, " ");
                    char* goods_name = strtok(NULL, " ");
                    char* price_str = strtok(NULL, " ");
                    int price = atoi(price_str);

                    int id = find_offer_goods_name(goods_name);
                    int status;
                    if (id == NOT_FOUND)
                        status = send(i, NOT_EXISTED, strlen(NOT_EXISTED), 0);
                    else if (offers[id].status == SOLD)
                        status = send(i, SOLD_OUT, strlen(SOLD_OUT), 0);
                    else if (offers[id].status == BARGAIN)
                        status = send(i, BARGAIN_MODE, strlen(BARGAIN_MODE), 0);
                    else
                    {
                        offers[id].status = BARGAIN;
                        offers[id].buyer_fd = i;
                        strcpy(offers[id].buyer_name, buyer_name);
                        offers[id].price = price;
                        char output[MAX_STRING_SIZE] = {0};
                        sprintf(output, "%sWENT TO BARGAIN:%s seller: %s,item: %s\n", 
                                KYEL, KWHT, name, offers[id].name);
                        status = send(i, ACCEPTED, strlen(ACCEPTED), 0);
                        broudcast(output);
                    }
                    if (status == -1)
                        printf("%s***\tcant connect to costumer\t***%s",KRED,KWHT);
                }
            }
        }
    }
    return 0;
}