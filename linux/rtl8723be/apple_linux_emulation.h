#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>
#include <sys/errno.h>

// Proteção essencial: só aplica o extern "C" se o arquivo atual for C++ (.cpp ou .hpp)
#ifdef __cplusplus
extern "C" {
#endif

// --- BYPASS DE SEGURANÇA XNU (BOUNDS SAFETY / FORTIFY SOURCE) ---
// Ignora os wrappers estritos de tamanho do macOS para aceitar o código legado do Linux
#undef memcpy
#define memcpy(dst, src, n) __builtin_memcpy(dst, src, n)
#undef memset
#define memset(dst, val, n) __builtin_memset(dst, val, n)
#undef memmove
#define memmove(dst, src, n) __builtin_memmove(dst, src, n)

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

// --- 2. KERNEL VERSION, TIMERS, OPTIMIZATIONS & UTILS ---
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0)
#endif

// Macros de Otimização do Compilador (Likely/Unlikely)
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

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
    unsigned int priority; 
};

struct sk_buff_head { int dummy; };
struct timer_list { int dummy; };
struct tasklet_struct { int dummy; };
struct delayed_work { int dummy; };
struct work_struct { int dummy; };
struct completion { int dummy; };
struct ieee80211_tx_queue_params { int dummy; };

struct ieee80211_hdr { 
    unsigned short frame_control;
    unsigned short duration_id; 
    unsigned char addr1[6];     
    unsigned char addr2[6];     
    unsigned char addr3[6];     
    unsigned short seq_ctrl;    
};

// Estruturas de canais e frequências para o trx.c
struct ieee80211_chan {
    unsigned int center_freq;
    unsigned int band;
};

struct ieee80211_chandef {
    struct ieee80211_chan *chan;
};

struct ieee80211_hw_conf {
    struct ieee80211_chandef chandef;
};

struct ieee80211_hw { 
    void *priv; 
    void *vif; 
    struct ieee80211_hw_conf conf;
};

// Estrutura de status de recepção de pacotes
struct ieee80211_rx_status {
    unsigned int freq;
    unsigned int band;
    unsigned int flag;
    unsigned char bw;
    unsigned char encoding;
    u8 rate_idx;
    u64 mactime;
    s8 signal;
};

// Definições internas de Station corrigidas com suporte a sub-estruturas MCS
struct ieee80211_mcs_cap {
    u8 rx_mask[16];
};

struct ieee80211_ht_cap {
    u32 cap;
    struct ieee80211_mcs_cap mcs; 
    u8 ampdu_density;
};

struct ieee80211_sta {
    struct ieee80211_ht_cap ht_cap;
    void *drv_priv;
    u16 aid;
    u32 supp_rates[16];
};

// Estruturas de chaves de segurança e criptografia de hardware
struct ieee80211_key_conf {
    u32 cipher;
};

struct ieee80211_tx_control {
    struct ieee80211_key_conf *hw_key;
};

struct ieee80211_tx_info {
    u32 flags;
    struct ieee80211_tx_control control;
};

// --- 7. STUBS DE FUNÇÕES, ALOCADORES E MEMÓRIA ---
#define GFP_KERNEL 0
static inline void *kzalloc(size_t size, int flags) { return IOMallocZero(size); }
#define kfree(ptr) do { if (ptr) { IOFree(ptr, sizeof(*ptr)); } } while(0)

#define spin_lock(lock)
#define spin_unlock(lock)
#define spin_lock_bh(lock)
#define spin_unlock_bh(lock)
#define rcu_read_lock()
#define rcu_read_unlock()

#define spin_lock_irqsave(lock, flags) do { (void)(flags); } while(0)
#define spin_unlock_irqrestore(lock, flags) do { (void)(flags); } while(0)

#define WARN_ONCE(cond, fmt, ...) do { (void)(cond); } while(0)

// Operações de endereços MAC da rede Linux (Tipagem corrigida para void*)
static inline void ether_addr_copy(void *dst, const void *src) { __builtin_memcpy(dst, src, ETH_ALEN); }

