#pragma once

#include "ControlConnection.hpp"
#include "NonCopyable.hpp"

namespace ftp {
class Client : public NonCopyable {
public:
  Client(const ConnectionInfo &info);
  void run();

private:
  ControlConnection ctrlConn;
};
} // namespace ftp