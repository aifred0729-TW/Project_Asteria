#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

SOCKET client_socket, server_socket;

void initializeConnection() {
    WSADATA wsa;
    struct sockaddr_in server, client;
    int c;
    int PORT = 9001;

    printf("[+] Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("[+] Socket created.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("[+] Bind done.\n");

    listen(server_socket, 3);
    printf("[+] Waiting for incoming connections on port 9001...\n");

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr*)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    printf("[+] Connection accepted.\n");

    return;
}

void closeConnection() {
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return;
}


void win() {
    Sleep(100);
    const char* message = "YOU WIN !!\n";
    send(client_socket, message, strlen(message), 0);
    return;
}

void failed() {
    Sleep(100);
    const char* message = "There are too few cats. No need to feed them.\n";
    send(client_socket, message, strlen(message), 0);
    return;
}

void programMain(char buffer[]) {

    struct {
        char feed[0x30];
        int meows;
    } data;

    data.meows = 10;
    memcpy(data.feed, buffer, 0x31);

    char message[30];
    sprintf(message, "Meow Meow Count : %d\n", data.meows);
    send(client_socket, message, strlen(message), 0);

    if (data.meows > 250) {
        win();
    }
    else {
        failed();
    }

    return;
}

int main() {

    while (1) {
        initializeConnection();

        char buffer[0x32] = { 0 };
        recv(client_socket, buffer, sizeof(buffer), 0);
        programMain(buffer);

        closeConnection();
    }

    return 0;
}
