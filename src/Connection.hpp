#pragma once

#include "ConnectionInfo.hpp"
#include "NonCopyable.hpp"
#include "Span.hpp"

namespace ftp {
template <typename T> class Span;

class Connection : public NonCopyable {
public:
  Connection(const ConnectionInfo &info) : info(info) {}

  inline bool isConnected() const { return info.isValid(); }

  virtual void onConnect() = 0;
  virtual void onDisconnect() = 0;
  virtual void onError(int errorNumber) = 0;
  virtual void onData(const ByteSpan &data) = 0;

  const ConnectionInfo info;
};
} // namespace ftp