#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include "common.h"
#include "msg_struct.h"
#include <time.h>
#include <ctype.h>
#include <wchar.h>

#define RED_COLOR   "\x1b[31m"
#define RESET_COLOR "\x1b[0m"
#define BLACK_COLOR "\x1b[30m"


//complément fonction custom_random
void name_disp(){
  printf("Enter choice by writting\n");
  fprintf(stdout, "---%sjapanese%s\n",RED_COLOR,RESET_COLOR );
  fprintf(stdout, "---%seuropean%s\n",RED_COLOR,RESET_COLOR  );
  fprintf(stdout, "---%schinese%s\n",RED_COLOR,RESET_COLOR  );
  fprintf(stdout, "---%safrican%s\n",RED_COLOR,RESET_COLOR  );
  fprintf(stdout, "---%sdefault%s\n",RED_COLOR,RESET_COLOR  );

}

//Complément fonction help
void disp(){
  fprintf(stdout,"\n");
  for (int i = 0; i < 204; i++) {
    fprintf(stdout,"%s-%s",RED_COLOR,RESET_COLOR );
  }
  fprintf(stdout,"\n");
}

//Help fonction pour les informations d'utilisation des fonctions
void help(char buff[]) {
    char message[MSG_LEN];
    strcpy(message, buff);
    int select = 0;

    char *espace = strchr(message, ' ');

    if (espace != NULL) {
        char *newline = strchr(espace, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }

      char *help = espace + 1;

      if (strcmp(help, "all") == 0) {
          select = -1;
      } else if (strcmp(help, "nick") == 0) {
          select = 1;
      } else if (strcmp(help, "who") == 0) {
          select = 2;
      } else if (strcmp(help, "whois") == 0) {
          select = 3;
      } else if (strcmp(help, "msgall") == 0) {
          select = 4;
      } else if (strcmp(help, "msg") == 0) {
          select = 5;
      } else if (strcmp(help, "create") == 0) {
          select = 6;
      } else if (strcmp(help, "channel_list") == 0) {
          select = 7;
      } else if (strcmp(help, "join") == 0) {
          select = 8;
      } else if (strcmp(help, "quit") == 0) {
          select = 9;
      } else if (strcmp(help, "quit_server") == 0) {
          select = 10;
      } else {
          select = 11;
      }

        if (select == -1 || select == 1) {
          fprintf(stdout, "The command %s/nick%s allow you to create yourselfe a nickname, enter ===> %s/nick pseudo%s <=== with pseudo as the name you want to be named\n",RED_COLOR, RESET_COLOR, RED_COLOR, RESET_COLOR);
          fprintf(stdout, "---Exemple /nick toto\n");
          fprintf(stdout, "If you want to have a name generated randomly, enter ===> %s/nick random%s <===\n", RED_COLOR, RESET_COLOR);
          fprintf(stdout, "---Exemple: /nick random\n");
          disp();
        }
        if (select == -1 || select == 2) {
          fprintf(stdout, "The command %s/who%s allow you to see all actual members online, to use this command, enter ===> %s/who%s <===\n",RED_COLOR, RESET_COLOR,  RED_COLOR, RESET_COLOR);
          fprintf(stdout, "---Exemple: /who\n");
          disp();
        }
        if (select == -1 || select == 3) {
          fprintf(stdout, "The command %s/whois%s allows you to see the details of an online member, such as their IP address and their connection socket, to use this command, enter ===> %s/whois pseudo%s <=== with pseudo the name you want to have the information about \n",RED_COLOR, RESET_COLOR,RED_COLOR, RESET_COLOR );
          fprintf(stdout, "---Exemple: /whois toto\n");
          disp();
        }
        if (select == -1 || select == 4) {
          fprintf(stdout, "The command %s/msgall%s allow you to send messages in BROADCAST, which means that your message will be send to all members online, to use this command, enter ===> %s/msgall your_message%s <===\n",RED_COLOR, RESET_COLOR,RED_COLOR, RESET_COLOR);
          fprintf(stdout, "---Exemple: /msgall hey what's up everybody !!!\n");
          disp();
        }
        if (select == -1 || select == 5) {
          fprintf(stdout, "The command %s/msg%s allow you to send private messages to someone, to use this command, enter ===> %s/msg pseudo your_message%s <=== with pseudo the person name who whom you want to send the message and your_message the message you want to send\n",RED_COLOR, RESET_COLOR,RED_COLOR, RESET_COLOR );
          fprintf(stdout, "---Exemple: /msg toto hey man what's up!!!\n");
          disp();
        }
        if (select == -1 || select == 6) {
          fprintf(stdout, "The command %s/create%s allow you to create a channel, to use this command, enter ===> %s/create channel_name%s <=== with channel_name the name you want to name the channel\n",RED_COLOR, RESET_COLOR,RED_COLOR, RESET_COLOR);
          fprintf(stdout, "---Exemple: /create ENSEIRB\n");
          fprintf(stdout, "---Use /create when you are inside a channel will kick you out of it and put you in the new channel created\n");
          disp();
        }
        if (select == -1 || select == 7) {
          fprintf(stdout, "The command %s/channel_list%s allow you to see all joinable channels, to use this command, enter ===> %s/channel_list%s <===\n",RED_COLOR, RESET_COLOR, RED_COLOR, RESET_COLOR);
          fprintf(stdout, "---Exemple: /channel_list\n");
          disp();
        }
        if (select == -1 || select == 8) {
          fprintf(stdout, "The command %s/join%s allow you to join a channel, to use this command, enter ===> %s/join channel_name%s <=== with channel_name the name of the channel you want to join\n",RED_COLOR, RESET_COLOR,RED_COLOR, RESET_COLOR );
          fprintf(stdout, "---Exemple: /join ENSEIRB\n");
          fprintf(stdout, "---You can only be in one channel\n");
          disp();
        }
        if (select == -1 || select == 9) {
          fprintf(stdout, "The command %s/quit%s allow you to quit the channel you are into, to use this command, enter ===> %s/quit%s <===\n",RED_COLOR, RESET_COLOR, RED_COLOR, RESET_COLOR );
          fprintf(stdout, "---Exemple: /quit\n");
          disp();
        }
        if (select == -1 || select == 10) {
          fprintf(stdout, "The command %s/quit_server%s disconnet you of the website, to use this command, enter ===> %s/quit_server%s <=== \n",RED_COLOR, RESET_COLOR,RED_COLOR, RESET_COLOR);
          fprintf(stdout, "---Exemple: /quit_server\n");
          disp();
        }
        if (select == 11) {
          fprintf(stdout, "Sorry the command %s[%s]%s doesn not exist.\n",RED_COLOR, help,RESET_COLOR );
          disp();
        }
    }
}

