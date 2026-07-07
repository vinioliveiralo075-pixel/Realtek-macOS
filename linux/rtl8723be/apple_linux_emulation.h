#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>

// --- 1. TIPOS BÁSICOS DO KERNEL ---
typedef unsigned char       u8;
typedef signed char         s8;
typedef unsigned short      u16;
typedef signed short        s16;
typedef unsigned int        u32;
typedef signed int          s32;
typedef unsigned long long  u64;
typedef signed long long    s64;
typedef u64                 dma_addr_t;

// --- 2. ESTRUTURAS DE LISTA E MEMÓRIA (MÁGICA DO KERNEL) ---
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})

struct list_head {
    struct list_head *next, *prev;
};

#define list_for_each_entry(pos, head, member) \
    for (pos = container_of((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = container_of(pos->member.next, typeof(*pos), member))

// --- 3. ENUMS DE REDE (NL80211) ---
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

// --- 4. DUMMIES PARA SILENCIAR ERROS ---
struct seq_file {};
struct wiphy {};
struct regulatory_request {};
struct firmware {};

// --- 5. SINCRONIZAÇÃO (STUBS - EMULAÇÃO VAZIA) ---
#define spin_lock_bh(lock)
#define spin_unlock_bh(lock)
#define rcu_read_lock()
#define rcu_read_unlock()

// --- 6. MEMÓRIA E I/O ---
#define __iomem
#define readb(addr)        (*(volatile u8 *)(addr))
#define readw(addr)        (*(volatile u16 *)(addr))
#define readl(addr)        (*(volatile u32 *)(addr))
#define writeb(val, addr)  (*(volatile u8 *)(addr) = (val))
#define writew(val, addr)  (*(volatile u16 *)(addr) = (val))
#define writel(val, addr)  (*(volatile u32 *)(addr) = (val))

// --- 7. MISC ---
#define GFP_KERNEL 0
#define IOLog printf
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

#endif // APPLE_LINUX_EMULATION_H
