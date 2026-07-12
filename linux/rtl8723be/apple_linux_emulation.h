#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/param.h> 

#ifdef __cplusplus
extern "C" {
#endif

// =========================================================================
// 1. BYPASS DE SEGURANÇA XNU & ALINHAMENTO
// =========================================================================
#undef memcpy
#define memcpy(dst, src, n) __builtin_memcpy(dst, src, n)
#undef memset
#define memset(dst, val, n) __builtin_memset(dst, val, n)
#undef memmove
#define memmove(dst, src, n) __builtin_memmove(dst, src, n)

#ifdef __packed
#undef __packed
#endif
#define __packed __attribute__((packed))

#ifdef __aligned
#undef __aligned
#endif
#define __aligned(x) __attribute__((aligned(x)))

#define __iomem
#define __init
#define __exit

// =========================================================================
// 2. TIPOS BÁSICOS DO KERNEL
// =========================================================================
typedef unsigned char        u8;
typedef unsigned short       u16;
typedef unsigned int         u32;
typedef unsigned long long   u64;

typedef signed char          s8;
typedef signed short         s16;
typedef signed int           s32;
typedef signed long long     s64;

typedef unsigned short       __le16;
typedef unsigned int         __le32;
typedef unsigned long long   __le64;
typedef unsigned short       __be16;
typedef unsigned int         __be32;

typedef long long            time64_t;
typedef unsigned long        dma_addr_t;
typedef unsigned long        kernel_ulong_t;

typedef struct { volatile int counter; } atomic_t;
typedef struct { int dummy; } spinlock_t;

// =========================================================================
// 3. OPERAÇÕES DE I/O (MMIO)
// =========================================================================
#define readb(addr)        (*(volatile u8 *)(addr))
#define readw(addr)        (*(volatile u16 *)(addr))
#define readl(addr)        (*(volatile u32 *)(addr))
#define writeb(val, addr)  (*(volatile u8 *)(addr) = (val))
#define writew(val, addr)  (*(volatile u16 *)(addr) = (val))
#define writel(val, addr)  (*(volatile u32 *)(addr) = (val))

// =========================================================================
// 4. CONFIGURAÇÕES DE TIMING, JIFFIES & COMPLETIONS
// =========================================================================
#ifndef HZ
  #define HZ 100
#endif
#ifndef hz
  #define hz HZ
#endif

#define MSEC_PER_SEC 1000
#define jiffies 0UL

#define mdelay(x)          IODelay((x) * 1000)
#define udelay(x)          IODelay(x)
static inline void usleep_range(unsigned long min, unsigned long max) { IODelay(min); }

#ifndef jiffies_to_msecs
  #define jiffies_to_msecs(x) ((unsigned int)((x) * 1000 / hz))
#endif
#ifndef msecs_to_jiffies
  #define msecs_to_jiffies(x) ((unsigned long)((x) * hz / 1000))
#endif

static inline int time_before(unsigned long a, unsigned long b) { return (long)(b - a) > 0; }
static inline int time_after(unsigned long a, unsigned long b) { return (long)(a - b) > 0; }

#define init_completion(x)                      ((void)0)
#define reinit_completion(x)                    ((void)0)
#define wait_for_completion_timeout(x, timeout) (timeout)
#define complete(x)                             ((void)(x))

// =========================================================================
// 5. LOCKS, ATOMICIDADE E DEBUG CONTRATUAL
// =========================================================================
#define spin_lock(lock)
#define spin_unlock(lock)
#define spin_lock_bh(lock)
#define spin_unlock_bh(lock)
#define rcu_read_lock()
#define rcu_read_unlock()
#define spin_lock_init(l)                       ((void)(l))

#define spin_lock_irqsave(lock, flags)          do { (void)(flags); } while(0)
#define spin_unlock_irqrestore(lock, flags)     do { (void)(flags); } while(0)

#define WARN_ONCE(cond, fmt, ...)               do { (void)(cond); } while(0)
#define WARN_ON(x)                              do { (void)(x); } while(0)

static inline int test_bit(int nr, const volatile unsigned long *addr) { return (*addr & (1UL << nr)) != 0; }
static inline int atomic_inc_return(atomic_t *v) { return ++(v->counter); }

// =========================================================================
// 6. PRINTK & DEBUGS
// =========================================================================
#define printk printf
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)

struct seq_file { int dummy; };
#define seq_puts(m, x)          ((void)0)
#define seq_printf(m, fmt, ...) ((void)0)

