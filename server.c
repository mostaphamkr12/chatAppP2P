#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common.h"
#include "msg_struct.h"
#include <poll.h>
#include <ctype.h>
#include <wchar.h>



char *display(void) {

    char *message = malloc(sizeof(char) * 500);
    snprintf(message, 500,
             "please login with /nick <your pseudo>\n\n"
             "To see how commands work, you can enter, %s/help command%s with 'command' being the command you want to use\n"
             "Command list ===> quit_server | nick | who | whois | msgall | msg | create | channel_list | join | quit\n"
             "If you want information about all commands, enter === > %s/help all%s <===\n"
             "---Example: /help all\n\n",
             RED_COLOR, RESET_COLOR, RED_COLOR, RESET_COLOR);
    return message;
}

void cleanupClientList(struct info_client *clients) { // cleanup
    while (clients != NULL) {
        struct info_client* temp = clients;
        clients = clients->next;
        close(temp->fd);
        free(temp);
    }
}
void cleanupchannel_list(struct channel *channels) {
    while (channels != NULL) {
        struct channel* temp = channels;
        channels = channels->next;
        free(temp);
    }
}


void nick_list(struct info_client *Clients,char *buffer,struct info_client *tmp){ //list of client
  while (Clients!=NULL) {
    if ((strcmp(Clients->nickname,"")!=0)&&(strcmp(Clients->nickname,tmp->nickname)!=0)){
    strcat(buffer,"---->");
    strcat(buffer,Clients->nickname);
    strcat(buffer,"\n");
    }
    Clients=Clients->next;
  }
}

struct info_client *find_skd ( int skd ,struct info_client *Clients){ //search for our socket in our list
  if (Clients->fd==skd){
    return Clients;
  }
  return find_skd(skd,Clients->next);
}

struct info_client *find_nickname_info ( char*nickname,struct info_client *Clients){ // find structur by nickname
 while (Clients!=NULL) {
    if ((strcmp(Clients->nickname,nickname)==0)){
      return Clients;
    }
    Clients=Clients->next;
  }
  return NULL;
}

