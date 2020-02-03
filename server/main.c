#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#include "cJSON.h"
#define MAX 1024
#define PORT 12345
#define SA struct sockaddr

struct member
{
    char *name;
    char *token;
    char *channel;
    int seen;
}x[50];
char *channelmembers[100][100];
int channeltedad[100];
char *channelnames[100];
void tokenmaker(char *);

int main()
{
    int tm = 0 , tc = 0;
    char *buffer = calloc(5000 , sizeof(char));
    //char buffer[80] = "";
    int server_socket, client_socket;
    struct sockaddr_in server, client;
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
    while(true)
    {
        // Create and verify socket
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == INVALID_SOCKET)
            wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
        else
            printf("Socket successfully created..\n");
        // Assign IP and port
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(PORT);
        // Bind newly created socket to given IP and verify
        if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
        {
            printf("Socket binding failed...\n");
            exit(0);
        }
        else
            printf("Socket successfully bound..\n");

        // Now server is ready to listen and verify
        if ((listen(server_socket, 3)) != 0)
        {
            printf("Listen failed...\n");
            exit(0);
        }
        else
            printf("Server listening..\n");
        // Accept the data packet from client and verify
        int len = sizeof(client);
        client_socket = accept(server_socket, (SA *)&client, &len);
        if (client_socket < 0)
        {
            printf("Server accceptance failed...\n");
            exit(0);
        }
        else
            printf("Server acccepted the client..\n");
        memset(buffer , 0 , strlen(buffer));
        recv(client_socket , buffer , MAX , 0);
        if(strncmp(buffer , "register" , 8) == 0)
        {
            char *name = calloc(20 , sizeof(char)) , *pass = calloc(20 , sizeof(char));
            int i = 0;
            for( ; ( i < 80 ) && ( *(buffer + 9 + i) != ',' ) ; i++)
                *(name + i) = *(buffer + 9 + i);
            *(name + i) = '\0';
            int j = 0;
            for( ; *(buffer + i + 11 + j) != '\0' ; j++)
                *(pass + j) = *(buffer + i + 11 + j);
            *(pass + j - 1) = '\0';
            char *esm = calloc(40 , sizeof(char));
            strcat(esm , name);
            strcat(esm , ".user");
            strcat(esm , ".txt");
            FILE *fpr = fopen(esm , "r");
            if(!fpr)
            {
                FILE *fpw = fopen(esm , "a+");
                strcat(name , " ");
                strcat(name , pass);
                strcat(name , "\n");
                fprintf(fpw , name);
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Successful");
                cJSON_AddStringToObject(state , "content", "");
                memset(buffer , 0 , strlen(buffer));
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                fclose(fpw);
                closesocket(server_socket);
            }
            else
            {
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Error");
                cJSON_AddStringToObject(state , "content", "");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
        }
        else if(strncmp(buffer , "login" , 5) == 0)
        {
            //int n = 0;
            char *name = calloc(20 , sizeof(char)) , *pass = calloc(20 , sizeof(char));
            int i = 0;
            for( ; ( i < 80 ) && ( *(buffer + 6 + i) != ',' ) ; i++)
                *(name + i) = *(buffer + 6 + i);
            *(name + i) = '\0';
            int j = 0;
            for( ; *(buffer + i + 8 + j) != '\0' ; j++)
                *(pass + j) = *(buffer + i + 8 + j);
            *(pass + j - 1) = '\0';
            char *esm = calloc(40 , sizeof(char));
            strcat(esm , name);
            strcat(esm , ".user");
            strcat(esm , ".txt");
            FILE *fpr = fopen(esm , "r");
            if(!fpr)
            {
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Error");
                cJSON_AddStringToObject(state , "content", "Username is not valid.");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
            else
            {
                char *PASS = calloc(20 , sizeof(char));
                fscanf(fpr , "%s %s" , name , PASS);
                if(strcmp(pass , PASS) != 0)
                {
                    memset(buffer , 0 , strlen(buffer));
                    cJSON *state = cJSON_CreateObject();
                    cJSON_AddStringToObject(state , "type", "Error");
                    cJSON_AddStringToObject(state , "content", "Wrong password.");
                    buffer = cJSON_Print(state);
                    strcat(buffer , "\n");
                    send(client_socket , buffer , strlen(buffer) , 0);
                    cJSON_Delete(state);
                    closesocket(server_socket);
                }
                else
                {
                    char *token = calloc(21 , sizeof(char));
                    tokenmaker(token);
                    x[tm].name = calloc(20 , sizeof(char));
                    x[tm].token = calloc(21 , sizeof(char));
                    x[tm].channel = calloc(20 , sizeof(char));
                    strcpy(x[tm].name , name);
                    strcpy(x[tm].token , token);
                    tm++;
                    memset(buffer , 0 , strlen(buffer));
                    cJSON *state = cJSON_CreateObject();
                    cJSON_AddStringToObject(state , "type", "Successful");
                    cJSON_AddStringToObject(state , "content", token);
                    buffer = cJSON_Print(state);
                    strcat(buffer , "\n");
                    send(client_socket , buffer , strlen(buffer) , 0);
                    cJSON_Delete(state);
                    closesocket(server_socket);
                }
            }
        }
        else if(strncmp(buffer , "create channel" , 14) == 0)
        {
            char *name = calloc(20 , sizeof(char)) , *authtoken = calloc(21 , sizeof(char));
            int i = 0;
            for( ; ( i < 80 ) && ( *(buffer + 15 + i) != ',' ) ; i++)
                *(name + i) = *(buffer + 15 + i);
            *(name + i) = '\0';
            int j = 0;
            for( ; *(buffer + i + 17 + j) != '\0' ; j++)
                *(authtoken + j) = *(buffer + i + 17 + j);
            *(authtoken + j - 1) = '\0';
            char *esm = calloc(40 , sizeof(char));
            strcat(esm , name);
            strcat(esm , ".channelmessages");
            strcat(esm , ".json");
            FILE *fpr = fopen(esm , "r");
            if(!fpr)
            {
                int i = 0;
                for( ; i < tm ; i++)
                {
                    if(strcmp(authtoken , x[i].token) ==0)
                    {
                        break;
                    }
                }
                cJSON *monitor = cJSON_CreateObject();
                cJSON_AddStringToObject(monitor , "type" , "List");
                cJSON *content;
                content = cJSON_AddArrayToObject(monitor , "content");
                cJSON *message = cJSON_CreateObject();
                char *payam = calloc(200 , sizeof(char));
                sprintf(payam , "%s created %s." , x[i].name , name);
                cJSON_AddStringToObject(message , "sender" , "server");
                cJSON_AddStringToObject(message , "content" , payam);
                cJSON_AddItemToArray(content , message);
                memset(buffer , 0 , strlen(buffer));
                buffer = cJSON_Print(monitor);
                FILE *fpw = fopen(esm , "a+");
                fprintf(fpw , "%s" , buffer);
                fclose(fpw);
                cJSON_Delete(monitor);
                channelmembers[tc][channeltedad[tc]] = calloc(20 , sizeof(char));
                strcpy(channelmembers[tc][channeltedad[tc]] , x[i].name);
                channelnames[tc] = calloc(20 , sizeof(char));
                strcpy(channelnames[tc] , name);
                channeltedad[tc]++;
                tc++;
                strcpy(x[i].channel , name);
                x[i].seen = 0;
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Successful");
                cJSON_AddStringToObject(state , "content", "");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
            else
            {
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Error");
                cJSON_AddStringToObject(state , "content", "");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
            fclose(fpr);
        }
        else if(strncmp(buffer , "join channel" , 12) == 0)
        {
            char *name = calloc(20 , sizeof(char)) , *authtoken = calloc(21 , sizeof(char));
            int i = 0;
            for( ; ( i < 80 ) && ( *(buffer + 13 + i) != ',' ) ; i++)
                *(name + i) = *(buffer + 13 + i);
            *(name + i) = '\0';
            int j = 0;
            for( ; *(buffer + i + 15 + j) != '\0' ; j++)
                *(authtoken + j) = *(buffer + i + 15 + j);
            *(authtoken + j - 1) = '\0';
            char *esm = calloc(40 , sizeof(char));
            strcat(esm , name);
            strcat(esm , ".channelmessages");
            strcat(esm , ".json");
            FILE *fpr = fopen(esm , "r");
            if(fpr)
            {
                int i = 0;
                for( ; i < tm ; i++)
                {
                    if(strcmp(authtoken , x[i].token) ==0)
                    {
                        break;
                    }
                }
                if(strcmp(x[i].channel , "") != 0)
                {
                    memset(buffer , 0 , strlen(buffer));
                    cJSON *state = cJSON_CreateObject();
                    cJSON_AddStringToObject(state , "type", "Error");
                    cJSON_AddStringToObject(state , "content", "You are in another channel.");
                    buffer = cJSON_Print(state);
                    strcat(buffer , "\n");
                    send(client_socket , buffer , strlen(buffer) , 0);
                    cJSON_Delete(state);
                    closesocket(server_socket);
                }
                else
                {
                    int flag = -1;
                    for(int j = 0 ; j < tc ; tc++)
                    {
                        if(strcmp(name , channelnames[j]) == 0)
                        {
                            flag = j;
                            break;
                        }
                    }
                    if(flag < 0)
                    {
                        channelmembers[tc][channeltedad[tc]] = calloc(20 , sizeof(char));
                        strcpy(channelmembers[tc][channeltedad[tc]] , x[i].name);
                        channelnames[tc] = calloc(20 , sizeof(char));
                        strcpy(channelnames[tc] , name);
                        strcpy(x[i].channel , name);
                        x[i].seen = 0;
                        channeltedad[tc]++;
                        tc++;
                    }
                    else
                    {
                        channelmembers[flag][channeltedad[flag]] = calloc(20 , sizeof(char));
                        strcpy(channelmembers[flag][channeltedad[flag]] , x[i].name);
                        strcpy(x[i].channel , name);
                        x[i].seen = 0;
                        channeltedad[flag]++;
                    }
                    char *message = calloc(200 , sizeof(char));
                    sprintf(message , "%s joined." , x[i].name);
                    char *tmp = calloc(5120 , sizeof(char));
                    FILE *fpr = fopen(esm , "r");
                    int e = 0;
                    while (1)
                    {
                        tmp[e] = fgetc(fpr);
                        if (feof(fpr))
                            break;
                        e++;
                    }
                    fclose(fpr);
                    memset(buffer , 0 , strlen(buffer));
                    cJSON *monitor_json = cJSON_Parse(tmp);
                    cJSON *content = NULL;
                    content = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
                    cJSON *messagE = cJSON_CreateObject();
                    cJSON_AddStringToObject(messagE , "sender" , "server");
                    cJSON_AddStringToObject(messagE , "content" , message);
                    cJSON_AddItemToArray(content , messagE);
                    memset(buffer , 0 , strlen(buffer));
                    buffer = cJSON_Print(monitor_json);
                    FILE *fpw = fopen(esm , "w");
                    fprintf(fpw , buffer);
                    fclose(fpw);
                    cJSON_Delete(monitor_json);
                    memset(buffer , 0 , strlen(buffer));
                    cJSON *state = cJSON_CreateObject();
                    cJSON_AddStringToObject(state , "type", "Successful");
                    cJSON_AddStringToObject(state , "content", "");
                    buffer = cJSON_Print(state);
                    strcat(buffer , "\n");
                    send(client_socket , buffer , strlen(buffer) , 0);
                    cJSON_Delete(state);
                    closesocket(server_socket);
                }
            }
            else
            {
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Error");
                cJSON_AddStringToObject(state , "content", "Channel not found.");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
        }
        else if(strncmp(buffer , "logout" , 6) == 0)
        {
            char *authtoken = calloc(21 , sizeof(char));
            strcpy(authtoken , buffer + 7);
            *(authtoken + strlen(authtoken) - 1) = '\0';
            int j = 0;
            for( ; j < tm ; j++)
            {
                if(strcmp(authtoken , x[j].token) == 0)
                {
                    break;
                }
            }
            strcpy(x[j].channel , "");
            strcpy(x[j].name , "");
            strcpy(x[j].token , "");
            memset(buffer , 0 , strlen(buffer));
            cJSON *state = cJSON_CreateObject();
            cJSON_AddStringToObject(state , "type", "Successful");
            cJSON_AddStringToObject(state , "content", "");
            buffer = cJSON_Print(state);
            strcat(buffer , "\n");
            send(client_socket , buffer , strlen(buffer) , 0);
            cJSON_Delete(state);
            closesocket(server_socket);
        }
        else if(strncmp(buffer , "send" , 4) == 0)
        {
            char *message = calloc(200 , sizeof(char)) , *authtoken = calloc(21 , sizeof(char));
            int i = 0;
            for( ; *(buffer + 5 + i) != ',' ; i++)
                *(message + i) = *(buffer + 5 + i);
            *(message + i) = '\0';
            int j = 0;
            for( ; *(buffer + i + 7 + j) != '\0' ; j++)
                *(authtoken + j) = *(buffer + i + 7 + j);
            *(authtoken + j - 1) = '\0';
            int k = 0;
            for( ; k < tm ; k++)
            {
                if(strcmp(authtoken , x[k].token) ==0)
                {
                    break;
                }
            }
            if(strcmp(x[k].channel , "") == 0)
            {
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Error");
                cJSON_AddStringToObject(state , "content", "");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
            else
            {
                char *esm = calloc(40 , sizeof(char));
                strcat(esm , x[k].channel);
                strcat(esm , ".channelmessages");
                strcat(esm , ".json");
                FILE *fpr = fopen(esm , "r");
                char *tmp = calloc(5120 , sizeof(char));
                int h = 0;
                while (1)
                {
                    tmp[h] = fgetc(fpr);
                    if (feof(fpr))
                        break;
                    h++;
                }
                fclose(fpr);
                cJSON *monitor_json = cJSON_Parse(tmp);
                cJSON *content = NULL;
                content = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
                cJSON *messagE = cJSON_CreateObject();
                cJSON_AddStringToObject(messagE , "sender" , x[k].name);
                cJSON_AddStringToObject(messagE , "content" , message);
                cJSON_AddItemToArray(content , messagE);
                memset(buffer , 0 , strlen(buffer));
                buffer = cJSON_Print(monitor_json);
                FILE *fpw = fopen(esm , "w");
                fprintf(fpw , buffer);
                fclose(fpw);
                cJSON_Delete(monitor_json);
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Successful");
                cJSON_AddStringToObject(state , "content", "");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
        }
        else if(strncmp(buffer , "refresh" , 7) == 0)
        {
            char *authtoken = calloc(21 , sizeof(char));
            int i = 0;
            for( ; ( i < 80 ) && ( *(buffer + 8 + i) != ',' ) ; i++)
                *(authtoken + i) = *(buffer + 8 + i);
            *(authtoken + 20) = '\0';
            int j = 0;
            for( ; j < tm ; j++)
            {
                if(strcmp(authtoken , x[j].token) ==0)
                {
                    break;
                }
            }
            if(strcmp(x[j].channel , "") == 0)
            {
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Error");
                cJSON_AddStringToObject(state , "content", "");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
            else
            {
                char esm[40] = {};
                strcat(esm , x[j].channel);
                strcat(esm , ".channelmessages");
                strcat(esm , ".json");
                FILE *fpr = fopen(esm , "r+");
                char tmp[5120] = {};
                int k = 0;
                while (1)
                {
                    tmp[k] = fgetc(fpr);
                    if (feof(fpr))
                        break;
                    k++;
                }
                fclose(fpr);
                cJSON *monitor = cJSON_Parse(tmp);
                cJSON *sending = cJSON_CreateObject();
                cJSON *messages = NULL;
                cJSON *packs = NULL;
                cJSON_AddStringToObject(sending , "type" , "List");
                packs = cJSON_AddArrayToObject(sending , "content");
                messages = cJSON_GetObjectItemCaseSensitive(monitor , "content");
                int tedad = cJSON_GetArraySize(messages);
                for(int l = x[j].seen ; l < tedad ; l++)
                {
                    cJSON *message = cJSON_GetArrayItem(messages , l);
                    cJSON *sender = NULL;
                    cJSON *content = NULL;
                    sender = cJSON_GetObjectItemCaseSensitive(message , "sender");
                    content = cJSON_GetObjectItemCaseSensitive(message , "content");
                    cJSON *pack = cJSON_CreateObject();
                    cJSON_AddStringToObject(pack , "sender" , sender->valuestring);
                    cJSON_AddStringToObject(pack , "content" , content->valuestring);
                    cJSON_AddItemToArray(packs , pack);
                }
                memset(buffer , 0 , strlen(buffer));
                buffer = cJSON_Print(sending);
                printf("%s\n" , buffer);
                send(client_socket , buffer , strlen(buffer) , 0);
                x[j].seen = tedad;
                cJSON_Delete(monitor);
                cJSON_Delete(sending);
                closesocket(server_socket);
            }
        }
        else if(strncmp(buffer , "channel members" , 15) == 0)
        {
            char *authtoken = calloc(21 , sizeof(char));
            int i = 0;
            for( ; ( i < 80 ) && ( *(buffer + 16 + i) != ',' ) ; i++)
                *(authtoken + i) = *(buffer + 16 + i);
            *(authtoken + 20) = '\0';
            int j = 0;
            for( ; j < tm ; j++)
            {
                if(strcmp(authtoken , x[j].token) ==0)
                {
                    break;
                }
            }
            if(strcmp(x[j].channel , "") == 0)
            {
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Error");
                cJSON_AddStringToObject(state , "content", "");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
            else
            {
                int k = 0;
                for( ; k < tc ; k++)
                {
                    if(strcmp(x[j].channel , channelnames[k]) == 0)
                    {
                        break;
                    }
                }
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON *content = NULL;
                cJSON_AddStringToObject(state , "type", "List");
                content = cJSON_AddArrayToObject(state , "content");
                for(int m = 0 ; m < tm ; m++)
                {
                    if(strcmp(x[j].channel , x[m].channel) == 0)
                    {
                        cJSON_AddItemToArray(content , cJSON_CreateString(x[m].name));
                    }
                }
                memset(buffer , 0 , strlen(buffer));
                buffer = cJSON_Print(state);
                send(client_socket , buffer , strlen(buffer) , 0);
                closesocket(server_socket);
            }
        }
        else if(strncmp(buffer , "leave" , 5) == 0)
        {
            char *authtoken = calloc(21 , sizeof(char));
            strcpy(authtoken , buffer + 6);
            *(authtoken + strlen(authtoken) - 1) = '\0';
            int j = 0;
            for( ; j < tm ; j++)
            {
                if(strcmp(authtoken , x[j].token) ==0)
                {
                    break;
                }
            }
            if(strcmp(x[j].channel , "") == 0)
            {
                memset(buffer , 0 , strlen(buffer));
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Error");
                cJSON_AddStringToObject(state , "content", "");
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
            else
            {
                char *esm = calloc(40 , sizeof(char));
                strcat(esm , x[j].channel);
                strcat(esm , ".channelmessages");
                strcat(esm , ".json");
                int k = 0;
                for( ; k < tc ; k++)
                {
                    if(strcmp(x[j].channel , channelnames[k]) == 0)
                    {
                        break;
                    }
                }
                for(int h = 0 ; h < 100 ; h++)
                {
                    if(strcmp(channelmembers[k][h] , x[j].name) == 0 )
                    {
                        strcpy(channelmembers[k][h] , "");
                        strcpy(x[j].channel , "");
                        break;
                    }
                }
                char *message = calloc(200 , sizeof(char));
                sprintf(message , "%s leaved the channel." , x[j].name);
                char *tmp = calloc(5120 , sizeof(char));
                FILE *fpr = fopen(esm , "r");
                int e = 0;
                while (1)
                {
                    tmp[e] = fgetc(fpr);
                    if (feof(fpr))
                        break;
                    e++;
                }
                fclose(fpr);
                memset(buffer , 0 , strlen(buffer));
                cJSON *monitor_json = cJSON_Parse(tmp);
                cJSON *content = NULL;
                content = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
                cJSON *messagE = cJSON_CreateObject();
                cJSON_AddStringToObject(messagE , "sender" , "server");
                cJSON_AddStringToObject(messagE , "content" , message);
                cJSON_AddItemToArray(content , messagE);
                memset(buffer , 0 , strlen(buffer));
                buffer = cJSON_Print(monitor_json);
                FILE *fpw = fopen(esm , "w");
                fprintf(fpw , buffer);
                fclose(fpw);
                cJSON_Delete(monitor_json);
                cJSON *state = cJSON_CreateObject();
                cJSON_AddStringToObject(state , "type", "Successful");
                cJSON_AddStringToObject(state , "content", "");
                memset(buffer , 0 , strlen(buffer));
                buffer = cJSON_Print(state);
                strcat(buffer , "\n");
                send(client_socket , buffer , strlen(buffer) , 0);
                cJSON_Delete(state);
                closesocket(server_socket);
            }
        }
    }
}

void tokenmaker(char *token)
{
    srand(time(NULL));
    for(int i = 0 ; i < 20 ; i++)
    {
        int a = rand() % 2 ;
        if(a == 0)
        {
            char b = 65 + ( rand() % 26 );
            *(token + i) = b;
        }
        else
        {
            char b = 97 + ( rand() % 26 );
            *(token + i) = b;
        }
    }
    *(token + 20) = '\0';
}
