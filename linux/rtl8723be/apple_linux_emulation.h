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

// === 2. FERRAMENTAS DO COMPILADOR E REDE DO LINUX ===
#define __packed __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))
#define BIT(x) (1ULL << (x))
#define ETH_ALEN 6
#define NUM_NL80211_BANDS 3
#define MAX_TID_COUNT 8
#define RTL_MAC80211_NUM_QUEUE 5

// === 3. ESTRUTURAS DE CONTROLE E CONCORRÊNCIA ===
struct list_head {
    struct list_head *next, *prev;
};

struct mutex {
    void* owner;
};

// Emulação do Spinlock usando as travas nativas do Kernel da Apple
typedef struct {
    IOSimpleLock* lock;
} spinlock_t;

// Emulação de variáveis atômicas
typedef struct {
    volatile int counter;
} atomic_t;

// === 4. DECLARAÇÕES FANTASMAS (Engana as checagens de rede do Linux) ===
struct ieee80211_supported_band {};
enum nl80211_iftype { NL80211_IFTYPE_UNSPECIFIED };
enum nl80211_channel_type { NL80211_CHAN_NO_HT };
struct sk_buff_head {};
struct ieee80211_tx_queue_params {};
struct ieee80211_sta {};
struct sk_buff {};
struct ieee80211_hdr {};
struct ieee80211_tx_info {};
struct ieee80211_rx_status {};
struct rtl_stats {};
struct ieee80211_hw {};
struct urb {};

// === 5. EMULADOR DE VERSÃO DO KERNEL DO LINUX ===
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0) 

// === 6. TIPOS E MACROS ADICIONAIS ===
#define THIS_MODULE NULL
#define GFP_KERNEL  0
#define __printf(a, b) 

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

// === 7. TRADUÇÃO DE LOGS (PRINTK / PR_INFO) ===
#define pr_info(fmt, ...)  IOLog(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   IOLog(fmt, ##__VA_ARGS__)

// === 8. O TRUQUE DO VZALLOC E VFREE ===
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

// === 9. NEUTRALIZADOR DE MACROS DO LINUX ===
#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_FIRMWARE(x)
#define MODULE_DEVICE_TABLE(x, y)
#define module_param_named(name, value, type, perm)
#define MODULE_PARM_DESC(name, desc)
#define SIMPLE_DEV_PM_OPS(name, suspend, resume)

#endif // APPLE_LINUX_EMULATION_H
