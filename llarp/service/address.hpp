#ifndef LLARP_SERVICE_ADDRESS_HPP
#define LLARP_SERVICE_ADDRESS_HPP

#include <dht/key.hpp>
#include <router_id.hpp>
#include <util/aligned.hpp>

#include <functional>
#include <numeric>
#include <string>
#include <set>

namespace llarp
{
  namespace service
  {
    /// Snapp Address
    struct Address : public AlignedBuffer< 32 >
    {
      /// if parsed using FromString this contains the subdomain
      /// this member is not used when comparing it's extra data for dns
      std::string subdomain;

      /// list of whitelisted gtld to permit
      static const std::set< std::string > AllowedTLDs;

      /// return true if we permit using this tld
      /// otherwise return false
      static bool
      PermitTLD(const char* tld);

      std::string
      ToString(const char* tld = ".worktips") const;

      bool
      FromString(const std::string& str, const char* tld = ".worktips");

      Address() : AlignedBuffer< 32 >()
      {
      }

      explicit Address(const Data& buf) : AlignedBuffer< 32 >(buf)
      {
      }

      Address(const Address& other)
          : AlignedBuffer< 32 >(other.as_array()), subdomain(other.subdomain)
      {
      }

      explicit Address(const AlignedBuffer< 32 >& other)
          : AlignedBuffer< 32 >(other)
      {
      }

      bool
      operator<(const Address& other) const
      {
        return as_array() < other.as_array();
      }

      friend std::ostream&
      operator<<(std::ostream& out, const Address& self)
      {
        return out << self.ToString();
      }

      bool
      operator==(const Address& other) const
      {
        return as_array() == other.as_array();
      }

      bool
      operator!=(const Address& other) const
      {
        return as_array() != other.as_array();
      }

      Address&
      operator=(const Address& other) = default;

      dht::Key_t
      ToKey() const;

      RouterID
      ToRouter() const
      {
        return RouterID(as_array());
      }

      struct Hash
      {
        size_t
        operator()(const Address& buf) const
        {
          return std::accumulate(buf.begin(), buf.end(), 0,
                                 std::bit_xor< size_t >());
        }
      };
    };

  }  // namespace service
}  // namespace llarp

#endif
