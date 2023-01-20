#include <arpa/inet.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "ControlConnection.hpp"
#include "Span.hpp"

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
  while (true) {
    struct sockaddr_in clientSockAddr;
    socklen_t clientSocklen = sizeof(sockaddr_in);
    int controlClientFd = accept(
        serverSocketFd, (struct sockaddr *)&clientSockAddr, &clientSocklen);
    ftp::ConnectionInfo connInfo{controlClientFd, clientSockAddr};
    ftp::ControlConnection conn{connInfo};
    if (conn.isConnected()) {
      conn.onConnect();
    } else {
      conn.onError(errno);
      continue;
    }
    std::array<unsigned char, 1024> buffer;
    while (true) {
      ssize_t receivedBytes =
          recv(connInfo.fd, buffer.data(), buffer.size(), 0);
      if (receivedBytes == -1) {
        conn.onError(errno);
        close(connInfo.fd);
        conn.onDisconnect();
        break;
      } else if (receivedBytes == 0) {
        close(connInfo.fd);
        conn.onDisconnect();
        break;
      }
      conn.onData(
          ftp::ByteSpan{buffer.data(), static_cast<uint32_t>(receivedBytes)});
    }
  }
  close(serverSocketFd);
  return 0;
}