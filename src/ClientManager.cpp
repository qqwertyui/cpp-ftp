#include "ClientManager.hpp"
#include <arpa/inet.h>
#include <cstdlib>
#include <sys/socket.h>
#include <thread>

namespace ftp {
void ClientManager::handleForever(int serverSocketFd) {
  while (true) {
    struct sockaddr_in clientSockAddr;
    socklen_t clientSocklen = sizeof(sockaddr_in);
    int controlClientFd = accept(
        serverSocketFd, (struct sockaddr *)&clientSockAddr, &clientSocklen);
    ftp::ConnectionInfo connInfo{controlClientFd, clientSockAddr};
    const std::string &uniqueClientId{connInfo.ip + ":" +
                                      std::to_string(connInfo.port)};
    const auto &[it, clientAdded] =
        activeClients.try_emplace(uniqueClientId, connInfo);
    if (not clientAdded) {
      puts("Unexpected error, possible attempt to add the same client twice, "
           "crashing...");
      std::abort();
    }
    std::thread{[it, this]() {
      it->second.run();
      removeClient(it->first);
    }}.detach();
  }
}

void ClientManager::removeClient(const std::string &uniqueClientId) {
  std::unique_lock<std::mutex> lock{mtx};
  activeClients.erase(uniqueClientId);
}

} // namespace ftp