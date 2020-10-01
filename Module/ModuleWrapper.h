#pragma once
#include "../Type/XString.h"

class ModuleWrapper
{
  const xString name_;

protected:
  bool available = false;

  ModuleWrapper(const xString &name)
      : name_(name)
  {
  }

  virtual void initialize() = 0;

  virtual bool isAvailable() = 0;

public:
  xString name() const
  {
    return name_;
  }

  xString status()
  {
    return name() + ": " + (isAvailable() ? "OK" : "NG");
  }
};