// =========================================================================
// 7. ENDEREÇAMENTO E REDE DE INFRAESTRUTURA (MAC & SK_BUFF)
// =========================================================================
#define ETH_ALEN 6
#define ETH_P_IP   0x0800
#define ETH_P_ARP  0x0806
#define ETH_P_IPV6 0x86DD
#define ETH_P_PAE  0x888E
#define IPPROTO_UDP 17

static inline void ether_addr_copy(void *dst, const void *src) { __builtin_memcpy(dst, src, ETH_ALEN); }
static inline int ether_addr_equal(const unsigned char *a, const unsigned char *b) { return __builtin_memcmp(a, b, 6) == 0; }
static inline int is_multicast_ether_addr(const unsigned char *addr) { return (addr[0] & 0x01); }
static inline int is_broadcast_ether_addr(const unsigned char *addr) {
    return (addr[0] == 0xff && addr[1] == 0xff && addr[2] == 0xff &&
            addr[3] == 0xff && addr[4] == 0xff && addr[5] == 0xff);
}
static inline bool is_valid_ether_addr(const unsigned char *addr) { (void)addr; return true; }

struct sk_buff { 
    void *data; 
    unsigned int len; 
    unsigned int priority; 
};
struct sk_buff_head { int dummy; };

static inline struct sk_buff *dev_alloc_skb(unsigned int length) { return NULL; }
static inline void skb_put_data(struct sk_buff *skb, const void *data, unsigned int len) { }
static inline int skb_queue_len(const struct sk_buff_head *list) { return 0; }
static inline struct sk_buff *__skb_dequeue(struct sk_buff_head *list) { return NULL; }
static inline void __skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk) { (void)list; (void)newsk; }
static inline void kfree_skb(struct sk_buff *skb) { }
static inline void skb_reserve(struct sk_buff *skb, int len) { }

static inline void *skb_put_zero(struct sk_buff *skb, unsigned int len) {
    if (!skb) return NULL;
    return skb->data; 
}

static inline void *skb_push(struct sk_buff *skb, unsigned int len) {
    if (skb) {
        skb->data = (void *)((char *)skb->data - len);
        skb->len += len;
    }
    return skb ? skb->data : NULL;
}

struct iphdr { u8 ihl:4, version:4; u8 protocol; };

// =========================================================================
// 8. INFRAESTRUTURA DE WORKQUEUES E TIMERS
// =========================================================================
struct timer_list { int dummy; };
struct delayed_work { int dummy; };

#define timer_setup(timer, callback, flags)     ((void)(timer), (void)(callback), (void)(flags))
#define del_timer_sync(t)                       ((void)(t))
#define INIT_DELAYED_WORK(w, f)                 ((void)(w), (void)(f))
#define cancel_delayed_work(w)                  ((void)(w))
static inline int mod_timer(struct timer_list *timer, unsigned long expires) { return 0; }

static inline void *alloc_workqueue(const char *fmt, unsigned int flags, int max_active, ...) { return (void *)1; }
static inline void queue_delayed_work(void *wq, struct delayed_work *dwork, unsigned long delay) {}

#define to_delayed_work(x)                      (x)
#define from_timer(var, callback_timer, timer_fieldname) \
    ((struct rtl_priv *)((char *)(callback_timer) - offsetof(struct rtl_priv, works.watchdog_timer)))

static inline int in_interrupt(void) { return 0; }

// =========================================================================
// 9. SUPORTE PCI E SUPORTE DO MÓDULO DO DRIVER
// =========================================================================
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
#define THIS_MODULE           NULL
#define KBUILD_MODNAME        "rtl8723be"

#define MODULE_DEVICE_TABLE(type, name)
#define MODULE_AUTHOR(name)
#define MODULE_LICENSE(name)
#define MODULE_DESCRIPTION(name)
#define MODULE_FIRMWARE(name)
#define module_param_named(name, value, type, perm)
#define MODULE_PARM_DESC(name, desc)
#define module_pci_driver(driver)
#define EXPORT_SYMBOL_GPL(x)
#define EXPORT_SYMBOL(x)

#define vzalloc(size)         kzalloc(size, 0)
#define vfree(ptr)            do { if(ptr) { /* stub */ } } while(0)

static inline dma_addr_t pci_map_single(void *pdev, void *ptr, size_t size, int direction) { return 0; }
static inline void pci_unmap_single(void *pdev, dma_addr_t dma_addr, size_t size, int direction) { }
static inline int pci_dma_mapping_error(void *pdev, dma_addr_t dma_addr) { return 0; }

struct dev_pm_ops { int dummy; };
#define SIMPLE_DEV_PM_OPS(name, suspend, resume) static const struct dev_pm_ops name = { 0 }