void removeClient(struct info_client **clients, int clientSocket) {
    struct info_client *current = *clients;
    struct info_client *prev = NULL;

    while (current != NULL) {
        if (current->fd == clientSocket) {
            if (prev == NULL) {
                *clients = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
}

struct channel *find_channel_name(struct channel *channels, char* name) {

    while (channels != NULL) {
     if ((strcmp(channels->name, name) == 0)) {
            return channels;
        }
        channels = channels->next;
    }
    return NULL;
}

void create_channel(struct channel **channels, char *name) {
    struct channel *new_channel = (struct channel *)malloc(sizeof(struct channel));
    memset(new_channel,0,sizeof(struct channel));
    strcpy(new_channel->name, name);
    new_channel->next = NULL;
    new_channel->nbr_user=1;


        if (*channels == NULL) {

            *channels = new_channel;
        } else {
            struct channel *current = *channels;
            while (current->next != NULL) {
                printf("%s\n",current->name);
                current = current->next;

            }
            current->next = new_channel;
        }

}
// Print all channel names
void prinf_channels(struct channel * channels){

        struct channel *temp = channels;
        while (temp != NULL) {
            printf("%s->",temp->name);
            temp = temp->next;
        }
        printf("NULL\n");
    }

void channel_list(char*buff,struct channel *channels){
    if (channels==NULL){
        strcpy(buff,"No channels available");
    }
     while (channels!=NULL) {
    if ((strcmp(channels->name,"")!=0)){
    strcat(buff,"---->");
    strcat(buff,channels->name);
    strcat(buff,"\n");
    }
    channels=channels->next;
  }
}
void multicast_join(char*buff ,struct channel *channels,char*channel_name,struct info_client *clients,int sd,struct message joinstruct){
struct info_client *client =find_skd(sd,clients);
struct channel*salon=find_channel_name(channels,channel_name);
memset(&joinstruct, 0, sizeof(struct message));
if ( strcmp(channel_name,client->channel_name)==0)
{
sprintf(buff,"[server] you are already in [%s]",client->channel_name);
}
else{
if (salon!=NULL){ //check if channel name exist
strcpy(client->channel_name,channel_name);// adding the name of channel
salon->nbr_user++;
sprintf(buff,"[%s] you have joined [%s]",channel_name,client->channel_name);
   char join_msg[5*MSG_LEN];
    sprintf(join_msg, " [%s]: %s has joined [%s]", client->channel_name, client->nickname, client->channel_name);
    joinstruct.pld_len = strlen(join_msg);

    for (struct info_client *client_node = clients; client_node != NULL; client_node = client_node->next) {
        if (strcmp(client_node->channel_name, client->channel_name) == 0 && (client_node->fd != sd)) {
               char join_msg[5*MSG_LEN];
                sprintf(join_msg, " [%s]: %s has joined [%s]", client->channel_name, client->nickname, client->channel_name);
                joinstruct.pld_len = strlen(join_msg);
           if (send(client_node->fd, &joinstruct, sizeof(joinstruct), 0) <= 0) {
    perror("Error broadcasting to client"); 
            }

            if (send(client_node->fd, join_msg, strlen(join_msg), 0) <= 0) {
                perror("Error broadcasting to client");
            }
        }
    }
memset(&joinstruct, 0, sizeof(struct message));
}
else {
   sprintf(buff,"[SERVER] no channel named [%s]",channel_name);
}
}
}

void remove_channel(struct channel **channels, const char *name) {
    struct channel *current = *channels;
    struct channel *prev = NULL;

    while (current != NULL && strcmp(current->name, name) != 0) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev == NULL) {
            *channels = current->next;
        } else {
            prev->next = current->next;
        }

        free(current);
    }
}



// treating messages from clients



void echo_server(int sockfd,struct info_client *clients, struct channel **channels) {
	struct message msgstruct;
	char buff[MSG_LEN];
  struct info_client *clt=NULL;

		memset(&msgstruct, 0, sizeof(struct message));
		memset(buff, 0, MSG_LEN);
		// Receiving structure
		if (recv(sockfd, &msgstruct, sizeof(struct message), 0) <= 0) {
			close(sockfd);
		}
		// Receiving message
		if (recv(sockfd, buff, msgstruct.pld_len, 0) <= 0) {
			close(sockfd);
		}

      struct info_client *echo =find_skd(sockfd,clients);
      if (msgstruct.type==MULTICAST_SEND && strcmp(echo->channel_name,"")==0){
        msgstruct.type=ECHO_SEND;
      }
    printf("pld_len: %i / nick_sender: %s / type: %s / infos: %s\n", msgstruct.pld_len, msgstruct.nick_sender, msg_type_str[msgstruct.type], msgstruct.infos);

switch (msgstruct.type) {
    case NICKNAME_NEW:
    struct info_client *exist=find_nickname_info(buff,clients);
    memset(&msgstruct, 0, sizeof(struct message));
       if ( exist!=NULL){
        memset(buff, 0, MSG_LEN);
        strcpy(buff, "nickname already tocken \n");

       }

        else{

        clt = find_skd(sockfd, clients); // Trouver la structure client correspondant au socket
         if (strcmp(clt->nickname, ""))
            {
            strcpy(clt->nickname, buff);
            sprintf(buff,"name change successfully ,new nick name :[%s]",clt->nickname);
            }
        else {
            strcpy(clt->nickname, buff);
            memset(buff, 0, MSG_LEN);
            strcpy(buff, "Welcome to the chat, ");
            strcat(buff, clt->nickname);
            strcat(buff, "!\n");
        }
        }

         msgstruct.pld_len = strlen(buff);
		// Sending structure (ECHO)

        if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			close(sockfd);
		}

		// Sending message (ECHO)
		if (send(sockfd, buff, strlen(buff), 0) <= 0) {
			close(sockfd);
		}

		printf("Message sent!\n");
        break;

    case NICKNAME_LIST:
        clt = find_skd(sockfd, clients); // Trouver la structure client correspondant au socket
        memset(buff, 0, MSG_LEN);
        memset(&msgstruct, 0, sizeof(struct message));
        strcpy(buff,"clients nicknames are : \n");
        nick_list(clients, buff, clt);
         msgstruct.pld_len = strlen(buff);
		// Sending structure (ECHO)

        if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			close(sockfd);
		}

		// Sending message (ECHO)
		if (send(sockfd, buff, strlen(buff), 0) <= 0) {
			close(sockfd);
		}

		printf("Message sent!\n");
        break;

   case NICKNAME_INFOS:
   clt=find_nickname_info(msgstruct.infos,clients);

        if(clt==NULL){

        memset(buff, 0, MSG_LEN);
        strcpy(buff,"clients not found  \n");


            msgstruct.pld_len = strlen(buff);
            if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			close(sockfd);
		}
		// Sending message (ECHO)
		if (send(sockfd, buff, strlen(buff), 0) <= 0) {
			close(sockfd);
		}

        }
        else{
        memset(buff, 0, MSG_LEN);

        sprintf(buff, "Info for client ,nickname: %s\n", clt->nickname);
        sprintf(buff + strlen(buff), "Adresse: %s\n", clt->adresse);
        sprintf(buff + strlen(buff), "Port: %d\n", clt->port);
         msgstruct.pld_len = strlen(buff);
		// Sending structure (ECHO)

        if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			close(sockfd);
		}

		// Sending message (ECHO)
		if (send(sockfd, buff, strlen(buff), 0) <= 0) {
			close(sockfd);
		}

		printf("Message sent!\n");
        }

   break;
