#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>

// --- 1. MACROS E PROTEÇÕES (PARA EVITAR REDEFINIÇÃO) ---
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

#ifndef NUM_NL80211_BANDS
#define NUM_NL80211_BANDS 3
#endif

#ifndef RTL_MAC80211_NUM_QUEUE
#define RTL_MAC80211_NUM_QUEUE 4
#endif

// --- 2. PRINTK E VERSIONAMENTO ---
#define printk printf
#undef __printf
#define __printf(a, b) __attribute__((format(printf, a, b)))

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif
#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

// --- 3. TIPOS ESTRUTURAIS ---
typedef unsigned char       u8;
typedef signed char         s8;
typedef unsigned short      u16;
typedef signed short        s16;
typedef unsigned int        u32;
typedef signed int          s32;
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

// --- 4. ESTRUTURAS DO KERNEL (STUBS) ---
struct mutex { int dummy; };
struct seq_file { int dummy; };
struct wiphy { int dummy; };
struct regulatory_request { int dummy; };
struct firmware { int dummy; };
struct ieee80211_supported_band { int dummy; };
struct sk_buff_head { int dummy; };
struct sk_buff { int dummy; };
struct timer_list { int dummy; };
struct urb { int dummy; };
struct ieee80211_tx_queue_params { int dummy; };
struct ieee80211_sta { int dummy; };
struct ieee80211_hdr { int dummy; };
struct ieee80211_tx_info { int dummy; };
struct ieee80211_rx_status { int dummy; };
struct tasklet_struct { int dummy; };
struct delayed_work { int dummy; };
struct work_struct { int dummy; };
struct completion { int dummy; };

// Estrutura principal que o driver precisa (adicionado o 'priv')
struct ieee80211_hw { void *priv; }; 

// Enum de interface de rede
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

enum nl80211_channel_type { NL80211_CHAN_NO_HT, NL80211_CHAN_HT20, NL80211_CHAN_HT40MINUS, NL80211_CHAN_HT40PLUS };

// --- 5. LÓGICA DE LISTA ---
#ifndef container_of
#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

struct list_head {
    struct list_head *next, *prev;
};

#define list_for_each_entry(pos, head, member) \
    for (pos = container_of((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = container_of(pos->member.next, typeof(*pos), member))

// --- 6. MACROS DE I/O ---
#define __iomem
#define readb(addr)        (*(volatile u8 *)(addr))
#define readw(addr)        (*(volatile u16 *)(addr))
#define readl(addr)        (*(volatile u32 *)(addr))
#define writeb(val, addr)  (*(volatile u8 *)(addr) = (val))
#define writew(val, addr)  (*(volatile u16 *)(addr) = (val))
#define writel(val, addr)  (*(volatile u32 *)(addr) = (val))

#define GFP_KERNEL 0
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

#endif
