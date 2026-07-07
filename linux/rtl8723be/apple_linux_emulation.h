#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>

// --- 1. DEFINIÇÕES BASE ---
struct list_head {
    struct list_head *next, *prev;
};

#ifndef offsetof
#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)
#endif

#define __packed __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))

#ifndef BIT
#define BIT(x) (1ULL << (x))
#endif

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#ifndef IEEE80211_NUM_BANDS
#define IEEE80211_NUM_BANDS 3
#endif

#ifndef RTL_MAC80211_NUM_QUEUE
#define RTL_MAC80211_NUM_QUEUE 4
#endif

#define IEEE80211_QOS_CTL_TID_MASK 0x000F

// --- 2. TIPOS ESTRUTURAIS ---
typedef unsigned char       u8;
typedef signed char         s8;
typedef unsigned short      u16;
typedef signed short        s16;
typedef unsigned int        u32;
typedef signed int          s32;
typedef int                 __s32;
typedef unsigned long long  u64;
typedef signed long long    s64;
typedef u64                 dma_addr_t;
typedef long long           time64_t;

typedef u16 __le16;
typedef u32 __le32;
typedef u64 __le64;
typedef u16 __be16;
typedef u32 __be32;
typedef u64 __be64;

typedef struct { int counter; } atomic_t;
typedef struct { int dummy; } spinlock_t;

// --- 3. STUBS DE ESTRUTURAS ---
struct sk_buff { void *data; };
struct ieee80211_hdr { u16 frame_control; };
struct mutex { int dummy; };
struct seq_file { int dummy; };
struct wiphy { int dummy; };
struct regulatory_request { int dummy; };
struct firmware { int dummy; };
struct ieee80211_supported_band { int dummy; };
struct sk_buff_head { int dummy; };
struct timer_list { int dummy; };
struct urb { int dummy; };
struct ieee80211_tx_queue_params { int dummy; };
struct ieee80211_sta { int dummy; };
struct ieee80211_tx_info { int dummy; };
struct ieee80211_rx_status { int dummy; };
struct tasklet_struct { int dummy; };
struct delayed_work { int dummy; };
struct work_struct { int dummy; };
struct completion { int dummy; };
struct pci_device_id { int dummy; };
struct ieee80211_hw { void *priv; }; 

// --- 4. FUNÇÕES DE SUPORTE (STUBS) ---
static inline void *skb_put(struct sk_buff *skb, unsigned int len) { return skb->data; }
static inline void do_gettimeofday(struct timeval *tv) { tv->tv_sec = 0; tv->tv_usec = 0; }
static inline u16 *ieee80211_get_qos_ctl(struct ieee80211_hdr *h) { static u16 tmp = 0; return &tmp; }
static inline void *ieee80211_find_sta(void *v, const u8 *b) { return NULL; }

// --- 5. LOCKS E I/O ---
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