//Génération d'un speudo aléatoire
void custom_random(char *str, char buff[]) {

  memset(buff, 0, NICK_LEN);
    memset(str, 0, NICK_LEN);

    char** syllables = NULL;
    char choice[20];
    int select = 0;
    int num_syllables = 0;

    srand(time(NULL));
    name_disp();
    ap:
    scanf("%s", choice);

    if (strcmp(choice, "european") == 0) {
        select = 1;
        num_syllables = 5;
    } else if (strcmp(choice, "japanese") == 0) {
        select = 2;
        num_syllables = 5;
    } else if (strcmp(choice, "chinese") == 0) {
      select = 3;
      num_syllables = 5;
    } else if (strcmp(choice, "african") == 0) {
      select = 4;
      num_syllables = 5;
    } else if (strcmp(choice, "default") == 0) {
      select = 5;
      num_syllables = 5;
    } else {
      fprintf(stdout, "Syllables for %s are unavailable\n", choice);
      goto ap;
    }

    syllables = malloc(num_syllables * sizeof(char *));
    for (int i = 0; i < num_syllables; i++) {
        syllables[i] = malloc(20 * sizeof(char));
    }

    if (select == 1) {
        strcpy(syllables[0], "a");
        strcpy(syllables[1], "i");
        strcpy(syllables[2], "u");
        strcpy(syllables[3], "e");
        strcpy(syllables[4], "o");
    } else if (select == 2) {
        strcpy(syllables[0], "ka");
        strcpy(syllables[1], "ki");
        strcpy(syllables[2], "ku");
        strcpy(syllables[3], "ke");
        strcpy(syllables[4], "ko");
    } else if (select == 3) {
        strcpy(syllables[0], "mi");
        strcpy(syllables[1], "na");
        strcpy(syllables[2], "no");
        strcpy(syllables[3], "sha");
        strcpy(syllables[4], "pi");
    } else if (select == 4) {
        strcpy(syllables[0], "na");
        strcpy(syllables[1], "ma");
        strcpy(syllables[2], "fi");
        strcpy(syllables[3], "ko");
        strcpy(syllables[4], "ada");
    } else if (select == 5) {
        strcpy(syllables[0], "ba");
        strcpy(syllables[1], "an");
        strcpy(syllables[2], "mo");
        strcpy(syllables[3], "ij");
        strcpy(syllables[4], "nk");
    }

    int random_number = rand() % 11;
    char new_name[NICK_LEN];

    new_name[0] = '\0';

    for (int i = 0; i < random_number; i++) {
        int idx = rand() % num_syllables;
        strcat(new_name, syllables[idx]);
    }

    strncpy(str, new_name, NICK_LEN - 1);
    for (int i = 0; i < num_syllables; i++) {
        free(syllables[i]);
    }
    free(syllables);
}


















