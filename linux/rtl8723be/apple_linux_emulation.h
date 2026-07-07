#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>

// --- 1. MACROS E DEFINIÇÕES BÁSICAS ---
#ifndef __packed
#define __packed __attribute__((packed))
#endif

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

// --- 2. TIPOS DE DADOS ---
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

// --- 3. ESTRUTURAS DO KERNEL ---
struct list_head { struct list_head *next, *prev; };
struct sk_buff { void *data; };
struct ieee80211_hdr { u16 frame_control; };
struct mutex { int dummy; };
struct sk_buff_head { int dummy; };
struct ieee80211_supported_band { int dummy; };

// --- 4. DEFINIÇÕES DE REDE/HARDWARE ---
#ifndef MAX_TID_COUNT
#define MAX_TID_COUNT 8
#endif

#ifndef RTL_MAC80211_NUM_QUEUE
#define RTL_MAC80211_NUM_QUEUE 4
#endif

#ifndef IEEE80211_NUM_BANDS
#define IEEE80211_NUM_BANDS 3
#endif

#ifndef NUM_NL80211_BANDS
#define NUM_NL80211_BANDS IEEE80211_NUM_BANDS
#endif

// --- 5. STUBS E FUNÇÕES DE COMPATIBILIDADE ---
static inline void *skb_put(struct sk_buff *skb, unsigned int len) { return skb->data; }
static inline void do_gettimeofday(struct timeval *tv) { tv->tv_sec = 0; tv->tv_usec = 0; }

// --- 6. LOCKS E I/O (MACROS) ---
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

#endif // APPLE_LINUX_EMULATION_H
