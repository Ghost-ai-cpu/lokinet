#ifndef BOOTSERV_WORKTIPSNET_CRON_HPP
#define BOOTSERV_WORKTIPSNET_CRON_HPP

#include "handler.hpp"

namespace worktipsnet
{
  namespace bootserv
  {
    struct CGIHandler final : public Handler
    {
      CGIHandler(std::ostream& o);
      ~CGIHandler();

      int
      Exec(const Config& conf) override;

      int
      ReportError(const char* err) override;

      int
      ServeFile(const char* fname, const char* mime) const;

      std::string
      PickRandomFileInDir(std::string dirname) const;
    };

  }  // namespace bootserv
}  // namespace worktipsnet

#endif