int create_server_socket() {// celui qui envoie file
    int sfd;
    struct sockaddr_in server_addr;
    int opt = 1;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(SERV_PORT));

    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind ici wesh");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, SOMAXCONN) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return sfd;

}

int handle_connect_peer() { //celui qui ecrit yes
    struct addrinfo hints, *result, *rp;
    int sfd;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(SERV_ADDR, SERV_PORT, &hints, &result) != 0) {
        perror("getaddrinfo()");
        exit(EXIT_FAILURE);
    }
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;
        }
        close(sfd);
    }
    if (rp == NULL) {
        fprintf(stderr, "Could not connect to peer server\n");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(result);

    printf("Connecting to peer... Done\n");
    return sfd;
}
































//Définir le nom d'utilisateur après les commandes
void user(char *str, char buff[]) {
    char *token = strtok(buff, "\n");

    if (token != NULL && token[0] == '/') {
        token = strtok(token, " ");
        if (token != NULL) {
            memset(str, 0, strlen(str));
            char *addr = strtok(NULL, " ");

            if (addr == NULL) {
                fprintf(stderr, "Error: Argument after command missing\n");
            } else {
                if (strcmp(addr, "random") == 0) {
                    memset(buff, 0, strlen(buff));
                    strncpy(buff, addr, strlen(addr));
                    custom_random(str, buff);
                } else {
                    strncpy(str, addr, strlen(addr));
                }
            }
        } else {
            fprintf(stderr, "Error: Command missing\n");
        }
    } else {
        fprintf(stderr, "Error: Invalid input format\n");
    }
}

//Définiton des commandes
enum msg_type command(char *buff) {

  if (buff[0]=='/'){

    char caractere[MSG_LEN];
    strcpy(caractere,buff);
    char *jonction="\n "; // \n et ' ' comme délimiteur
    char *command=strtok(caractere,jonction);

    if (strcmp(command,"/quit_server") == 0 ){
        return QUIT;
    }
    if (strcmp(command,"/nick") == 0 ){
        return NICKNAME_NEW;
    }
    if (strcmp(command,"/who") == 0 ){
        return NICKNAME_LIST;
    }
    if (strcmp(command,"/whois") == 0 ){
        return NICKNAME_INFOS;
    }
    if (strcmp(command,"/msgall") == 0 ){
        return BROADCAST_SEND;
    }
    if (strcmp(command,"/msg") == 0 ){
        return UNICAST_SEND;
    }
    if (strcmp(command,"/create") == 0 ){
        return MULTICAST_CREATE;
    }
    if (strcmp(command,"/channel_list") == 0 ){
        return MULTICAST_LIST;
    }
    if (strcmp(command,"/join") == 0 ){
        return MULTICAST_JOIN;
    }
    if (strcmp(command,"/quit") == 0 ){
        return MULTICAST_QUIT;
    }
    if (strcmp(command,"/send") == 0 ){
        return FILE_REQUEST;
    }
    if (strcmp(command,"/Y") == 0 ){
        return FILE_ACCEPT;
    }
    if (strcmp(command,"/N") == 0 ){
        return FILE_REJECT;
    }
    if (strcmp(command,"/help") == 0 ){
        help(buff);
    }
    else {
      fprintf(stdout, "command %s unknown\n",command );
      return ECHO_SEND;
    }
  }

