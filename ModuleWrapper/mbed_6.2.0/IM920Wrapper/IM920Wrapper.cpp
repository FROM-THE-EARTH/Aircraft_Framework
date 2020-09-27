#include "IM920Wrapper.h"

void IM920Wrapper::transmit(const std::string &str) {
  const std::string s = name() + "->" + str;

  if (sendToSerial_) {
    printf("[Debug]");
    printf("%s", s.c_str());
    printf("\r\n");
  }

  if (isAvailable()) {
    const size_t size = s.size() + 1;
    char *buf = new char[size];
    strcpy(buf, s.c_str());

    IM920::send(buf, size);

    delete[] buf;
  }
}

std::string IM920Wrapper::receive() {
  char buf[257];

  const int i = IM920::recv(buf, 256);
  buf[i] = 0;

  const std::string str = buf;

  if (sendToSerial_) {
    printf("[Debug]");
    printf("%s", str.c_str());
    printf("\r\n");
  }

  return str;
}