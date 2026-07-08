#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>

// --- 1. ATRIBUTOS DE MEMÓRIA E ACESSO DE I/O ---
#ifdef __packed
#undef __packed
#endif
#define __packed __attribute__((packed))

#ifdef __aligned
#undef __aligned
#endif
#define __aligned(x) __attribute__((aligned(x)))

#define __iomem

#define readb(addr)        (*(volatile u8 *)(addr))
#define readw(addr)        (*(volatile u16 *)(addr))
#define readl(addr)        (*(volatile u32 *)(addr))
#define writeb(val, addr)  (*(volatile u8 *)(addr) = (val))
#define writew(val, addr)  (*(volatile u16 *)(addr) = (val))
#define writel(val, addr)  (*(volatile u32 *)(addr) = (val))

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
#define IEEE80211_QOS_CTL_TID_MASK 0x000f

// --- 3. TODOS OS TIPOS BÁSICOS DO KERNEL ---
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

typedef struct { volatile int counter; } atomic_t;
typedef struct { int dummy; } spinlock_t;

// --- 4. CORREÇÃO DA MACRO __printf ---
#ifdef __printf
#undef __printf
#endif
#define __printf(a, b) __attribute__((format(printf, a, b)))

// --- 5. PRÉ-DECLARAÇÕES PARA LIMPAR WARNINGS DE VISIBILIDADE ---
enum nl80211_channel_type { NL80211_CONN_LESS_PRIMARY };
enum ieee80211_smps_mode { IEEE80211_SMPS_DISABLED };
struct ieee80211_sta;
struct ieee80211_tx_info;
struct ieee80211_rx_status;
struct seq_file;
struct pci_device_id;
struct urb;
struct firmware;
struct wiphy;
struct regulatory_request;

// --- 6. ESTRUTURAS COMPLETAS DO LINUX ---
struct list_head { struct list_head *next, *prev; };
struct mutex { int dummy; };
struct sk_buff { void *data; };
struct sk_buff_head { int dummy; };
struct timer_list { int dummy; };
struct tasklet_struct { int dummy; };
struct delayed_work { int dummy; };
struct work_struct { int dummy; };
struct completion { int dummy; };
struct ieee80211_tx_queue_params { int dummy; };
struct ieee80211_hdr { unsigned short frame_control; };
struct ieee80211_supported_band { int dummy; };

// Enum expandido com os modos Ad-Hoc e Mesh que o dm.c pede
enum nl80211_iftype { 
    NL80211_IFTYPE_ADHOC = 1,
    NL80211_IFTYPE_STATION = 2, 
    NL80211_IFTYPE_AP = 3,
    NL80211_IFTYPE_MESH_POINT = 7
};

struct ieee80211_hw { void *priv; void *vif; };

// --- 7. STUBS DE FUNÇÕES E MACROS DE LOOP ---
#define spin_lock(lock)
#define spin_unlock(lock)
#define spin_lock_bh(lock)
#define spin_unlock_bh(lock)
#define rcu_read_lock()
#define rcu_read_unlock()

#define printk printf
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

// Burlar o loop de varredura do Linux de forma válida para a sintaxe do C
#define list_for_each_entry(pos, head, member) \
    for (pos = NULL; pos != NULL; )

static inline u8 *ieee80211_get_qos_ctl(const void *hdr) { static u8 dummy[2] = {0}; return dummy; }
static inline struct ieee80211_sta *ieee80211_find_sta(void *vif, const u8 *addr) { return NULL; }

#endif // APPLE_LINUX_EMULATION_H