  if (buff[0]!='/'){
    return MULTICAST_SEND;
  }


    return ECHO_SEND; //Si aucune commande ne correspond à une commande
  }


//Traitement accepte ou refuse demande de transfer de fichiers
void cnt_ask(char dsc[15]) {
    char choice[15];

    ask_again:
    memset(choice, 0, sizeof(choice));
    scanf(" %1[^\n]", choice);

    if (strcmp(choice, "Y") == 0) {
        strcpy(dsc, "/Y");
    } else if (strcmp(choice, "N") == 0) {
        strcpy(dsc, "/N");
    } else {
        printf("No valid characters accepted.\n");
        goto ask_again;
    }
}

//Renvoie le speudo de l'utilisateur qui veux envoyé un fichier
void ack_user(char buff[], char *info, char *file) {


    if (sscanf(buff, "%s wants you to accept the transfer of the file named %s", info, file) != 2) {
        fprintf(stdout, "Impossible de lire les informations correctement\n");
    }
}

//Traitement des données de messages type msgall/msg à envoyer dans le buffer
void to_send(int select, char *buff) {

    char message[MSG_LEN];
    strcpy(message, buff);

    char *espace = strchr(message, ' ');//premier espace

     if (select == 1) {
       char *sous_chaine = espace + 1;
       sous_chaine[strlen(sous_chaine)] = '\0';
       memset(buff, 0, MSG_LEN);
       strncpy(buff, sous_chaine, MSG_LEN);

     } else if(select == 2) {
       espace = strchr(espace + 1, ' '); //deuxième espace
       char *sous_chaine = espace + 1;
       sous_chaine[strlen(sous_chaine)] = '\0';
       memset(buff, 0, MSG_LEN);
       strncpy(buff, sous_chaine, MSG_LEN);
     }
}

//supprime les doublons pour avoir juste 1 fois le mauvais caractere afficher
void removeDuplicates(char doublon[]) {

    int len = strlen(doublon);
    char unique[len];

    int len_unique = 0;

    for (int i = 0; i < len; i++) {
        int isDuplicate = 0;

        for (int j = 0; j < len_unique; j++) {
            if (doublon[i] == unique[j]) {
                isDuplicate = 1;
                break;
            }
        }

        if (!isDuplicate) {
            unique[len_unique] = doublon[i];
            len_unique++;
        }
    }

    unique[len_unique] = '\0';
    fprintf(stdout, "Wrong characters are ===>[ %s ]\n", unique);
}


void set(char *str,char buff[], int select) {

  memset(str, 0, NICK_LEN);
  int len = strlen(buff);
  char wrong[len];
  memset(wrong, 0, len);
  int j = 0, k = 0;
  char setBuff[len];
  memset(setBuff, 0, len);
  strcpy(setBuff, buff);

  for (int i = 0; i < len; i++) {
      if (!isdigit(setBuff[i]) && !isalpha(setBuff[i])) {
          wrong[j] = setBuff[i];
          j++;
      } else {
          setBuff[k] = setBuff[i];
          k++;
      }
  }
  len = k;
  setBuff[len] = '\0';

    if (select == 2) {

    if (strcmp(setBuff, buff) != 0) {
        strncpy(str, setBuff, len);
        fprintf(stdout, "channel name has been changed into %s\n", setBuff);
        removeDuplicates(wrong);
    } else {
      strncpy(str, buff, strlen(buff));
      memset(buff,0,NICK_LEN);
    }
  }

    if (select == 1) {

    if (strlen(setBuff) > NICK_LEN) {
      fprintf(stdout, "Nick lenght must no exeed %d caracteres\n",NICK_LEN );
      //echo_client();
    }
    if (strcmp(setBuff, buff) != 0) {
        strncpy(str, setBuff, len);
        fprintf(stdout, "your name has been changed into %s\n", setBuff);
        removeDuplicates(wrong);
    } else {
      strncpy(str, buff, strlen(buff));
      memset(buff,0,NICK_LEN);
    }
  }
}

