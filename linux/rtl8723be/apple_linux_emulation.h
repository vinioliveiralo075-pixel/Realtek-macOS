#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>

// --- 1. MACROS E PROTEÇÕES ---
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

// Evitar redefinição com o wifi.h
#ifndef RTL_MAC80211_NUM_QUEUE
#define RTL_MAC80211_NUM_QUEUE 4
#endif

#define IEEE80211_QOS_CTL_TID_MASK 0x000F

// --- 2. PRINTK E VERSIONAMENTO ---
#define printk printf
#undef __printf
#define __printf(a, b) __attribute__((format(printf, a, b)))

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
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

// --- 4. ESTRUTURAS DO KERNEL (COM CAMPOS NECESSÁRIOS) ---
struct sk_buff { void *data; };
struct ieee80211_hdr { u16 frame_control; };

// Stubs simples
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

// --- 5. FUNÇÕES FALSAS (STUBS) ---
static inline u16 *ieee80211_get_qos_ctl(struct ieee80211_hdr *h) { static u16 tmp = 0; return &tmp; }
static inline void *ieee80211_find_sta(void *v, u8 *b) { return NULL; }
static inline struct ieee80211_hdr *rtl_get_hdr(struct sk_buff *skb) { return (struct ieee80211_hdr *)skb->data; }

// --- 6. LOCKS E SINCRONIZAÇÃO (MACROS VAZIAS) ---
#define spin_lock(lock)
#define spin_unlock(lock)
#define spin_lock_bh(lock)
#define spin_unlock_bh(lock)
#define rcu_read_lock()
#define rcu_read_unlock()

// --- 7. LOGS E I/O ---
#define GFP_KERNEL 0
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

#endif
