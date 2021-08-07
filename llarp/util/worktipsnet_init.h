#ifndef LLARP_UTIL_WORKTIPSNET_INIT_H
#define LLARP_UTIL_WORKTIPSNET_INIT_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef Worktipsnet_INIT
#if defined(_WIN32)
#define Worktipsnet_INIT \
  DieInCaseSomehowThisGetsRunInWineButLikeWTFThatShouldNotHappenButJustInCaseHandleItWithAPopupOrSomeShit
#else
#define Worktipsnet_INIT _worktipsnet_non_shit_platform_INIT
#endif
#endif

  int
  Worktipsnet_INIT(void);

#ifdef __cplusplus
}
#endif
#endif