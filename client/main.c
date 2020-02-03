#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.h"
#define MAX 5000
#define PORT 12345
#define SA struct sockaddr
int CHATAPP(char *);
void accountmenu(char *);
void mainmenu(char * , char *);
void chatmenu(char * , char *);
int main()
{
    char *buffer = calloc(MAX , sizeof(char));
    accountmenu(buffer);
    return 0;
}
int CHATAPP(char *buffer)
{
	int client_socket;
	struct sockaddr_in servaddr;
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	send(client_socket , buffer , strlen(buffer) , 0);
    memset(buffer , 0 , MAX);
	recv(client_socket , buffer , MAX, 0);
	// Close the socket
	closesocket(client_socket);
	return 1;
}
void accountmenu(char *buffer)
{
    int choice = 0;
    char *username = calloc(20 , sizeof(char)) , *password = calloc(20 , sizeof(char));
    while(1)
    {
        printf("Account menu:\n1: Register\n2: Login\n");
        scanf("%d", &choice);
        if(choice == 1)
        {
            memset( buffer , 0 , strlen(buffer) );
            strcpy(buffer, "register ");
            printf("Enter Username:\n");
            scanf("%s", username);
            printf("Enter Password:\n");
            scanf("%s", password);
            strncat(buffer , username , strlen(username));
            strncat(buffer , ", " , strlen(", "));
            strncat(buffer , password , strlen(password));
            strncat(buffer , "\n" , strlen("\n"));
            memset(username , 0 , strlen(username));
            memset(password , 0 , strlen(password));
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *type = cJSON_GetObjectItemCaseSensitive(monitor_json, "type");
            memset( buffer , 0 , strlen(buffer) );
            strncpy(buffer, "Successful", strlen("Successful"));
            if(strcmp(type->valuestring , buffer) == 0)
                printf("Successfully registered.\nNow you can login!\n");
            else
                printf("Not registered.\nTry again!\n");
        }
        else if(choice == 2)
        {
            strcpy(buffer, "login ");
            printf("Enter Username:\n");
            scanf("%s", username);
            printf("Enter Password:\n");
            scanf("%s", password);
            strncat(buffer , username , strlen(username));
            strncat(buffer , ", " , 2);
            strncat(buffer , password , strlen(password));
            strncat(buffer , "\n" , 1);
            memset(username , 0 , strlen(username));
            memset(password , 0 , strlen(password));
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *type = cJSON_GetObjectItemCaseSensitive(monitor_json, "type");
            cJSON *content = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
            memset(buffer , 0 , strlen(buffer));
            if(strncmp(type->valuestring , "Error" , 5) == 0)
            {
                if(strncmp(content->valuestring , "Username is not valid." , 22) == 0)
                    printf("Your Username is wrong.\nTry again!\n");
                else if(strncmp(content->valuestring , "Wrong password." , 15) == 0)
                    printf("Your Password is wrong.\nTry again!\n");
                else
                    printf("You've logged in before.\n");
            }
            else
            {
                printf("You are logged in.\n");
                mainmenu(buffer , content->valuestring);
            }
        }
        else
            printf("Not accepted.\nChoose 1 or 2!\n");
    }
}
void mainmenu(char *buffer , char *authtoken)
{
    int choice;
    char *channelname = calloc(20 , sizeof(char));
    while(1)
    {
        printf("Main menu:\n1: Create Channel\n2: Join Channel\n3: Logout\n");
        scanf("%d", &choice);
        if(choice == 1)
        {
            memset(buffer , 0 , strlen(buffer));
            strcpy(buffer, "create channel ");
            printf("Enter ChannelName:\n");
            scanf("%s", channelname);
            strncat(buffer , channelname , strlen(channelname));
            strncat(buffer , ", " , 2);
            strncat(buffer , authtoken , strlen(authtoken));
            strncat(buffer , "\n" , 1);
            memset(channelname , 0 , strlen(channelname));
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *type = cJSON_GetObjectItemCaseSensitive(monitor_json, "type");
            if(strncmp(type->valuestring , "Successful" , 5) == 0)
            {
                printf("Channel successfully created.\n");
                chatmenu(buffer , authtoken);
            }
            else
                printf("Channelname is not available.\n");
        }
        else if(choice == 2)
        {
            memset(buffer , 0 , strlen(buffer));
            strcpy(buffer, "join channel ");
            printf("Enter ChannelName:\n");
            scanf("%s", channelname);
            strncat(buffer , channelname , strlen(channelname));
            strncat(buffer , ", " , 2);
            strncat(buffer , authtoken , strlen(authtoken));
            strncat(buffer , "\n" , 1);
            memset(channelname , 0 , strlen(channelname));
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *type = cJSON_GetObjectItemCaseSensitive(monitor_json, "type");
            cJSON *content = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
            if(strncmp(type->valuestring , "Successful" , 10) == 0)
            {
                printf("You have successfully joined this channel.\n");
                chatmenu(buffer , authtoken);
            }
            else if(strncmp(content->valuestring , "Channel not found." , 18) == 0)
                printf("There is no channel with this name.\n");
            else if(strncmp(content->valuestring , "You are in another channel." , 27) == 0)
                printf("You are in another channel,So you can't join in this one.\n");
        }
        else if(choice == 3)
        {
            strcpy(buffer, "logout ");
            strncat(buffer , authtoken , strlen(authtoken));
            strncat(buffer , "\n" , 1);
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *type = cJSON_GetObjectItemCaseSensitive(monitor_json, "type");
            if(strncmp(type->valuestring , "Successful" , 10) == 0)
            {
                printf("You are successfully logged out.\n");
                accountmenu(buffer);
            }
            memset(buffer , 0 , strlen(buffer));
        }
        else
            printf("Not accepted.\nChoose 1 , 2 or 3.\n");
    }
}
void chatmenu(char *buffer , char *authtoken)
{
    int choice;
    char *message = calloc(200 , sizeof(char));
    while(1)
    {
        printf("Chat menu:\n1: Send Message\n2: Refresh\n3: Channel Members\n4: Leave Channel\n");
        scanf("%d", &choice);
        if(choice == 1)
        {
            strcpy(buffer, "send ");
            printf("Enter Message:\n");
            getchar();
            memset(message , 0 , 200);
            scanf("%[^\n]s", message);
            strncat(buffer , message , strlen(message));
            strncat(buffer , ", " , 2);
            strncat(buffer , authtoken , strlen(authtoken));
            strncat(buffer , "\n" , 1);
            memset(message , 0 , strlen(message));
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *type = cJSON_GetObjectItemCaseSensitive(monitor_json, "type");
            if(strncmp(type->valuestring , "Successful" , 10) != 0)
                printf("Message is not sent!\n");
            memset(buffer , 0 , strlen(buffer));
        }
        else if(choice == 2)
        {
            strcpy(buffer, "refresh ");
            strncat(buffer , authtoken , strlen(authtoken));
            strncat(buffer , "\n" , 1);
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *contents = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
            cJSON *contenT = NULL;
            cJSON_ArrayForEach(contenT, contents)
            {
                cJSON *sender = cJSON_GetObjectItemCaseSensitive(contenT, "sender");
                cJSON *content = cJSON_GetObjectItemCaseSensitive(contenT, "content");
                printf("From %s :  ", sender->valuestring);
                printf("%s\n",content->valuestring);
            }
            memset(buffer , 0 , strlen(buffer));
        }
        else if(choice == 3)
        {
            strcpy(buffer, "channel members ");
            strncat(buffer , authtoken , strlen(authtoken));
            strncat(buffer , "\n" , 1);
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *contents = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
            cJSON *contenT = NULL;
            printf("Channel members :\n");
            cJSON_ArrayForEach(contenT, contents)
            {
                printf("%s\n", contenT->valuestring);
            }
            memset(buffer , 0 , strlen(buffer));
        }
        else if(choice == 4)
        {
            strcpy(buffer, "leave ");
            strncat(buffer , authtoken , strlen(authtoken));
            strncat(buffer , "\n" , 1);
            CHATAPP(buffer);
            cJSON *monitor_json = cJSON_Parse(buffer);
            cJSON *type = cJSON_GetObjectItemCaseSensitive(monitor_json, "type");
            if(strncmp(type->valuestring , "Successful" , 10) == 0)
            {
                printf("You successfully left the channel.\n");
                memset(buffer , 0 , strlen(buffer));
                mainmenu(buffer , authtoken);
            }
        }
        else
            printf("Not accepted.\nChoose 1 , 2 , 3 or 4.\n");
    }
}