//Traitement des informations
void infos(struct message *msgstruct, char buff[]) {

 char str[NICK_LEN];
  memset(str,0,NICK_LEN);
  char channel[NICK_LEN];
  char channel_set[NICK_LEN];
  char pseudo[NICK_LEN];
  char pseudo_set[NICK_LEN];
  char sender[NICK_LEN];
  char file[INFOS_LEN];
  memset(sender, 0, NICK_LEN);
  memset(file, 0, INFOS_LEN);
  int select = -1;
  switch (msgstruct -> type) {
    case (QUIT):
    fprintf(stdout, "Disconnection\n" );
    exit(0);
    break;

    case (NICKNAME_NEW):
    select = 1;
    strncpy(pseudo, buff, strlen(buff));
    user(str,pseudo);
    set(pseudo_set,str,select);
    if(strcmp(msgstruct->nick_sender,"") == 0 ) {
        strncpy(msgstruct->infos, pseudo_set, strlen(pseudo_set));
        memset(buff, 0, MSG_LEN);
        strncpy(buff, pseudo_set, MSG_LEN);
      } else {
          strcpy(msgstruct->infos,msgstruct->nick_sender);
      }
    break;

    case (NICKNAME_LIST):
          memset(msgstruct->infos,0,INFOS_LEN);
    break;

    case (NICKNAME_INFOS):
          strncpy(pseudo, buff, strlen(buff));
          user(str,pseudo);
          strncpy(msgstruct->infos, str, strlen(str));
          memset(buff, 0, MSG_LEN);
          strncpy(buff, str, MSG_LEN);
    break;

    case (BROADCAST_SEND):
          memset(msgstruct->infos,0,INFOS_LEN);
          select = 1;
          to_send(select,buff);//Envoie du message
    break;

    case (UNICAST_SEND):
          strncpy(pseudo, buff, strlen(buff));
          user(str,pseudo);
          strncpy(msgstruct->infos, str, strlen(str));
          select = 2;
          to_send(select,buff);
    break;

    case (MULTICAST_CREATE):
          strncpy(channel, buff, strlen(buff));
          user(str,channel);
          select = 2;
          set(channel_set,str,select);
          strncpy(msgstruct->infos, channel_set, strlen(channel_set));
          memset(buff, 0, MSG_LEN);
          strncpy(buff, channel_set, MSG_LEN);

    break;

    case (MULTICAST_LIST):
          memset(msgstruct->infos,0,INFOS_LEN);
          strcpy(buff,"\n");
    break;

    case (MULTICAST_JOIN):

          strncpy(channel, buff, strlen(buff));
          user(str,channel);
          strncpy(msgstruct->infos, str, strlen(str));
          memset(buff, 0, MSG_LEN);
          strncpy(buff, str, MSG_LEN);
    break;

    case (MULTICAST_QUIT):
          strncpy(channel, buff, strlen(buff));
          user(str,channel);
          strncpy(msgstruct->infos, str, strlen(str));
          memset(buff, 0, MSG_LEN);
          strncpy(buff, str, MSG_LEN);
    break;

    case (FILE_REQUEST):
          strncpy(pseudo, buff, strlen(buff));
          user(str,pseudo);
          strncpy(msgstruct->infos, str, strlen(str));
          select = 2;
          to_send(select,buff);
    break;

    case (FILE_ACCEPT):
       strncpy(str, buff, strlen(buff));
        ack_user(str,sender,file);
        strcpy(msgstruct->infos, sender);
        strcpy(buff, file);
    break;

    case (FILE_REJECT):

    break;

    default:
          memset(msgstruct->infos,0,INFOS_LEN);
    break;
  }

}

