#include <arpa/inet.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ClientManager.hpp"

inline constexpr uint16_t SERVER_PORT = 1337;
inline constexpr uint32_t BACKLOG_SIZE = 10;

int main(int argc, char **argv) {
  int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocketFd == -1) {
    printf("Could not create socket\n");
    return 1;
  }
  int reuseaddr = 1;
  int status = setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR,
                          (void *)&reuseaddr, sizeof(reuseaddr));
  if (status == -1) {
    printf("Could not set socket option: %s\n", strerror(errno));
    return 1;
  }

  struct sockaddr_in serverSockAddr;
  socklen_t serverSocklen = sizeof(sockaddr_in);
  serverSockAddr.sin_family = AF_INET;
  serverSockAddr.sin_port = htons(SERVER_PORT);
  serverSockAddr.sin_addr.s_addr = inet_network("0.0.0.0");
  status =
      bind(serverSocketFd, (struct sockaddr *)&serverSockAddr, serverSocklen);
  if (status == -1) {
    printf("Could not bind socket: %s\n", strerror(errno));
    return 1;
  }

  status = listen(serverSocketFd, BACKLOG_SIZE);
  if (status == -1) {
    printf("Could not listen on socket: %s\n", strerror(errno));
    return 1;
  }

  printf("Listening for incoming connections...\n");
  ftp::ClientManager{}.handleForever(serverSocketFd);
  return 0;
}