struct mac_dummy_driver { const struct dev_pm_ops *pm; };
struct pci_driver {
    const char *name;
    const struct pci_device_id *id_table;
    void *probe;
    void *remove;
    void *shutdown;
    struct mac_dummy_driver driver;
};

// =========================================================================
// 10. ENUMERAÇÕES, ESTRUTURAS E CONSTANTES DO SUBSISTEMA 802.11
// =========================================================================
#ifndef cpu_to_le32
  #define cpu_to_le32(x) ((unsigned int)(x))
#endif
#ifndef le32_to_cpu
  #define le32_to_cpu(x) ((unsigned int)(x))
#endif
#ifndef cpu_to_le16
  #define cpu_to_le16(x) ((unsigned short)(x))
#endif
#ifndef le16_to_cpu
  #define le16_to_cpu(x) ((unsigned short)(x))
#endif

static inline unsigned short be16_to_cpu(__be16 val) { return (unsigned short)(((val & 0xFF) << 8) | ((val >> 8) & 0xFF)); }
static inline unsigned int be32_to_cpu(__be32 val) {
    return ((val & 0xFF) << 24) | ((val & 0xFF00) << 8) | ((val >> 8) & 0xFF00) | ((val >> 24) & 0xFF);
}
static inline unsigned short be16_to_cpup(const __be16 *p) { return be16_to_cpu(*p); }
static inline u64 div64_u64(u64 dividend, u64 divisor) { return dividend / divisor; }

#define IEEE80211_FCTL_FTYPE       0x000c
#define IEEE80211_FTYPE_MGMT       0x0000
#define IEEE80211_FTYPE_CTL        0x0004
#define IEEE80211_SCTL_FRAG        0x000f
#define IEEE80211_SCTL_SEQ         0xfff0
#define IEEE80211_FCTL_TODS        0x0100
#define IEEE80211_FCTL_FROMDS      0x0200
#define IEEE80211_FCTL_MOREFRAGS   0x0400
#define IEEE80211_TX_CTL_AMPDU     0x00000002
#define IEEE80211_STYPE_ACTION     0x00d0

#define IEEE80211_HT_CAP_SUP_WIDTH_20_40                  (1 << 1)
#define IEEE80211_HT_CAP_SGI_20                           (1 << 5)
#define IEEE80211_HT_CAP_SGI_40                           (1 << 6)
#define IEEE80211_CHAN_NO_HT40MINUS                       (1 << 0)
#define IEEE80211_HT_CAP_DSSSCCK40                        (1 << 2)
#define IEEE80211_HT_CAP_MAX_AMSDU                        (1 << 3)
#define IEEE80211_HT_MAX_AMPDU_64K                        4
#define IEEE80211_HT_MPDU_DENSITY_16                      6
#define IEEE80211_HT_MCS_TX_DEFINED                       1
#define IEEE80211_MAX_AMPDU_BUF                           64

#define IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_11454           (1 << 0)
#define IEEE80211_VHT_CAP_SHORT_GI_80                     (1 << 1)
#define IEEE80211_VHT_CAP_TXSTBC                          (1 << 2)
#define IEEE80211_VHT_CAP_RXSTBC_1                        (1 << 3)
#define IEEE80211_VHT_CAP_SU_BEAMFORMER_CAPABLE           (1 << 4)
#define IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE           (1 << 5)
#define IEEE80211_VHT_CAP_HTC_VHT                         (1 << 6)
#define IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_MASK (1 << 7)
#define IEEE80211_VHT_CAP_RX_ANTENNA_PATTERN              (1 << 8)
#define IEEE80211_VHT_CAP_TX_ANTENNA_PATTERN              (1 << 9)

#define IEEE80211_VHT_MCS_SUPPORT_0_7                     0
#define IEEE80211_VHT_MCS_SUPPORT_0_8                     1
#define IEEE80211_VHT_MCS_SUPPORT_0_9                     2
#define IEEE80211_VHT_MCS_NOT_SUPPORTED                   3

#define IEEE80211_TX_RC_SHORT_GI                          (1 << 0)
#define IEEE80211_TX_RC_USE_CTS_PROTECT                  (1 << 1)
#define IEEE80211_TX_RC_USE_RTS_CTS                       (1 << 2)
#define IEEE80211_TX_RC_MCS                               (1 << 3)
#define IEEE80211_TX_RC_VHT_MCS                           (1 << 4)
#define IEEE80211_TX_RC_USE_SHORT_PREAMBLE               (1 << 0)