static inline int ether_addr_equal(const unsigned char *a, const unsigned char *b) {
    return __builtin_memcmp(a, b, 6) == 0;
}
static inline int is_multicast_ether_addr(const unsigned char *addr) {
    return (addr[0] & 0x01);
}
static inline int is_broadcast_ether_addr(const unsigned char *addr) {
    return (addr[0] == 0xff && addr[1] == 0xff && addr[2] == 0xff &&
            addr[3] == 0xff && addr[4] == 0xff && addr[5] == 0xff);
}

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

// Implementação básica do skb_push para gerenciar ponteiros do buffer de dados
static inline void *skb_push(struct sk_buff *skb, unsigned int len) {
    if (skb) {
        skb->data = (void *)((char *)skb->data - len);
        skb->len += len;
    }
    return skb ? skb->data : NULL;
}

static inline int mod_timer(struct timer_list *timer, unsigned long expires) { return 0; }
static inline unsigned long msecs_to_jiffies(const unsigned int m) { return m; }

// Mapeamentos de barramento PCI de rede do Linux
static inline dma_addr_t pci_map_single(void *pdev, void *ptr, size_t size, int direction) { return 0; }
static inline void pci_unmap_single(void *pdev, dma_addr_t dma_addr, size_t size, int direction) { }
static inline int pci_dma_mapping_error(void *pdev, dma_addr_t dma_addr) { return 0; }

// --- GAMBIARRA DE COMPATIBILIDADE PARA TEMPO (JIFFIES) ---
#include <sys/param.h> 

#ifndef hz
  #define hz 100 
#endif

#ifndef jiffies_to_msecs
  #define jiffies_to_msecs(x) ((unsigned int)((x) * 1000 / hz))
#endif

#ifndef msecs_to_jiffies
  #define msecs_to_jiffies(x) ((unsigned long)((x) * hz / 1000))
#endif

// =======================================================
// --- EMULAÇÕES PARA CORRIGIR O SW.C (LINUX -> MAC) ---
// =======================================================

typedef unsigned long kernel_ulong_t;

struct pci_device_id {
    unsigned int vendor, device;
    unsigned int subvendor, subdevice;
    #ifdef __cplusplus
    unsigned int pci_class; 
    #else
    unsigned int class;
    #endif
    unsigned int class_mask;
    kernel_ulong_t driver_data;
};

#define PCI_VENDOR_ID_REALTEK 0x10ec
#define PCI_ANY_ID            (~0U)

#define THIS_MODULE          NULL
#define MODULE_DEVICE_TABLE(type, name)
#define MODULE_AUTHOR(name)
#define MODULE_LICENSE(name)
#define MODULE_DESCRIPTION(name)
#define MODULE_FIRMWARE(name)
#define module_param_named(name, value, type, perm)
#define MODULE_PARM_DESC(name, desc)

#define vzalloc(size)        kzalloc(size, 0)
#define vfree(ptr)           do { if(ptr) { /* stub */ } } while(0)
#define le16_to_cpu(x)       (x)

#define request_firmware_nowait(...) (0)

// =======================================================

#define KBUILD_MODNAME "rtl8723be"

struct dev_pm_ops { int dummy; };
#define SIMPLE_DEV_PM_OPS(name, suspend, resume) static const struct dev_pm_ops name = { 0 }

struct mac_dummy_driver {
    const struct dev_pm_ops *pm;
};

struct pci_driver {
    const char *name;
    const struct pci_device_id *id_table;
    void *probe;
    void *remove;
    void *shutdown;
    struct mac_dummy_driver driver;
};

#define module_pci_driver(driver)

// =======================================================
// --- EMULAÇÕES PARA O TRX.C (SUPORTE WIRELESS 802.11) ---
// =======================================================

#ifndef cpu_to_le32
#define cpu_to_le32(x) ((unsigned int)(x))
#endif
#ifndef le32_to_cpu
#define le32_to_cpu(x) ((unsigned int)(x))
#endif
#ifndef cpu_to_le16
#define cpu_to_le16(x) ((unsigned short)(x))
#endif

#define IEEE80211_FCTL_FTYPE       0x000c
#define IEEE80211_FTYPE_CTL        0x0004
#define IEEE80211_FCTL_TODS        0x0100
#define IEEE80211_FCTL_FROMDS      0x0200
#define IEEE80211_FCTL_MOREFRAGS   0x0400