case BROADCAST_SEND:
char broadcast_msg[3*MSG_LEN];

    struct info_client *sender = find_skd(sockfd, clients);
    sprintf(broadcast_msg, "[%s]: %s", sender->nickname, buff);
    if (sender == NULL) {
        break;
    }
    for (struct info_client *client_node = clients; client_node != NULL; client_node = client_node->next) {
        if (client_node->fd != sockfd && client_node->fd != -1){
            struct message broadcast_message;
            broadcast_message.type = BROADCAST_SEND;
            broadcast_message.pld_len = strlen(broadcast_msg);
            strncpy(broadcast_message.nick_sender, sender->nickname, NICK_LEN);
            strncpy(broadcast_message.infos, broadcast_msg, MSG_LEN);
            if (send(client_node->fd, &broadcast_message, sizeof(broadcast_message), 0) <= 0) {
                // Handle errors or disconnections here
                perror("Error broadcasting to client");
                // Handle the error, which could include closing the connection.
            }
            if (send(client_node->fd, broadcast_msg, strlen(broadcast_msg), 0) <= 0) {
			perror("Error broadcasting to client");
            }
    }
    }
    memset(broadcast_msg, 0, MSG_LEN);
    break;


case UNICAST_SEND:

 struct info_client *reciever = find_nickname_info(msgstruct.infos,clients);
