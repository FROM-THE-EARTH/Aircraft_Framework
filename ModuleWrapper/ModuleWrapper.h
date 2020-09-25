#pragma once
#include <string>

class ModuleWrapper {
  const std::string name_;

protected:
  bool available = false;

  ModuleWrapper(std::string name) : name_(name) {}

  virtual void initialize() = 0;

  virtual bool isAvailable() = 0;

public:
  std::string name() const { return name_; }

  std::string status() { return name() + ": " + (isAvailable() ? "OK" : "NG"); }
};