#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>

// --- 1. RESET DE ATRIBUTOS DO COMPILADOR (O Grande Erro!) ---
// Remove os clones do macOS e força a sintaxe correta do GCC/Linux
#ifdef __packed
#undef __packed
#endif
#define __packed __attribute__((packed))

#ifdef __aligned
#undef __aligned
#endif
#define __aligned(x) __attribute__((aligned(x)))

// --- 2. KERNEL VERSION & MACROS ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

#define ETH_ALEN 6
#define NUM_NL80211_BANDS 3

// --- 3. TODOS OS TIPOS BÁSICOS E ATÔMICOS ---
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

typedef unsigned short      __le16;
typedef unsigned int        __le32;
typedef unsigned long long  __le64;

typedef long long           time64_t;
typedef unsigned long       dma_addr_t;

// Tipos de sincronização do Linux que o wifi.h exige
typedef struct { volatile int counter; } atomic_t;
typedef struct { int dummy; } spinlock_t;

// --- 4. CORREÇÃO DA MACRO __printf ---
#ifdef __printf
#undef __printf
#endif
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- 5. ESTRUTURAS E ENUMS (DUMMIES) ---
struct list_head { struct list_head *next, *prev; };
struct mutex { int dummy; };
struct sk_buff { void *data; };
struct sk_buff_head { int dummy; };
struct timer_list { int dummy; };
struct tasklet_struct { int dummy; };
struct delayed_work { int dummy; }; // Corrigido: Incomplete type estrutural
struct ieee80211_tx_queue_params { int dummy; };
struct ieee80211_hdr { unsigned short frame_control; };
struct ieee80211_supported_band { int dummy; };
enum nl80211_iftype { NL80211_IFTYPE_STATION = 2, NL80211_IFTYPE_AP = 3 };

// --- 6. STUBS DE FUNÇÕES ---
#define spin_lock(lock)
#define spin_unlock(lock)
#define printk printf
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

#endif // APPLE_LINUX_EMULATION_H
