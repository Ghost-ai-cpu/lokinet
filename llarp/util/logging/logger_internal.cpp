#include <util/logging/logger_internal.hpp>

#include <date/date.h>

namespace llarp
{
  std::ostream&
  operator<<(std::ostream& out, const log_timestamp& ts)
  {
    std::chrono::milliseconds delta{ts.delta};
    auto h = std::chrono::duration_cast< std::chrono::hours >(delta);
    delta -= h;
    auto m = std::chrono::duration_cast< std::chrono::minutes >(delta);
    delta -= m;
    auto s = std::chrono::duration_cast< std::chrono::seconds >(delta);
    delta -= s;
    llarp_time_t ms = delta;
    std::chrono::time_point< std::chrono::system_clock,
                             std::chrono::milliseconds >
        now{std::chrono::milliseconds{ts.now}};
    return date::operator<<(out, now) << " UTC [+" << ms << "]";
  }
}  // namespace llarp