void echo_client(int sockfd) {

        struct message msgstruct;
        char buff[MSG_LEN];
        int n;
        int pl = 0;

        struct pollfd fds[3];

  fds[0].fd = 0; // stdin (clavier)
  fds[0].events = POLLIN; // Surveille la disponibilité de lecture
  fds[1].fd = sockfd; // Descripteur de dossier de la socket
  fds[1].events = POLLIN; // Surveille la disponibilité de lecture
  fds[2].fd=-1;
        fds[2].events=POLLIN;
        fds[2].revents=0;

        while (1) {

                int ret = poll(fds, 2, -1); // 2 indique les 2 descripteurs de fichiers et -1 est l'erreur retournée
                        if (ret == -1) {
                        perror("poll");
                        close(sockfd);
                        break;
                        }

                                      if (fds[0].revents & POLLIN) {

                                      memset(&msgstruct, 0, sizeof(struct message));
                                      memset(buff, 0, MSG_LEN);

                                      retry:

                                      n = 0;
                                      while ((buff[n++] = getchar()) != '\n') {}

                                      msgstruct.type = command(buff);
                                      infos(&msgstruct,buff);
                                      msgstruct.pld_len = strlen(buff);//définit la taille de buff dans la variable pld_len de la structure message

                                      // Sending structure
                                      if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                                              break;
                                      }
                                      // Sending message of pld_len
                                      if (send(sockfd, buff, msgstruct.pld_len, 0) <= 0) {
                                              break;
                                      }
                              }
//recevoir
                                    if (fds[1].revents & POLLIN) {
                                              // Cleaning memory
                                              memset(&msgstruct, 0, sizeof(struct message));
                                              memset(buff, 0, MSG_LEN);
                                              char ask_send[] = "wants you to accept the transfer of the file named";

                                              // Receiving structure
                                              if (recv(sockfd, &msgstruct, sizeof(struct message), 0) <= 0) {
                                                      break;
                                              }
                                              // Receiving message
                                              if (recv(sockfd, buff, msgstruct.pld_len, 0) <= 0) {
                                                      break;
                                              }



                                     fprintf(stdout, "%s\n",buff );
                                     char *result = strstr(buff, ask_send);

                                    if (strcmp(buff,"nickname already token") == 0) {
                                      goto retry;
                                    }







                                          if (msgstruct.type==FILE_ACCEPT){

                                            int sf=handle_connect_peer();
                                            FILE *file_in;
                                            //char file_to_send[] = "send.txt";
                                            char characters[10*MSG_LEN];
                                            int i = 0;

                                            //Ouvre le fichier en lecture
                                            file_in = fopen(msgstruct.infos, "r");
                                            if (file_in == NULL) {
                                                printf("Impossible d'ouvrir le fichier %s.\n", msgstruct.infos);
                                            }

                                            //Lire les caracteres du fichier
                                            while (fgets(characters, 10 * MSG_LEN, file_in) != NULL) {
                                                for (i = 0; characters[i] != '\0'; i++) {
                                                  pl++;
                                                }
                                            }

                                          fclose(file_in);
                                          char characters_array[pl];
                                          fprintf(stdout, "pl : %d\n",pl );

                                          file_in = fopen(msgstruct.infos, "r");
                                              if (file_in == NULL) {
                                                  printf("Impossible d'ouvrir à nouveau le fichier %s.\n", msgstruct.infos);
                                              }

                                              i = 0;
                                          while (fgets(characters, 10*MSG_LEN, file_in) != NULL) {
                                              for (int j = 0; characters[j] != '\0'; j++) {
                                               characters_array[i++] = characters[j];
                                             }
                                            }

                                       fclose(file_in);

                                            msgstruct.type=FILE_SEND;
                                            msgstruct.pld_len = strlen(characters_array);
                                            msgstruct.file_size=pl;

                                            if (send(sf, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                                                break;
                                            }
                                            if (send(sf, characters_array, strlen(characters_array), 0) <= 0) {
                                            break;
                                            }
                                          }








                                                  if (result != NULL) {
                                                    char choice[15];
                                                    cnt_ask(choice);
                                                    msgstruct.type = command(choice);
                                                    if(msgstruct.type==FILE_ACCEPT){

                                                     int acc= create_server_socket();
                                                    fds[2].fd=acc;
                                                    fds[2].revents=POLLIN;
                                                  //  printf("fds[2].revents: %d\n", fds[2].fd);
                                                    }
                                                    infos(&msgstruct,buff);
                                                    //memset(buff, 0, strlen(buff));
                                                    //strcpy (buff,"\n");
                                                    msgstruct.pld_len = strlen(buff);

                                                    if (send(sockfd, &msgstruct, sizeof(msgstruct), 0) <= 0) {
                                                          break;
                                                    }
                                                    // Sending message of pld_len
                                                    if (send(sockfd, buff, msgstruct.pld_len, 0) <= 0) {
                                                          break;
                                                    }
                                                 }
                                        } //fin recevoir

                      if(fds[2].revents & POLLIN){

                              memset(&msgstruct, 0, sizeof(struct message));
                              memset(buff, 0, MSG_LEN);
                              FILE *file_out;
                              char filename_out[] = "rc/copie.txt";
                              char chemin[]="rc/copie.txt";
                              fds[2].revents=0;

                              struct sockaddr_in client;
                              socklen_t addr_len = sizeof(client);
                              int connfd = accept(fds[2].fd, (struct sockaddr *)&client, &addr_len);//receive from client
                              if (connfd < 0) {
                                  perror("accept");
                                  exit(EXIT_FAILURE);
                              }

                              file_out = fopen(filename_out, "w");
                                if (file_out == NULL) {
                                    printf("Impossible de créer le fichier %s.\n", filename_out);

                                }


                                    if (recv(connfd, &msgstruct, sizeof(struct message), 0) <= 0) {
                                            perror("recieving msg");
                                    }

                                    if (recv(connfd, buff, msgstruct.pld_len, 0) <= 0) {
                                                    perror("recieving buff");
                                    }



                                    fprintf(stdout, "pl recevoir : %d\n",msgstruct.file_size );
                                    pl = msgstruct.file_size;
                      for (int i = 0; i < pl; i++) {
                          fputc(buff[i], file_out);
                      }

                         fclose(file_out);
                         printf("Fichier copie.txt créé avec succès à l'emplacement : %s\n", chemin);
                             fds[2].revents=0;
                             fds[2].fd=0;

                      }
          }//fin while
 }//fin fonction

int handle_connect(const char *server_name, const char *server_port) {

    struct addrinfo hints, *result, *rp;
    int sfd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;        // Famille d'adresses IPv4
    hints.ai_socktype = SOCK_STREAM;  // Type de socket pour TCP
                hints.ai_protocol = IPPROTO_TCP; // Protocole TCP

    if (getaddrinfo(server_name, server_port, &hints, &result) != 0) {
        perror("getaddrinfo()");
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
          if (sfd == -1) {
              continue;
          }
          if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
              break;
          }
        close(sfd);
    }

      if (rp == NULL) {
          fprintf(stderr, "Could not connect\n");
          exit(EXIT_FAILURE);
      }

      freeaddrinfo(result);
      return sfd;
}


int main(int argc, char const *argv[]) {

          if (argc != 3) {
                  fprintf(stderr, "Launching program : %s <server_name> <server_port>\n", argv[0]);
                  exit(EXIT_FAILURE);
              }
              fprintf(stdout, "Connecting to server ...\n");

          int sfd;
          sfd = handle_connect(argv[1], argv[2]);
          echo_client(sfd);
          close(sfd);

        return EXIT_SUCCESS;
}