#define WIPHY_VENDOR_CMD_NEED_WDEV                        (1 << 0)
#define WIPHY_VENDOR_CMD_NEED_NETDEV                      (1 << 1)
#define WIPHY_FLAG_IBSS_RSN                               (1 << 2)
#define WIPHY_FLAG_HAS_REMAIN_ON_CHANNEL                  (1 << 3)

#define NL80211_BAND_2GHZ                                 0
#define NL80211_BAND_5GHZ                                 1
#define IEEE80211_ADDBA_PARAM_TID_MASK                    0x001e

#define WLAN_CIPHER_SUITE_WEP40    0x000fac01
#define WLAN_CIPHER_SUITE_TKIP     0x000fac02
#define WLAN_CIPHER_SUITE_CCMP     0x000fac04
#define WLAN_CIPHER_SUITE_WEP104   0x000fac05

#define RX_FLAG_FAILED_FCS_CRC     0x0001
#define RX_FLAG_MACTIME_START      0x0002
#define RX_FLAG_DECRYPTED          0x0004
#define RATE_INFO_BW_40            2
#define RX_ENC_HT                  1

#define WLAN_CATEGORY_HT              7
#define WLAN_HT_ACTION_SMPS           2
#define WLAN_HT_SMPS_CONTROL_STATIC   0
#define WLAN_HT_SMPS_CONTROL_DYNAMIC  1
#define WLAN_HT_SMPS_CONTROL_DISABLED 0

enum nl80211_iftype {
    NL80211_IFTYPE_UNSPECIFIED, NL80211_IFTYPE_ADHOC, NL80211_IFTYPE_STATION, NL80211_IFTYPE_AP,
    NL80211_IFTYPE_AP_VLAN, NL80211_IFTYPE_WDS, NL80211_IFTYPE_MONITOR, NL80211_IFTYPE_MESH_POINT,
    NL80211_IFTYPE_P2P_CLIENT, NL80211_IFTYPE_P2P_GO, NL80211_IFTYPE_P2P_DEVICE, NL80211_IFTYPE_OCB,
    NL80211_IFTYPE_NAN
};

enum nl80211_channel_type {
    NL80211_CHAN_NO_HT, NL80211_CHAN_HT20, NL80211_CHAN_HT40MINUS, NL80211_CHAN_HT40PLUS
};

enum ieee80211_smps_mode {
    IEEE80211_SMPS_STATIC = 0, IEEE80211_SMPS_DYNAMIC, IEEE80211_SMPS_AUTOMATIC,
    IEEE80211_SMPS_OFF, IEEE80211_SMPS_NUM_MODES
};

enum ieee80211_hw_set_type {
    SIGNAL_DBM = 0, RX_INCLUDES_FCS, AMPDU_AGGREGATION, CONNECTION_MONITOR,
    MFP_CAPABLE, REPORTS_TX_ACK_STATUS, SUPPORTS_TX_FRAG, SUPPORT_FAST_XMIT,
    SUPPORTS_AMSDU_IN_AMPDU, SUPPORTS_PS, PS_NULLFUNC_STACK, SUPPORTS_DYNAMIC_PS
};

// Declarações Fantasmas para evitar conflitos de escopo opacos
struct urb; 
struct firmware;
struct regulatory_request;
struct ieee80211_hw;
struct ieee80211_sta;

struct ieee80211_channel { int center_freq; int band; int hw_value; unsigned int flags; int max_power; };
struct ieee80211_rate { unsigned int bitrate; unsigned int flags; int hw_value; };
struct ieee80211_ht_cap { int dummy; };
struct ieee80211_vht_cap { int dummy; };

struct ieee80211_supported_band {
    int band; struct ieee80211_channel *channels; int n_channels;
    struct ieee80211_rate *bitrates; int n_bitrates;
    struct ieee80211_ht_cap ht_cap; struct ieee80211_vht_cap vht_cap;  
};

struct wireless_dev { int dummy; };
struct wiphy;

struct wiphy_vendor_command {
    struct {
        u32 vendor_id;
        u32 subcmd;
    } info;
    int (*doit)(struct wiphy *wiphy, struct wireless_dev *wdev, const void *data, int len);
    unsigned int flags; 
};

struct wiphy {
    const struct wiphy_vendor_command *vendor_commands;
    int n_vendor_commands;
    struct ieee80211_supported_band *bands[2];
    unsigned int interface_modes; unsigned int flags; int rts_threshold;
};

struct ieee80211_tx_rate { u32 flags; u8 idx; };
struct ieee80211_tx_info { u32 flags; int band; };
struct ieee80211_vif { struct { int use_short_slot; } bss_conf; };

