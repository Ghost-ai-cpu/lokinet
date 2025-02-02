#ifndef LLARP_DHT_EXPLORENETWORKJOB
#define LLARP_DHT_EXPLORENETWORKJOB

#include <dht/tx.hpp>
#include <router_id.hpp>

namespace llarp
{
  namespace dht
  {
    struct ExploreNetworkJob : public TX< RouterID, RouterID >
    {
      ExploreNetworkJob(const RouterID &peer, AbstractContext *ctx)
          : TX< RouterID, RouterID >(TXOwner{}, peer, ctx)
      {
      }

      bool
      Validate(const RouterID &) const override
      {
        // TODO: check with worktipsd
        return true;
      }

      void
      Start(const TXOwner &peer) override;

      void
      SendReply() override;
    };
  }  // namespace dht
}  // namespace llarp

#endif