#define IEEE80211_SCTL_FRAG        0x000f
#define IEEE80211_SCTL_SEQ         0xfff0

#define IEEE80211_TX_CTL_AMPDU     0x00000002

// Conjunto completo de chaves criptográficas (Cipher Suites) 802.11
#define WLAN_CIPHER_SUITE_WEP40    0x000fac01
#define WLAN_CIPHER_SUITE_TKIP     0x000fac02
#define WLAN_CIPHER_SUITE_CCMP     0x000fac04
#define WLAN_CIPHER_SUITE_WEP104   0x000fac05

static inline int ieee80211_is_beacon(unsigned short fc) {
    return (fc & 0x00fc) == 0x0080;
}
static inline int ieee80211_is_mgmt(unsigned short fc) {
    return (fc & 0x000c) == 0x0000;
}
static inline int ieee80211_is_ctl(unsigned short fc) {
    return (fc & 0x000c) == 0x0004;
}
static inline int ieee80211_is_nullfunc(unsigned short fc) {
    return (fc & 0x00fc) == 0x0048 || (fc & 0x00fc) == 0x00c8;
}
static inline int ieee80211_is_data_qos(unsigned short fc) {
    return (fc & 0x000c) == 0x0008 && (fc & 0x0080);
}

// Extração de endereços de Origem (SA) e Destino (DA) nos frames de Wi-Fi
static inline unsigned char *ieee80211_get_SA(void *hdr) {
    return ((unsigned char *)hdr) + 10;
}
static inline unsigned char *ieee80211_get_DA(void *hdr) {
    return ((unsigned char *)hdr) + 4;
}

// Flags e definições de Criptografia adicionadas para o trx.c
#define RX_FLAG_FAILED_FCS_CRC  0x0001
#define RX_FLAG_MACTIME_START   0x0002
#define RX_FLAG_DECRYPTED       0x0004
#define RATE_INFO_BW_40         2
#define RX_ENC_HT               1

static inline int _ieee80211_is_robust_mgmt_frame(const void *hdr) { 
    return 0; 
}
static inline int ieee80211_has_protected(unsigned short fc) { 
    return (fc & 0x4000) ? 1 : 0; 
}

// Fecha o bloco extern "C" de forma segura se for C++
#ifdef __cplusplus
}
#endif

// Anular macros de exportação de símbolos do Linux
#define EXPORT_SYMBOL_GPL(x)
#define EXPORT_SYMBOL(x)

// Mantemos apenas a função, já que as structs estão definidas no topo do arquivo
static inline void __skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk) {
    (void)list;
    (void)newsk;
}

// Emulação do sistema seq_file do Linux (usado para logs/debug)
struct seq_file {
    int dummy;
};

#define seq_puts(m, x)          ((void)0)
#define seq_printf(m, fmt, ...) ((void)0)

// Emulação de contexto de interrupção e sincronização (completions) do Linux
static inline int in_interrupt(void) {
    return 0;
}

#define init_completion(x)           ((void)0)
#define reinit_completion(x)         ((void)0)
#define wait_for_completion_timeout(x, timeout) (timeout)

// Emulação para macros de inicialização de módulo do Linux
#define __init
#define __exit
#define module_init(x)
#define module_exit(x)
#define complete(x) (void)(x)

// ============================================================================
// EMULAÇÃO DO SUBSISTEMA WIRELESS COMPLETO (PARTE 6 - FINAL)
// ============================================================================

#define NL80211_BAND_2GHZ 0
#define NL80211_BAND_5GHZ 1

#define IEEE80211_CHAN_NO_HT40MINUS      (1 << 0)
#define IEEE80211_HT_CAP_DSSSCCK40       (1 << 2)
#define IEEE80211_HT_CAP_MAX_AMSDU       (1 << 3)
#define IEEE80211_HT_MAX_AMPDU_64K       4
#define IEEE80211_HT_MPDU_DENSITY_16     6
#define IEEE80211_HT_MCS_TX_DEFINED      1

