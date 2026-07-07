#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>

// --- 1. MACROS DE VERSIONAMENTO ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

// --- 2. CORREÇÃO DO __printf ---
#ifdef __printf
#undef __printf
#endif
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- 3. DEFINIÇÕES DE HARDWARE ---
#ifndef RTL_MAC80211_NUM_QUEUE
#define RTL_MAC80211_NUM_QUEUE 4
#endif

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

// --- 4. ENUMS FALTANTES ---
enum nl80211_iftype {
    NL80211_IFTYPE_UNSPECIFIED,
    NL80211_IFTYPE_ADHOC,
    NL80211_IFTYPE_STATION,
    NL80211_IFTYPE_AP,
    NL80211_IFTYPE_MESH_POINT,
    NL80211_IFTYPE_P2P_CLIENT,
    NL80211_IFTYPE_P2P_GO,
    NL80211_IFTYPE_P2P_DEVICE,
};

enum nl80211_channel_type {
    NL80211_CHAN_NO_HT,
    NL80211_CHAN_HT20,
    NL80211_CHAN_HT40MINUS,
    NL80211_CHAN_HT40PLUS
};

#define IEEE80211_NUM_BANDS 3
#define NUM_NL80211_BANDS IEEE80211_NUM_BANDS

// --- 5. TIPOS BÁSICOS E COMPATIBILIDADE ---
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

struct ieee80211_supported_band { int dummy; };

#endif // APPLE_LINUX_EMULATION_H