struct info_client*src=find_skd(sockfd,clients);
char unicast_msg[3*MSG_LEN];

        memset(&msgstruct, 0, sizeof(struct message));



        if ( reciever == NULL) {
            strcpy(buff,"client doesn't exist");
            msgstruct.pld_len=strlen(buff);

              if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			close(sockfd);
		}
            if (send(sockfd, buff, strlen(buff), 0) <= 0) {
			perror("Error broadcasting to client");
            }
        }
        else{
             sprintf(unicast_msg, "[%s]: %s", src->nickname, buff);

              msgstruct.pld_len=strlen(unicast_msg);

              if (send(reciever->fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			close(reciever->fd);
		}
            if (send(reciever->fd, unicast_msg, strlen(unicast_msg), 0) <= 0) {
			perror("Error broadcasting to client");
            }
        }

    break;

case MULTICAST_CREATE:

    struct channel *find = find_channel_name(*channels, msgstruct.infos);

    if (find != NULL) {
        memset(buff, 0, MSG_LEN);
        memset(&msgstruct, 0, sizeof(struct message));
        strcpy(buff, "channel_name already taken \n");
        msgstruct.pld_len = strlen(buff);

        if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
            close(sockfd);
        }
        if (send(sockfd, buff, strlen(buff), 0) <= 0) {
            perror("Error broadcasting to client");
        }
    } else {


        char msg[2 * MSG_LEN];
        struct info_client *client =find_skd(sockfd,clients);
        if(strcmp(client->channel_name,"")!=0){
            printf("Client channel name: %s\n", client->channel_name);
            printf("case client in channel");

             struct channel *remove = find_channel_name(*channels, client->channel_name);

             remove->nbr_user--;
             if (remove->nbr_user==0){
                remove_channel(channels,client->channel_name);
             }
        }

        strcpy(client->channel_name,msgstruct.infos);
        //strcpy(client->channel_name,msgstruct.infos);
        sprintf(msg,"%s is created",msgstruct.infos);
        create_channel(channels, msgstruct.infos);
        msgstruct.pld_len = strlen(msg);
        msgstruct.type = MULTICAST_JOIN;

        if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
            close(sockfd);
        }
        if (send(sockfd, msg, strlen(msg), 0) <= 0) {
            perror("Error broadcasting to client");
        }
        memset(msg,0,2*MSG_LEN);
    }

    break;


  case MULTICAST_JOIN:
    memset(buff, 0, MSG_LEN);
    struct message joinstruct;
    multicast_join(buff, *channels, msgstruct.infos, clients, sockfd,joinstruct);
    msgstruct.pld_len = strlen(buff);
   // struct info_client *client = find_skd(sockfd, clients);

    // Send the join message to the new client
    if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
        perror("Error broadcasting to client");
    }
    if (send(sockfd, buff, strlen(buff), 0) <= 0) {
        perror("Error broadcasting to client");
    }


    // Broadcast the join message to other clients in the same channel
    /*
   char join_msg[5*MSG_LEN];
    sprintf(join_msg, " [%s]: %s has joined [%s]", client->channel_name, client->nickname, client->channel_name);
    msgstruct.pld_len = strlen(join_msg);

    for (struct info_client *client_node = clients; client_node != NULL; client_node = client_node->next) {
        if (strcmp(client_node->channel_name, client->channel_name) == 0 && (client_node->fd != sockfd)) {
               char join_msg[5*MSG_LEN];
                sprintf(join_msg, " [%s]: %s has joined [%s]", client->channel_name, client->nickname, client->channel_name);
                msgstruct.pld_len = strlen(join_msg);
            if (send(client_node->fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                perror("Error broadcasting to client");
            }
            if (send(client_node->fd, join_msg, strlen(join_msg), 0) <= 0) {
                perror("Error broadcasting to client");
            }
        }
    }
    */

    break;


    case MULTICAST_LIST:
        memset(buff, 0, MSG_LEN);
        channel_list(buff,*channels);
        msgstruct.pld_len = strlen(buff);

        if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
            close(sockfd);
        }
        if (send(sockfd, buff, strlen(buff), 0) <= 0) {
            perror("Error broadcasting to client");
        }
    break;

