#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>

// --- 1. MACROS DE PROTEÇÃO ---
#ifndef MAX_TID_COUNT
#define MAX_TID_COUNT 8
#endif

#ifndef __packed
#define __packed __attribute__((packed))
#endif

// --- 2. TIPOS DE DADOS ESSENCIAIS (O que o compilador pediu) ---
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

// --- 3. CORREÇÃO DO __printf ---
// Essa macro estava quebrando a sintaxe do seu debug.h
#ifdef __printf
#undef __printf
#endif
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- 4. ESTRUTURAS FALTANTES ---
struct sk_buff { void *data; };

// --- 5. STUBS DE FUNÇÕES ---
static inline void *skb_put(struct sk_buff *skb, unsigned int len) { return skb->data; }

// --- 6. I/O E LOCKS ---
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
