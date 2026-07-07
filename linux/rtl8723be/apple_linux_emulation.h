#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>

// --- 1. MACROS ESSENCIAIS ---
#ifndef offsetof
#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)
#endif

#define __packed __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))
#define BIT(x) (1ULL << (x))
#define ETH_ALEN 6
#define NUM_NL80211_BANDS 3

// --- 2. VERSIONAMENTO E PRINTF ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif
#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

// Forçar a definição de __printf para evitar que o compilador a veja como um tipo
#undef __printf
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- 3. TIPOS BÁSICOS ---
typedef unsigned char       u8;
typedef signed char         s8;
typedef unsigned short      u16;
typedef signed short        s16;
typedef unsigned int        u32;
typedef signed int          s32;
typedef unsigned long long  u64;
typedef signed long long    s64;
typedef u64                 dma_addr_t;

typedef u16 __le16;
typedef u32 __le32;
typedef u64 __le64;
typedef u16 __be16;
typedef u32 __be32;
typedef u64 __be64;

// --- 4. LISTAS E ESTRUTURAS ---
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

// --- 5. STUBS PARA ESTRUTURAS (COMPATIBILIDADE) ---
struct mutex { int dummy; };
struct seq_file { int dummy; };
struct wiphy { int dummy; };
struct regulatory_request { int dummy; };
struct firmware { int dummy; };
struct ieee80211_supported_band { int dummy; };
struct sk_buff_head { int dummy; };

// --- 6. ENUMS E LOCKS ---
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

#define spin_lock_bh(lock)
#define spin_unlock_bh(lock)
#define rcu_read_lock()
#define rcu_read_unlock()

// --- 7. I/O E LOGS ---
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

#endif // APPLE_LINUX_EMULATION_H
