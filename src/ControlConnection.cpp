#include "ControlConnection.hpp"
#include <cstdio>
#include <cstring>
#include <errno.h>

namespace ftp {
void ControlConnection::onConnect() {
  printf("New connection from %s:%u\n", info.ip.c_str(), info.port);
}
void ControlConnection::onDisconnect() {
  printf("Connection %s:%u closed\n", info.ip.c_str(), info.port);
}
void ControlConnection::onError(int errorNumber) {
  printf("Could not listen on socket: %s\n", strerror(errorNumber));
}
void ControlConnection::onData(const ByteSpan &data) {
  printf("Received %lu bytes from %s:%u\n", data.size(), info.ip.c_str(),
         info.port);
}
} // namespace ftp