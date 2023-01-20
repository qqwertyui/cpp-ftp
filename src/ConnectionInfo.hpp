#pragma once

#include <arpa/inet.h>
#include <string>

namespace ftp {
struct ConnectionInfo {
  ConnectionInfo(int connectionFd, struct sockaddr_in connectionSockaddr)
      : fd{connectionFd}, port{htons(connectionSockaddr.sin_port)},
        ip{inet_ntoa(connectionSockaddr.sin_addr)} {}

  inline bool isValid() const { return fd != -1; }

  int fd;
  int port;
  std::string ip;
};
} // namespace ftp