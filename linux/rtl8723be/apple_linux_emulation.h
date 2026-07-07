#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>

// --- Fixes para erros comuns ---
#define ETH_ALEN 6
#define MAX_TID_COUNT 8
#define RTL_MAC80211_NUM_QUEUE 4
#define IEEE80211_NUM_BANDS 3
#define NUM_NL80211_BANDS IEEE80211_NUM_BANDS

// --- Tipos que faltam no macOS/Kernel ---
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef long long time64_t;
typedef unsigned long dma_addr_t;

// --- Macros essenciais ---
#ifndef __packed
#define __packed __attribute__((packed))
#endif

#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

// --- Correção da Macro __printf ---
#ifdef __printf
#undef __printf
#endif
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- Estruturas (Dummy) ---
struct mutex { int dummy; };
struct sk_buff { void *data; };
struct sk_buff_head { int dummy; };
struct timer_list { int dummy; };
struct tasklet_struct { int dummy; };
struct ieee80211_tx_queue_params { int dummy; };
struct ieee80211_hdr { unsigned short frame_control; };
enum nl80211_iftype { NL80211_IFTYPE_STATION = 2 };

// --- I/O e Locks (Stubs) ---
#define spin_lock(lock)
#define spin_unlock(lock)
#define printk printf
#define pr_info(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...) printf(fmt, ##__VA_ARGS__)

#endif
