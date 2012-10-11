// TODO

#include <arpa/inet.h>

int getIPFromAlNetwork() {
  struct sockaddr_in s_addr;
  s_addr.sin_addr.s_addr = inet_addr("192.168.1.213");
  return s_addr.sin_addr.s_addr;
}
