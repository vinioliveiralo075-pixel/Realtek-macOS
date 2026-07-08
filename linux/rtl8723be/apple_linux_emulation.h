#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>

// --- 1. VERSÃO DO KERNEL E MACROS CRÍTICAS ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#ifndef NUM_NL80211_BANDS
#define NUM_NL80211_BANDS 3
#endif

// --- 2. TODOS OS TIPOS BÁSICOS (AGORA COMPLETO!) ---
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

typedef unsigned short      __le16;
typedef unsigned int        __le32;
typedef unsigned long long  __le64;

typedef long long           time64_t;
typedef unsigned long       dma_addr_t;

// --- 3. CORREÇÃO DA MACRO __printf ---
#ifdef __printf
#undef __printf
#endif
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- 4. ESTRUTURAS E ENUMS (DUMMIES) ---
struct list_head { struct list_head *next, *prev; };
struct mutex { int dummy; };
struct sk_buff { void *data; };
struct sk_buff_head { int dummy; };
struct timer_list { int dummy; };
struct tasklet_struct { int dummy; };
struct ieee80211_tx_queue_params { int dummy; };
struct ieee80211_hdr { unsigned short frame_control; };
struct ieee80211_supported_band { int dummy; };
enum nl80211_iftype { NL80211_IFTYPE_STATION = 2, NL80211_IFTYPE_AP = 3 };

// --- 5. STUBS DE FUNÇÕES ---
#define spin_lock(lock)
#define spin_unlock(lock)
#define printk printf
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

#endif // APPLE_LINUX_EMULATION_H
