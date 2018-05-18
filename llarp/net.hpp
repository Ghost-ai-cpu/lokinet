#ifndef LLARP_NET_HPP
#define LLARP_NET_HPP
#include <llarp/net.h>

bool operator == (const sockaddr& a, const sockaddr& b)
{
  socklen_t sz = sizeof(a.sa_data);
  switch(a.sa_family)
  {
  case AF_INET:
    sz = sizeof(sockaddr_in);
    break;
  case AF_INET6:
    sz = sizeof(sockaddr_in6);
    break;
  case AF_PACKET:
    sz = sizeof(sockaddr_ll);
    break;
  default:
    break;
  }
  return a.sa_family == b.sa_family && memcmp(a.sa_data, b.sa_data, sz) == 0;
}

bool operator < (const sockaddr_in6 & a, const sockaddr_in6 & b)
{
  return memcmp(&a, &b, sizeof(sockaddr_in6)) < 0;
}

bool operator < (const in6_addr & a, const in6_addr b)
{
  return memcmp(&a, &b, sizeof(in6_addr)) < 0;
}

namespace llarp
{
  struct Addr
  {
    int af = AF_INET;
    in6_addr addr = {0};
    uint16_t port = 0;

    Addr() {};
    Addr(const sockaddr & other) {
      uint8_t * addrptr = addr.s6_addr;
      switch(other.sa_family)
      {
      case AF_INET:
        // SIIT
        memcpy(12+addrptr, &((const sockaddr_in*)(&other))->sin_addr, 4);
        addrptr[11] = 0xff;
        addrptr[10] = 0xff;
        port = ((sockaddr_in*)(&other))->sin_port;
        break;
      case AF_INET6:
        memcpy(addrptr, &((const sockaddr_in6*)(&other))->sin6_addr, 16);
        port = ((sockaddr_in6*)(&other))->sin6_port;
        break;
        // TODO : sockaddr_ll
      default:
        break;
      }
    }
    
    bool operator < (const Addr & other) const
    {
      return af < other.af && addr < other.addr && port < other.port;
    }
    
  };
}

#endif