struct ieee80211_mgmt {
    u16 frame_control; u8 da[ETH_ALEN]; u8 sa[ETH_ALEN]; u8 bssid[ETH_ALEN];
    union {
        struct { u8 variable[1]; } beacon;
        struct {
            u8 category;
            union {
                struct { u8 action; u8 smps_control; } ht_smps;
                struct { u16 capab; } addba_req;
            } u;
        } action;
    } u;
};

// =========================================================================
// 11. ENGENHARIA DE MOCK INLINE (FUNÇÕES COMPATÍVEIS PARA BASE.C)
// =========================================================================
static inline int ieee80211_is_beacon(unsigned short fc) { return (fc & 0x00fc) == 0x0080; }
static inline int ieee80211_is_mgmt(unsigned short fc) { return (fc & 0x000c) == 0x0000; }
static inline int ieee80211_is_ctl(unsigned short fc) { return (fc & 0x000c) == 0x0004; }
static inline int ieee80211_is_nullfunc(unsigned short fc) { return (fc & 0x00fc) == 0x0048 || (fc & 0x00fc) == 0x00c8; }
static inline int ieee80211_is_data_qos(unsigned short fc) { return (fc & 0x000c) == 0x0008 && (fc & 0x0080); }
static inline int ieee80211_is_data(unsigned short fc) { return ((fc & 0x000c) == 0x0008); }
static inline int ieee80211_is_auth(unsigned short fc) { return ((fc & 0x00fc) == 0x00b0); }
static inline int ieee80211_is_probe_req(unsigned short fc) { return ((fc & 0x00fc) == 0x0040); }
static inline int ieee80211_is_probe_resp(unsigned short fc) { return ((fc & 0x00fc) == 0x0050); }
static inline int ieee80211_is_action(unsigned short fc) { return ((fc & 0x00fc) == 0x00d0); }

static inline u8 ieee80211_get_hdrlen_from_skb(struct sk_buff *skb) { return 24; }
static inline void ieee80211_rx_irqsafe(void *hw, struct sk_buff *skb) { (void)hw; (void)skb; }
static inline void ieee80211_start_tx_ba_cb_irqsafe(void *vif, const u8 *addr, u8 tid) { (void)vif; (void)addr; (void)tid; }
static inline void ieee80211_stop_tx_ba_cb_irqsafe(void *vif, const u8 *addr, u8 tid) { (void)vif; (void)addr; (void)tid; }
static inline void ieee80211_connection_loss(void *hw) { (void)hw; }

static inline unsigned char *ieee80211_get_SA(void *hdr) { return ((unsigned char *)hdr) + 10; }
static inline unsigned char *ieee80211_get_DA(void *hdr) { return ((unsigned char *)hdr) + 4; }

static inline int _ieee80211_is_robust_mgmt_frame(const void *hdr) { return 0; }
static inline int ieee80211_has_protected(unsigned short fc) { return (fc & 0x4000) ? 1 : 0; }

static inline u8 *ieee80211_get_qos_ctl(const void *hdr) { static u8 dummy[2] = {0}; return dummy; }
static inline struct ieee80211_sta *ieee80211_find_sta(void *vif, const u8 *addr) { return NULL; }

static inline void ieee80211_hw_set(struct ieee80211_hw *hw, enum ieee80211_hw_set_type type) { (void)hw; (void)type; }
#define SET_IEEE80211_PERM_ADDR(hw, addr)               ((void)(hw), (void)(addr))
static inline void get_random_bytes(void *buf, int nbytes) { (void)buf; (void)nbytes; }
#define wiphy_rfkill_set_hw_state(w, s)                  ((void)(w), (void)(s))
#define wiphy_rfkill_start_polling(w)                    ((void)(w))
#define wiphy_rfkill_stop_polling(w)                     ((void)(w))

#define wiphy_to_ieee80211_hw(w)                         ((struct ieee80211_hw *)(w))

static inline int ieee80211_rate_get_vht_nss(const struct ieee80211_tx_rate *r) { return 1; }
static inline int ieee80211_rate_get_vht_mcs(const struct ieee80211_tx_rate *r) { return 0; }
static inline struct ieee80211_rate *ieee80211_get_tx_rate(void *hw, void *info) {
    static struct ieee80211_rate dummy_rate = {0}; return &dummy_rate;
}

static inline struct ieee80211_tx_info *IEEE80211_SKB_CB(struct sk_buff *skb) {
    static struct ieee80211_tx_info mock_info;
    return &mock_info;
}

#ifdef __cplusplus
}
#endif

#endif // APPLE_LINUX_EMULATION_H
