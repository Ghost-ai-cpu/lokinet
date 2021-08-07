#include "network_worktips_worktipsnet_WorktipsnetVPN.h"
#include "worktipsnet_jni_vpnio.hpp"
#include "worktipsnet_jni_common.hpp"
#include <net/ip.hpp>

extern "C"
{
  JNIEXPORT jint JNICALL
  Java_network_worktips_worktipsnet_WorktipsnetVPN_PacketSize(JNIEnv *, jclass)
  {
    return llarp::net::IPPacket::MaxSize;
  }

  JNIEXPORT jobject JNICALL
  Java_network_worktips_worktipsnet_WorktipsnetVPN_Alloc(JNIEnv *env, jclass)
  {
    worktipsnet_jni_vpnio *vpn = new worktipsnet_jni_vpnio();
    return env->NewDirectByteBuffer(vpn, sizeof(worktipsnet_jni_vpnio));
  }

  JNIEXPORT void JNICALL
  Java_network_worktips_worktipsnet_WorktipsnetVPN_Free(JNIEnv *env, jclass, jobject buf)
  {
    worktipsnet_jni_vpnio *vpn = FromBuffer< worktipsnet_jni_vpnio >(env, buf);
    if(vpn == nullptr)
      return;
    delete vpn;
  }
  JNIEXPORT void JNICALL
  Java_network_worktips_worktipsnet_WorktipsnetVPN_Stop(JNIEnv *env, jobject self)
  {
    worktipsnet_jni_vpnio *vpn = GetImpl< worktipsnet_jni_vpnio >(env, self);
    if(vpn)
    {
      vpn->Close();
    }
  }

  JNIEXPORT jint JNICALL
  Java_network_worktips_worktipsnet_WorktipsnetVPN_ReadPkt(JNIEnv *env, jobject self,
                                               jobject pkt)
  {
    worktipsnet_jni_vpnio *vpn = GetImpl< worktipsnet_jni_vpnio >(env, self);
    if(vpn == nullptr)
      return -1;
    void *pktbuf = env->GetDirectBufferAddress(pkt);
    auto pktlen  = env->GetDirectBufferCapacity(pkt);
    if(pktbuf == nullptr)
      return -1;
    return vpn->ReadPacket(pktbuf, pktlen);
  }

  JNIEXPORT jboolean JNICALL
  Java_network_worktips_worktipsnet_WorktipsnetVPN_WritePkt(JNIEnv *env, jobject self,
                                                jobject pkt)
  {
    worktipsnet_jni_vpnio *vpn = GetImpl< worktipsnet_jni_vpnio >(env, self);
    if(vpn == nullptr)
      return false;
    void *pktbuf = env->GetDirectBufferAddress(pkt);
    auto pktlen  = env->GetDirectBufferCapacity(pkt);
    if(pktbuf == nullptr)
      return false;
    return vpn->WritePacket(pktbuf, pktlen);
  }

  JNIEXPORT void JNICALL
  Java_network_worktips_worktipsnet_WorktipsnetVPN_SetInfo(JNIEnv *env, jobject self,
                                               jobject info)
  {
    worktipsnet_jni_vpnio *vpn = GetImpl< worktipsnet_jni_vpnio >(env, self);
    if(vpn == nullptr)
      return;
    VisitObjectMemberStringAsStringView< bool >(
        env, info, "ifaddr", [vpn](llarp::string_view val) -> bool {
          vpn->SetIfAddr(val);
          return true;
        });
    VisitObjectMemberStringAsStringView< bool >(
        env, info, "ifname", [vpn](llarp::string_view val) -> bool {
          vpn->SetIfName(val);
          return true;
        });
    vpn->info.netmask = GetObjectMemberAsInt< uint8_t >(env, info, "netmask");
  }
}