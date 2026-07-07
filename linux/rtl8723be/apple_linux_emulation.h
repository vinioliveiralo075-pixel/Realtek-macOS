#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>

// --- 1. TIPOS BÁSICOS (SINAIS E ENDIANESS) ---
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

typedef u16 __le16;
typedef u32 __le32;
typedef u64 __le64;
typedef u16 __be16;
typedef u32 __be32;
typedef u64 __be64;

// --- 2. MACROS DE VERSIONAMENTO E PROTEÇÃO ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

#ifndef RTL_MAC80211_NUM_QUEUE
#define RTL_MAC80211_NUM_QUEUE 4
#endif

#ifndef IEEE80211_NUM_BANDS
#define IEEE80211_NUM_BANDS 3
#endif

#define NUM_NL80211_BANDS IEEE80211_NUM_BANDS

// --- 3. CORREÇÃO DO __printf ---
#ifdef __printf
#undef __printf
#endif
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- 4. ENUMS E ESTRUTURAS ---
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

struct ieee80211_supported_band { int dummy; };
struct list_head { struct list_head *next, *prev; };
struct sk_buff { void *data; };
struct ieee80211_hdr { u16 frame_control; };

// --- 5. STUBS DE FUNÇÕES ---
static inline void *skb_put(struct sk_buff *skb, unsigned int len) { return skb->data; }
static inline void do_gettimeofday(struct timeval *tv) { tv->tv_sec = 0; tv->tv_usec = 0; }
static inline u16 *ieee80211_get_qos_ctl(struct ieee80211_hdr *h) { static u16 tmp = 0; return &tmp; }
static inline void *ieee80211_find_sta(void *v, const u8 *b) { return NULL; }

// --- 6. LOCKS E I/O ---
#define spin_lock(lock)
#define spin_unlock(lock)
#define spin_lock_bh(lock)
#define spin_unlock_bh(lock)
#define rcu_read_lock()
#define rcu_read_unlock()

#define __iomem
#define readb(addr)        (*(volatile u8 *)(addr))
#define readw(addr)        (*(volatile u16 *)(addr))
#define readl(addr)        (*(volatile u32 *)(addr))
#define writeb(val, addr)  (*(volatile u8 *)(addr) = (val))
#define writew(val, addr)  (*(volatile u16 *)(addr) = (val))
#define writel(val, addr)  (*(volatile u32 *)(addr) = (val))

#define GFP_KERNEL 0
#define printk printf
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

#endif
