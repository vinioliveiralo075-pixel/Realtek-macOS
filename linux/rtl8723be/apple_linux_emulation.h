#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>
#include <sys/errno.h>

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

// --- 2. KERNEL VERSION, TIMERS & UTILS ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#define ETH_ALEN 6
#define NUM_NL80211_BANDS 3
#define IEEE80211_QOS_CTL_TID_MASK 0x000f

#ifndef EOPNOTSUPP
#define EOPNOTSUPP ENOTSUP  
#endif

// Emulação do tempo do Linux (Jiffies e Delays)
#define jiffies 0UL
#define mdelay(x)          IODelay((x) * 1000)
#define udelay(x)          IODelay(x)

// Máscaras de DMA PCI
#define DMA_BIT_MASK(n)    (((n) == 64) ? ~0ULL : ((1ULL << (n)) - 1))
#define PCI_DMA_TODEVICE   1

// Local IRQ Flags
#define local_save_flags(flags) do { (void)(flags); } while(0)
#define local_irq_enable()      do { } while(0)
#define local_irq_restore(flags) do { (void)(flags); } while(0)

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

// --- 5. PRÉ-DECLARAÇÕES E ENUMS DO SUBSISTEMA DE REDE ---
enum nl80211_channel_type { NL80211_CONN_LESS_PRIMARY };
enum ieee80211_smps_mode { IEEE80211_SMPS_DISABLED };
struct ieee80211_tx_info;
struct ieee80211_rx_status;
struct seq_file;
struct pci_device_id;
struct urb;
struct firmware;
struct wiphy;
struct regulatory_request;

enum nl80211_iftype { 
    NL80211_IFTYPE_UNSPECIFIED = 0,
    NL80211_IFTYPE_ADHOC = 1,
    NL80211_IFTYPE_STATION = 2, 
    NL80211_IFTYPE_AP = 3,
    NL80211_IFTYPE_MESH_POINT = 7
};

// Constantes de Capacidade HT (High Throughput)
#define IEEE80211_HT_CAP_SUP_WIDTH_20_40 0x0002
#define IEEE80211_HT_CAP_SGI_40          0x0020
#define IEEE80211_HT_CAP_SGI_20          0x0010

// --- 6. ESTRUTURAS COMPLETAS DO LINUX ---
struct list_head { struct list_head *next, *prev; };
struct mutex { int dummy; };

struct sk_buff { 
    void *data; 
    unsigned int len; 
};

struct sk_buff_head { int dummy; };
struct timer_list { int dummy; };
struct tasklet_struct { int dummy; };
struct delayed_work { int dummy; };
struct work_struct { int dummy; };
struct completion { int dummy; };
struct ieee80211_tx_queue_params { int dummy; };
struct ieee80211_hdr { unsigned short frame_control; };
struct ieee80211_supported_band { int dummy; };

struct ieee80211_hw { void *priv; void *vif; };

// Definições internas de Station corrigidas com suporte a sub-estruturas MCS
struct ieee80211_mcs_cap {
    u8 rx_mask[16];
};

struct ieee80211_ht_cap {
    u32 cap;
    struct ieee80211_mcs_cap mcs; // Resolve os erros das linhas 2363 e 2364
};

struct ieee80211_sta {
    struct ieee80211_ht_cap ht_cap;
    void *drv_priv;
    u16 aid;
    u32 supp_rates[16];
};

// --- 7. STUBS DE FUNÇÕES, ALOCADORES E MEMÓRIA ---
#define GFP_KERNEL 0
static inline void *kzalloc(size_t size, int flags) { return IOMallocZero(size); }
#define kfree(ptr) do { if (ptr) { (void)(ptr); } } while(0)

#define spin_lock(lock)
#define spin_unlock(lock)
#define spin_lock_bh(lock)
#define spin_unlock_bh(lock)
#define rcu_read_lock()
#define rcu_read_unlock()

#define spin_lock_irqsave(lock, flags) do { (void)(flags); } while(0)
#define spin_unlock_irqrestore(lock, flags) do { (void)(flags); } while(0)

#define WARN_ONCE(cond, fmt, ...) do { (void)(cond); } while(0)
#define ether_addr_copy(dst, src) memcpy(dst, src, ETH_ALEN)

#define printk printf
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

#define list_for_each_entry(pos, head, member) \
    for (pos = NULL; pos != NULL; )

static inline u8 *ieee80211_get_qos_ctl(const void *hdr) { static u8 dummy[2] = {0}; return dummy; }
static inline struct ieee80211_sta *ieee80211_find_sta(void *vif, const u8 *addr) { return NULL; }

static inline struct sk_buff *dev_alloc_skb(unsigned int length) { return NULL; }
static inline void skb_put_data(struct sk_buff *skb, const void *data, unsigned int len) { }
static inline int skb_queue_len(const struct sk_buff_head *list) { return 0; }
static inline struct sk_buff *__skb_dequeue(struct sk_buff_head *list) { return NULL; }
static inline void kfree_skb(struct sk_buff *skb) { }

static inline int mod_timer(struct timer_list *timer, unsigned long expires) { return 0; }
static inline unsigned long msecs_to_jiffies(const unsigned int m) { return m; }

static inline void pci_unmap_single(void *pdev, dma_addr_t dma_addr, size_t size, int direction) { }

#endif // APPLE_LINUX_EMULATION_H
