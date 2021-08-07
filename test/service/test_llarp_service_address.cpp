#include <service/address.hpp>

#include <gtest/gtest.h>

struct ServiceAddressTest : public ::testing::Test
{
  const std::string snode =
      "8zfiwpgonsu5zpddpxwdurxyb19x6r96xy4qbikff99jwsziws9y.snode";
  const std::string worktips =
      "7okic5x5do3uh3usttnqz9ek3uuoemdrwzto1hciwim9f947or6y.worktips";
  const std::string sub = "worktipsnet.test";
  const std::string invalid =
      "7okic5x5do3uh3usttnqz9ek3uuoemdrwzto1hciwim9f947or6y.net";
};

TEST_F(ServiceAddressTest, TestParseBadTLD)
{
  llarp::service::Address addr;
  ASSERT_FALSE(addr.FromString(snode, ".net"));
  ASSERT_FALSE(addr.FromString(invalid, ".net"));
}

TEST_F(ServiceAddressTest, TestParseBadTLDAppenedOnEnd)
{
  llarp::service::Address addr;
  const std::string bad = worktips + ".net";
  ASSERT_FALSE(addr.FromString(bad, ".net"));
}

TEST_F(ServiceAddressTest, TestParseBadTLDAppenedOnEndWithSubdomain)
{
  llarp::service::Address addr;
  const std::string bad = sub + "." + worktips + ".net";
  ASSERT_FALSE(addr.FromString(bad, ".net"));
}

TEST_F(ServiceAddressTest, TestParseSNodeNotWorktips)
{
  llarp::service::Address addr;
  ASSERT_TRUE(addr.FromString(snode, ".snode"));
  ASSERT_FALSE(addr.FromString(snode, ".worktips"));
}

TEST_F(ServiceAddressTest, TestParseWorktipsNotSNode)
{
  llarp::service::Address addr;
  ASSERT_FALSE(addr.FromString(worktips, ".snode"));
  ASSERT_TRUE(addr.FromString(worktips, ".worktips"));
}

TEST_F(ServiceAddressTest, TestParseWorktipsWithSubdomain)
{
  llarp::service::Address addr;
  const std::string addr_str = sub + "." + worktips;
  ASSERT_TRUE(addr.FromString(addr_str, ".worktips"));
  ASSERT_EQ(addr.subdomain, sub);
  ASSERT_EQ(addr.ToString(), addr_str);
};

TEST_F(ServiceAddressTest, TestParseSnodeWithSubdomain)
{
  llarp::service::Address addr;
  const std::string addr_str = sub + "." + snode;
  ASSERT_TRUE(addr.FromString(addr_str, ".snode"));
  ASSERT_EQ(addr.subdomain, sub);
  ASSERT_EQ(addr.ToString(".snode"), addr_str);
};
