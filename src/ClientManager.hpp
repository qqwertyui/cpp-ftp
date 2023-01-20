#pragma once

#include "Client.hpp"
#include "NonCopyable.hpp"
#include <map>
#include <mutex>
#include <string>

namespace ftp {
class ClientManager : public NonCopyable {
public:
  ClientManager() = default;
  void handleForever(int serverSocketFd);

private:
  void removeClient(const std::string &uniqueClientId);

  std::map<std::string, Client> activeClients;
  std::mutex mtx;
};
} // namespace ftp