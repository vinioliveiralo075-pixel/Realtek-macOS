#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>

// --- FIX ESSENCIAL: Definições de Kernel que o wifi.h exige de cara ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

// --- Endianness Types ---
typedef unsigned short __le16;
typedef unsigned int   __le32;
typedef unsigned long long __le64;

// --- Proteção contra redefinições futuras ---
#ifndef MAX_TID_COUNT
#define MAX_TID_COUNT 8
#endif

#ifndef RTL_MAC80211_NUM_QUEUE
#define RTL_MAC80211_NUM_QUEUE 4
#endif

#ifndef IEEE80211_NUM_BANDS
#define IEEE80211_NUM_BANDS 3
#endif

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

// --- Tipos básicos ---
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// --- Macros de utilidade ---
#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

#define __packed __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))

// --- Structs (Dummies para satisfazer o compilador) ---
struct mutex { int dummy; };
struct sk_buff { void *data; };
struct sk_buff_head { int dummy; };
struct timer_list { int dummy; };
struct tasklet_struct { int dummy; };
struct ieee80211_tx_queue_params { int dummy; };
struct ieee80211_hdr { unsigned short frame_control; };
struct ieee80211_supported_band { int dummy; };
enum nl80211_iftype { NL80211_IFTYPE_STATION = 2 };

// --- I/O Stubs ---
#define spin_lock(lock)
#define spin_unlock(lock)
#define printk printf
#define pr_info(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...) printf(fmt, ##__VA_ARGS__)

#endif
