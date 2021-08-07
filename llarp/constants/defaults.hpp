#ifndef LLARP_DEFAULTS_HPP
#define LLARP_DEFAULTS_HPP

#ifndef DEFAULT_RESOLVER_US
#define DEFAULT_RESOLVER_US "1.1.1.1"
#endif
#ifndef DEFAULT_RESOLVER_EU
#define DEFAULT_RESOLVER_EU "1.1.1.1"
#endif
#ifndef DEFAULT_RESOLVER_AU
#define DEFAULT_RESOLVER_AU "1.1.1.1"
#endif

#ifdef DEBIAN
#ifndef DEFAULT_WORKTIPSNET_USER
#define DEFAULT_WORKTIPSNET_USER "debian-worktipsnet"
#endif
#ifndef DEFAULT_WORKTIPSNET_GROUP
#define DEFAULT_WORKTIPSNET_GROUP "debian-worktipsnet"
#endif
#else
#ifndef DEFAULT_WORKTIPSNET_USER
#define DEFAULT_WORKTIPSNET_USER "worktipsnet"
#endif
#ifndef DEFAULT_WORKTIPSNET_GROUP
#define DEFAULT_WORKTIPSNET_GROUP "worktipsnet"
#endif
#endif

#endif
