#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>


#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 4096
#define XOR_KEY 55

void calculateStudentID(FILE* file) {
    int studentID[] = {3, 5, 6, 8, 9, 4};
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += studentID[i];
    }
    int remainder = sum % studentID[4];
    printf("Remainder: %d\n", remainder);
    fprintf(file, "Remainder: %d\n", remainder);
}

void xorCipher(char buffer[132], FILE* file) {
    for (int i = 0; i < 132; i++) {
        buffer[i] ^= XOR_KEY;
        printf("%c", buffer[i]);
        fprintf(file, "%c", buffer[i]);
    }
    printf("\n");
    fprintf(file, "\n");
}

int isPrime(int number) {
    if (number < 2) return 0;
    for (int i = 2; i <= number - 1; i++) {
        if (number % i == 0) return 0;
    }
    return 1;
}

void printPrimeIndices(char buffer[], int length, FILE* file) {
    for (int i = 2; i < length - 1; i++) {
        if (isPrime(i)) {
            printf("%c", buffer[i - 1]);
            fprintf(file, "%c", buffer[i - 1]);
        }
    }
    printf("\n");
    fprintf(file, "\n");
}

int main() {
    FILE* file = fopen("communication_log.txt", "w");
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    struct addrinfo *serverInfo = NULL, *ptr = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 2);

    result = getaddrinfo("", "", &hints, &serverInfo);
    if (result != 0) {
        printf("getaddrinfo failed: %d\n", result);
        fprintf(file, "getaddrinfo failed: %d\n", result);
        WSACleanup();
        return 1;
    }

    SOCKET clientSocket = INVALID_SOCKET;
    ptr = serverInfo;

    clientSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    result = connect(clientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (result == SOCKET_ERROR) {
        printf("Connection failed\n");
    } else {
        printf("Connected to server\n");
    }

    for (int i = 0; i < 17; i++) {
        char sendBuffer[BUFFER_SIZE];
        fgets(sendBuffer, BUFFER_SIZE, stdin);

        result = send(clientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
        if (result == SOCKET_ERROR) {
            printf("Send failed: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        char recvBuffer[BUFFER_SIZE];
        result = (int)recv(clientSocket, recvBuffer, BUFFER_SIZE, 0);

        if (result > 0) {
            printf("Bytes received: %d\n", result);
        } else if (result == 0) {
            printf("Connection closed\n");
        } else {
            printf("Receive failed: %d\n", WSAGetLastError());
        }

        if (i == 4) {
            calculateStudentID(file);
        }
        if (i == 7) {
            xorCipher(recvBuffer, file);
        }
        if (i == 12) {
            printPrimeIndices(recvBuffer, strlen(recvBuffer), file);
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    fclose(file);
    return 0;
}