case MULTICAST_QUIT:
    //memset(buff, 0, MSG_LEN);
        struct info_client *quitting_client = find_skd(sockfd, clients);
        struct channel *salon = find_channel_name(*channels, msgstruct.infos);
        if (strcmp(quitting_client->channel_name, msgstruct.infos) != 0) {
        msgstruct.type = MULTICAST_QUIT; // Set the type
        strcpy(buff, " no channel with this name ,or you have to join the channel");
         msgstruct.pld_len = strlen(buff);


        if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
            perror("Error broadcasting to client");
        }

        if (send(sockfd, buff, strlen(buff), 0) <= 0) {
            perror("Error broadcasting to client");
        }

        } else {
        salon->nbr_user--;

        if (salon->nbr_user == 0) {
        strcpy(quitting_client->channel_name, "");
        remove_channel(channels, msgstruct.infos);
        sprintf(buff, "[server] : You were the last user in this ,%s is destroyed ", msgstruct.infos);
        msgstruct.type = MULTICAST_QUIT; // Set the type

         msgstruct.pld_len = strlen(buff);


        if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
            perror("Error broadcasting to client");
        }

        if (send(sockfd, buff, strlen(buff), 0) <= 0) {
            perror("Error broadcasting to client");
        }

        }
        else{
        sprintf(buff, " [%s]: %s has quit", msgstruct.infos,quitting_client->nickname);
        msgstruct.pld_len = strlen(buff);
        msgstruct.type = MULTICAST_QUIT; // Set the type
        // Broadcast the quit message to other clients in the same channel
        for (struct info_client *client_node = clients; client_node != NULL; client_node = client_node->next) {
            if (strcmp(client_node->channel_name, msgstruct.infos) == 0) {
                if (send(client_node->fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
                if (send(client_node->fd, buff, strlen(buff), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
            }
        }
        strcpy(quitting_client->channel_name, "");


    }

    }

    break;

    case MULTICAST_SEND:

        struct info_client *user = find_skd(sockfd, clients);
        char multicast_msg[3*MSG_LEN];
        sprintf(multicast_msg, " [%s]->[%s]:  %s ", user->channel_name,user->nickname,buff);
        msgstruct.pld_len = strlen(multicast_msg);
        //msgstruct.type = MULTICAST_SEND;
         for (struct info_client *client_node = clients; client_node != NULL; client_node = client_node->next) {
            if (strcmp(client_node->channel_name,user->channel_name) == 0) {
                if (send(client_node->fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
                if (send(client_node->fd, multicast_msg, strlen(multicast_msg), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
            }
        }

    break;
    case FILE_REQUEST:
    struct info_client *recepteur=find_nickname_info(msgstruct.infos,clients);
    struct info_client *emetteur=find_skd(sockfd,clients);

    char request_msg[3*MSG_LEN];

    if (recepteur==NULL){
    sprintf(request_msg, "no client found by this name :[%s]",msgstruct.infos);
      msgstruct.pld_len = strlen(request_msg);
       if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
                if (send(sockfd, request_msg, strlen(request_msg), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
    }
    else{
    sprintf(request_msg,"%s wants you to accept the transfer of the file named %s Do you accept? [Y/N]",emetteur->nickname,buff);
    msgstruct.pld_len = strlen(request_msg);
     msgstruct.type=FILE_REQUEST;
       if (send(recepteur->fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
       if (send(recepteur->fd, request_msg, strlen(request_msg), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
    }
    break;
 case FILE_ACCEPT:
        char accept_msg[3*MSG_LEN];
      //  memset(&msgstruct, 0, sizeof(struct message));
        struct info_client *emet=find_skd(sockfd,clients);
        struct info_client *recep=find_nickname_info(msgstruct.infos,clients);
        sprintf(accept_msg," %s accepted file transfert",emet->nickname);
        memset(&msgstruct, 0, sizeof(struct message));
        msgstruct.pld_len = strlen(accept_msg);
         msgstruct.type=FILE_ACCEPT;
         //printf("dfdfdfdfd%s\n",buff);
        strcpy(msgstruct.infos,buff);
           if (send(recep->fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                        perror("Error broadcasting to client");
                    }
           if (send(recep->fd, accept_msg, strlen(accept_msg), 0) <= 0) {
                        perror("Error broadcasting to client");
                    }
    break;
case FILE_REJECT:

    char reject_msg[3*MSG_LEN];
    struct info_client *emett=find_skd(sockfd,clients);
    struct info_client *rece=find_nickname_info(msgstruct.infos,clients);
    sprintf(reject_msg," %s refused file transfert",emett->nickname);
    msgstruct.pld_len = strlen(reject_msg);
     msgstruct.type=FILE_REJECT;
       if (send(rece->fd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
       if (send(rece->fd, reject_msg, strlen(reject_msg), 0) <= 0) {
                    perror("Error broadcasting to client");
                }
break;


default:
      if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
			close(sockfd);
		}

		// Sending message (ECHO)
		if (send(sockfd, buff, strlen(buff), 0) <= 0) {
			close(sockfd);
		}

break;

}


memset(&msgstruct, 0, sizeof(struct message));
memset(buff, 0, MSG_LEN);
}




int handle_bind(char *Serv_Port) {
	struct addrinfo hints, *result, *rp;
	int sfd;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, Serv_Port, &hints, &result) != 0) {
		perror("getaddrinfo()");
		exit(EXIT_FAILURE);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
		rp->ai_protocol);
		if (sfd == -1) {
			continue;
		}
		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
			break;
		}
		close(sfd);
	}
	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(result);
	return sfd;
}

void addclient(struct info_client **Clients, int sockfd, struct sockaddr_in *info_client) {
    struct info_client *new_client = (struct info_client *)malloc(sizeof(struct info_client));
    new_client->fd = sockfd;
    new_client->port = ntohs(info_client->sin_port);
    strncpy(new_client->adresse, inet_ntoa(info_client->sin_addr), INET_ADDRSTRLEN);

    strcpy(new_client->nickname, "");
      strcpy(new_client->channel_name, "");
    new_client->next = NULL;

    if (*Clients == NULL) {
        *Clients = new_client;
    } else {
        struct info_client *current = *Clients;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_client;
    }
}

int main(int argc, char *argv[]){

	    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_port>\n", argv[0]);
        return 1;
    }
  

  char hostname[256]; // Déclarer un tableau pour stocker le nom de l'hôte
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("Nom de l'hôte : %s\n", hostname);
    } else {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }


	int sfd = handle_bind(argv[1]);
    struct info_client  *clients=NULL;
    struct channel *channels = NULL;
//gerer plusieurs clients
    if (listen(sfd, MAX_CLIENTS) != 0) {
        perror("listen()");
        close(sfd);
        exit(EXIT_FAILURE);
    }

   struct pollfd fds[MAX_CLIENTS];
    fds[0].fd = sfd;
    fds[0].events = POLLIN;

    for (int i = 1; i < MAX_CLIENTS; i++) {
        fds[i].fd = -1;
    }

    while (1) {

        int ret = poll(fds, MAX_CLIENTS, -1);
        if (ret < 0) {
            perror("poll");
            close(sfd);
            exit(EXIT_FAILURE);
        }

        if (fds[0].revents & POLLIN) {

            struct sockaddr_in client;
            socklen_t addr_len = sizeof(client);
            int connfd = accept(sfd, (struct sockaddr *)&client, &addr_len);
            if (connfd < 0) {
                perror("accept");
                close(sfd);
                exit(EXIT_FAILURE);
            }
            

        char *welcom= display();
        struct message structure;
        structure.pld_len=strlen(welcom);
           if (send(connfd, &structure, sizeof(structure), 0) <= 0) {
			close(connfd);
		}


          if (send(connfd, welcom, strlen(welcom), 0) <= 0) {
			close(connfd);
		}
            int i;
                    for (i = 1; i < MAX_CLIENTS ; i++) {
                        if (fds[i].fd == -1) {
                            fds[i].fd = connfd;
                            fds[i].events = POLLIN;
                            break;
                        }
                    }

            addclient(&clients,fds[i].fd ,&client);

            if (i== MAX_CLIENTS) {
                fprintf(stderr, "Too many clients. Connection rejected.\n");
                close(connfd);
            }
        }

    for (int i = 1; i < MAX_CLIENTS; i++) {
    if (fds[i].revents & (POLLHUP | POLLERR)) {
        close(fds[i].fd);
        // Remove the client connected clients
        removeClient(&clients, fds[i].fd); // Implement the removeClient function
        // Mark the entry in the poll array as unused
        fds[i].fd = -1;
    } else if (fds[i].fd != -1 && (fds[i].revents & POLLIN)) {
        echo_server(fds[i].fd, clients,&channels);
        fds[i].revents = 0;
    }
}
    }

cleanupchannel_list(channels);
cleanupClientList(clients);
close(sfd);
}
