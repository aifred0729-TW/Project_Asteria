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
    int PORT = 9002;

    printf("[+] Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return;
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
        WSACleanup();
        return;
    }
    printf("[+] Socket created.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return;
    }
    printf("[+] Bind done.\n");

    listen(server_socket, 3);
    printf("[+] Waiting for incoming connections on port 9002...\n");

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr*)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        printf("Accept failed. Error Code: %d", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return;
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

void JMPRSP();

void makeStackExecutable(void* stackPtr) {
    DWORD oldProtect;
    if (VirtualProtect((LPVOID)((uintptr_t)stackPtr & ~(0x1000 - 1)), 0x1000, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        printf("[+] Stack page set to executable.\n");
    }
    else {
        printf("[-] VirtualProtect failed. Error Code: %d\n", GetLastError());
    }
}

void programMain(char buffer[]) {
    char storageBase[0x2c0];
    makeStackExecutable(storageBase);
    memcpy(storageBase, buffer, 0x500);
    return;
}

int main() {
    char magicStorage[0x200];

    while (1) {
        initializeConnection();

        char buffer[0x500] = { 0 };
        recv(client_socket, buffer, sizeof(buffer), 0);
        programMain(buffer);

        closeConnection();
    }

    return 0;
}
