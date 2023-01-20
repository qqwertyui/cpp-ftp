#pragma once

#include "Connection.hpp"

namespace ftp {
class ControlConnection : public Connection {
public:
  using Connection::Connection;

  virtual void onConnect() override;
  virtual void onDisconnect() override;
  virtual void onError(int errorNumber) override;
  virtual void onData(const ByteSpan &data) override;
};
} // namespace ftp