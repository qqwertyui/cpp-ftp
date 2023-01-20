#include "Client.hpp"
#include "Span.hpp"
#include <array>
#include <errno.h>
#include <unistd.h>

namespace ftp {
Client::Client(const ConnectionInfo &info) : ctrlConn(info) {}

void Client::run() {
  if (not ctrlConn.isConnected()) {
    ctrlConn.onError(errno);
  }
  ctrlConn.onConnect();
  std::array<unsigned char, 1024> buffer;
  while (true) {
    ssize_t receivedBytes =
        recv(ctrlConn.info.fd, buffer.data(), buffer.size(), 0);
    if (receivedBytes == -1) {
      ctrlConn.onError(errno);
      break;
    } else if (receivedBytes == 0) {
      break;
    }
    ctrlConn.onData(
        ftp::ByteSpan{buffer.data(), static_cast<uint32_t>(receivedBytes)});
  }
  close(ctrlConn.info.fd);
  ctrlConn.onDisconnect();
}
} // namespace ftp