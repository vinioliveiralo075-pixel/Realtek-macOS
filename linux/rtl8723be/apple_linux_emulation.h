#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>

// --- 1. DEFINIÇÕES CRÍTICAS (PARA O WIFI.H) ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

// --- 2. TIPOS DE ENDIANNESS ---
typedef unsigned short __le16;
typedef unsigned int   __le32;
typedef unsigned long long __le64;

// --- 3. TIPOS BÁSICOS ---
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

// --- 4. PREVENÇÃO DE REDEFINIÇÃO ---
#ifndef MAX_TID_COUNT
#define MAX_TID_COUNT 8
#endif

#ifndef __packed
#define __packed __attribute__((packed))
#endif

#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif

// --- 5. ESTRUTURAS BÁSICAS (DUMMIES) ---
struct sk_buff { void *data; };
struct list_head { struct list_head *next, *prev; };
struct timer_list { int dummy; };
struct tasklet_struct { int dummy; };
struct ieee80211_supported_band { int dummy; };
struct ieee80211_hdr { unsigned short frame_control; };

// --- 6. MACROS DE I/O E LOCKS ---
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
