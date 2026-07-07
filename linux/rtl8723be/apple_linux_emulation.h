#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>

// --- 1. ATRIBUTOS E MACROS DE COMPATIBILIDADE ---
#ifndef __packed
#define __packed __attribute__((packed))
#endif

#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

// --- 2. TIPOS BÁSICOS DO KERNEL ---
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

typedef long long           time64_t;
typedef unsigned long       dma_addr_t;
typedef struct { int counter; } atomic_t;
typedef struct { int lock; } spinlock_t;

// --- 3. DEFINIÇÕES DE REDE (COM PROTEÇÃO) ---
#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#ifndef MAX_TID_COUNT
#define MAX_TID_COUNT 8
#endif

// --- 4. STRUCTS E ENUMS (FORWARD DECLARATIONS) ---
struct sk_buff { void *data; };
struct sk_buff_head { void *dummy; };
struct ieee80211_tx_queue_params { void *dummy; };
struct ieee80211_sta { void *dummy; };
struct ieee80211_tx_info { void *dummy; };
struct ieee80211_rx_status { void *dummy; };
struct urb { void *dummy; };
struct mutex { void *dummy; };

enum nl80211_iftype {
    NL80211_IFTYPE_STATION = 2,
    NL80211_IFTYPE_AP = 3
};

// --- 5. CORREÇÃO DA MACRO __printf ---
#ifdef __printf
#undef __printf
#endif
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- 6. STUBS DE FUNÇÕES ---
static inline void *skb_put(struct sk_buff *skb, unsigned int len) { return skb->data; }
static inline void do_gettimeofday(struct timeval *tv) { tv->tv_sec = 0; tv->tv_usec = 0; }

// --- 7. I/O E LOCKS ---
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
