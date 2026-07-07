#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h> // O coração do Kernel do macOS

// === 1. DICIONÁRIO DE TIPOS: Traduzindo o "idioma" do Linux para o Mac ===
typedef unsigned char       u8;
typedef signed char         s8;
typedef unsigned short      u16;
typedef signed short        s16;
typedef unsigned int        u32;
typedef signed int          s32;
typedef unsigned long long  u64;
typedef signed long long    s64;

// Tipos de Endianness e Hardware
typedef u16 __le16;
typedef u32 __le32;
typedef u64 __le64;
typedef u16 __be16; 
typedef u32 __be32;
typedef u64 __be64;
typedef u64 dma_addr_t;
typedef long long time64_t;

// Remove o modificador __iomem exclusivo do compilador do Linux
#define __iomem

// === 2. EMULAÇÃO DE LEITURA E ESCRITA PCI (MMIO) ===
// Transforma os comandos do Linux em acessos diretos de ponteiro aceitos pelo Mac
#define readb(addr)        (*(volatile u8 *)(addr))
#define readw(addr)        (*(volatile u16 *)(addr))
#define readl(addr)        (*(volatile u32 *)(addr))
#define writeb(val, addr)  (*(volatile u8 *)(addr) = (val))
#define writew(val, addr)  (*(volatile u16 *)(addr) = (val))
#define writel(val, addr)  (*(volatile u32 *)(addr) = (val))

// === 3. FERRAMENTAS DO COMPILADOR E REDE DO LINUX ===
#define __packed __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))
#define BIT(x) (1ULL << (x))
#define ETH_ALEN 6
#define NUM_NL80211_BANDS 3
#define RTL_MAC80211_NUM_QUEUE 5

#define IEEE80211_QOS_CTL_TID_MASK 0x000f

// === 4. ESTRUTURAS DE CONTROLE E CONCORRÊNCIA ===
struct list_head {
    struct list_head *next, *prev;
};

struct mutex {
    void* owner;
};

typedef struct {
    IOSimpleLock* lock;
} spinlock_t;

typedef struct {
    volatile int counter;
} atomic_t;

struct timer_list {
    void *function;
    unsigned long expires;
    unsigned long data;
};

struct tasklet_struct {
    struct tasklet_struct *next;
    unsigned long state;
    atomic_t count;
    void (*func)(unsigned long);
    unsigned long data;
};

struct work_struct {
    atomic_t data;
    struct list_head entry;
    void (*func)(struct work_struct *work);
};

struct delayed_work {
    struct work_struct work;
    struct timer_list timer;
};

struct completion {
    unsigned int done;
};

// === 5. DECLARAÇÕES E ESTRUTURAS DE REDE ===
struct ieee80211_supported_band {};
enum nl80211_iftype { NL80211_IFTYPE_UNSPECIFIED };
enum nl80211_channel_type { NL80211_CHAN_NO_HT };
struct sk_buff_head {};
struct ieee80211_tx_queue_params {};
struct ieee80211_sta {};
struct ieee80211_hdr {
    u16 frame_control;
};
struct sk_buff {
    unsigned char *data;
};
struct ieee80211_tx_info {};
struct ieee80211_rx_status {};
struct urb {};
struct pci_device_id {};

struct ieee80211_hw {
    void *priv; 
    void *vif;
};

// Declaração de funções nativas do subsistema de rede do Linux
static inline u8* ieee80211_get_qos_ctl(struct ieee80211_hdr *hdr) {
    static u8 dummy_qos = 0;
    return &dummy_qos;
}

static inline struct ieee80211_sta* ieee80211_find_sta(void *vif, const u8 *bssid) {
    return NULL;
}

// === 6. EMULADOR DE VERSÃO DO KERNEL DO LINUX ===
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0) 

// === 7. TIPOS E MACROS ADICIONAIS ===
#define THIS_MODULE NULL
#define GFP_KERNEL  0
#define __printf(a, b) 

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

// === 8. TRADUÇÃO DE LOGS ===
#define pr_info(fmt, ...)  IOLog(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   IOLog(fmt, ##__VA_ARGS__)
#define printk(fmt, ...)   printf(fmt, ##__VA_ARGS__)

// === 9. O TRUQUE DO VZALLOC E VFREE ===
static inline void* apple_vzalloc(unsigned long size) {
    void* mem = IOMallocZero(size + sizeof(unsigned long));
    if (!mem) return NULL;
    *(unsigned long*)mem = size;
    return (void*)((char*)mem + sizeof(unsigned long));
}

static inline void apple_vfree(void* ptr) {
    if (!ptr) return;
    void* real_ptr = (void*)((char*)ptr - sizeof(unsigned long));
    unsigned long size = *(unsigned long*)real_ptr;
    IOFree(real_ptr, size + sizeof(unsigned long));
}

#define vzalloc(size) apple_vzalloc(size)
#define vfree(ptr)    apple_vfree(ptr)

// === 10. NEUTRALIZADOR DE MACROS ===
#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_FIRMWARE(x)
#define MODULE_DEVICE_TABLE(x, y)
#define module_param_named(name, value, type, perm)
#define MODULE_PARM_DESC(name, desc)
#define SIMPLE_DEV_PM_OPS(name, suspend, resume)

#endif // APPLE_LINUX_EMULATION_H