// Macros de Wi-Fi AC (VHT) exigidas pelo base.c
#define IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_11454          (1 << 0)
#define IEEE80211_VHT_CAP_SHORT_GI_80                    (1 << 1)
#define IEEE80211_VHT_CAP_TXSTBC                         (1 << 2)
#define IEEE80211_VHT_CAP_RXSTBC_1                       (1 << 3)
#define IEEE80211_VHT_CAP_SU_BEAMFORMER_CAPABLE          (1 << 4)
#define IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE          (1 << 5)
#define IEEE80211_VHT_CAP_HTC_VHT                        (1 << 6)
#define IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_MASK (1 << 7)
#define IEEE80211_VHT_CAP_RX_ANTENNA_PATTERN             (1 << 8)
#define IEEE80211_VHT_CAP_TX_ANTENNA_PATTERN             (1 << 9)
#define IEEE80211_VHT_MCS_SUPPORT_0_9                    0
#define IEEE80211_VHT_MCS_NOT_SUPPORTED                  3

// Flags de comandos de fabricante (Vendor Commands)
#define WIPHY_VENDOR_CMD_NEED_WDEV    (1 << 0)
#define WIPHY_VENDOR_CMD_NEED_NETDEV  (1 << 1)

// Tipos Big Endian e funções de conversão inline
typedef unsigned short __be16;
typedef unsigned int   __be32;

static inline unsigned short be16_to_cpu(__be16 val) {
    return ((val & 0xFF) << 8) | ((val >> 8) & 0xFF);
}
static inline unsigned int be32_to_cpu(__be32 val) {
    return ((val & 0xFF) << 24) | ((val & 0xFF00) << 8) | 
           ((val >> 8) & 0xFF00) | ((val >> 24) & 0xFF);
}

// Estruturas de canais e taxas
struct ieee80211_channel {
    int center_freq;
    int band;
    int hw_value;
    unsigned int flags;
    int max_power;
};

struct ieee80211_rate {
    unsigned int bitrate;
    unsigned int flags;
    int hw_value;
};

// Remendo para suportar rx_mask e rx_highest da estrutura MCS do HT
struct _patch_ieee80211_mcs_cap {
    unsigned int tx_params;
    unsigned short rx_highest;
    unsigned char rx_mask[10]; 
};
#define ieee80211_mcs_cap _patch_ieee80211_mcs_cap

struct ieee80211_sta_ht_cap {
    bool ht_supported;
    unsigned int cap;
    unsigned int ampdu_factor;
    unsigned int ampdu_density;
    struct ieee80211_mcs_cap mcs;
};

// Sub-estrutura para as taxas de transmissão do Wi-Fi AC (VHT)
struct ieee80211_vht_mcs_cap {
    unsigned short rx_mcs_map;
    unsigned short rx_highest;
    unsigned short tx_mcs_map;
    unsigned short tx_highest;
};

struct ieee80211_sta_vht_cap {
    bool vht_supported;
    unsigned int cap;
    struct ieee80211_vht_mcs_cap vht_mcs;
};

struct ieee80211_supported_band {
    int band;
    struct ieee80211_channel *channels;
    int n_channels;
    struct ieee80211_rate *bitrates;
    int n_bitrates;
    struct ieee80211_sta_ht_cap ht_cap;   // Corrigido para struct real
    struct ieee80211_sta_vht_cap vht_cap; // Adicionado campo vht_cap
};

// Estruturas do ecossistema Wiphy / Hardware do Linux
struct wireless_dev {
    int dummy;
};

struct wiphy_vendor_command {
    unsigned int info_idx;
    unsigned int flags;
    const void *doit;
};

struct wiphy {
    const struct wiphy_vendor_command *vendor_commands;
    int n_vendor_commands;
    struct ieee80211_supported_band *bands[2];
};

struct ieee80211_hw {
    struct wiphy *wiphy;
    struct wiphy private_wiphy; // Espaço reservado para o cálculo do offsetof
};

// Macro de conversão de wiphy para hw
#define wiphy_to_ieee80211_hw(w) ((struct ieee80211_hw *)((char *)(w) - offsetof(struct ieee80211_hw, private_wiphy)))

#endif // APPLE_LINUX_EMULATION_H
