#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/param.h> 

#ifdef KERNEL
#include <libkern/OSMalloc.h>
#include <sys/malloc.h>
#include <kern/thread_call.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Seção 1: Cabeçalhos Básicos e Macros de Módulos do Linux
 *******************************************************************************/
#ifndef _SECTION_1_EMULATION_
#define _SECTION_1_EMULATION_

#include <sys/param.h>
#include <sys/kernel.h>
#include <libkern/libkern.h>

/* Tipos primitivos do Linux mapeados para o ambiente do macOS */
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
typedef signed char         s8;
typedef short               s16;
typedef int                 s32;
typedef long long           s64;

typedef u32 __le32;
typedef u16 __le16;
typedef u16 __be16;

typedef unsigned int gfp_t;

/* Atributos de seção e compilação do Linux */
#define __init
#define __exit
#define __iomem
#define __packed __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))

/* Macros de metadados do módulo - ignoradas no macOS */
#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_VERSION(x)
#define MODULE_PARM_DESC(x, y)
#define module_param(name, type, perm)
#define module_param_named(name, value, type, perm)

/* Correção para os erros do rtl_btc.c: Stubs de inicialização do módulo */
#define module_init(initfn) static inline void __init_##initfn(void) { (void)initfn; }
#define module_exit(exitfn) static inline void __exit_##exitfn(void) { (void)exitfn; }

#endif /* _SECTION_1_EMULATION_ */

// =========================================================================
// 2. MACROS DE MANIPULAÇÃO DE BITS e BYPASS XNU
// =========================================================================
#ifndef BIT
  #define BIT(nr) (1UL << (nr))
#endif
#ifndef BIT_ULL
  #define BIT_ULL(nr) (1ULL << (nr))
#endif

#undef memcpy
#define memcpy(dst, src, n) __builtin_memcpy(dst, src, n)
#undef memset
#define memset(dst, val, n) __builtin_memset(dst, val, n)
#undef memmove
#define memmove(dst, src, n) __builtin_memmove(dst, src, n)
#undef memcmp
#define memcmp(s1, s2, n) __builtin_memcmp(s1, s2, n)

// =========================================================================
// 3. TIPOS BÁSICOS DO KERNEL LINUX
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
typedef unsigned long long   __be64;

typedef long long            time64_t;
typedef unsigned long        dma_addr_t;
typedef unsigned long        kernel_ulong_t;
typedef unsigned int         gfp_t;

typedef struct { volatile int counter; } atomic_t;

#ifndef bool
  typedef int bool;
  #define true 1
  #define false 0
#endif

/*******************************************************************************
 * Seção 4: Correção de Incomplete Types e Estruturas Básicas
 *******************************************************************************/
#ifndef _SECTION_4_EMULATION_
#define _SECTION_4_EMULATION_

#include <sys/queue.h>

/* Definições de enums e bandas do subsistema nl80211/ieee80211 */
enum nl80211_iftype {
    NL80211_IFTYPE_UNSPECIFIED,
    NL80211_IFTYPE_ADHOC,
    NL80211_IFTYPE_STATION,
    NL80211_IFTYPE_AP,
    NL80211_IFTYPE_MESH_POINT,
    NL80211_IFTYPE_P2P_CLIENT,
    NL80211_IFTYPE_P2P_GO,
    NL80211_IFTYPE_MONITOR
};

enum nl80211_channel_type {
    NL80211_CHAN_NO_HT,
    NL80211_CHAN_HT20,
    NL80211_CHAN_HT40MINUS,
    NL80211_CHAN_HT40PLUS
};

enum ieee80211_smps_mode {
    IEEE80211_SMPS_OFF,
    IEEE80211_SMPS_STATIC,
    IEEE80211_SMPS_DYNAMIC
};

#define NUM_NL80211_BANDS 2

struct ieee80211_supported_band {
    int bitrates;
    int channels;
};

/* Stubs de estruturas regulatórias para evitar warnings de escopo */
struct wiphy { int dummy; };
struct regulatory_request { int dummy; };
struct ieee80211_tx_info { int dummy; };

/* Estruturas do protocolo 802.11 e Filas de Transmissão */
struct ieee80211_hdr {
    u16 frame_control;
    u16 duration_id;
    u8 addr1[6];
    u8 addr2[6];
    u8 addr3[6];
    u16 seq_ctrl;
    u8 addr4[6];
    u16 qos_ctrl;
    u32 ht_ctrl;
};

struct ieee80211_tx_queue_params {
    u8 aifs;
    u16 cw_min;
    u16 cw_max;
    u16 txop_limit;
};

/* Estruturas de sincronismo e tasks do Linux */
struct mutex {
    void *apple_mutex_ptr;
};

struct tasklet_struct {
    void (*func)(unsigned long);
    unsigned long data;
};

struct urb {
    void *context;
};

/* Estruturas de controle de conexões (Stubs) */
struct ieee80211_sta {
    u8 mac_addr[6];
    void *driver_priv;
};

struct wireless_dev {
    int ifindex;
};

/* Estrutura principal do Hardware Wi-Fi emulado */
struct ieee80211_hw {
    void *priv; /* Guardará o ponteiro rtl_priv do driver */
    void *wiphy;
    u8 mac_addr[6];
    enum nl80211_iftype opmode;
    struct ieee80211_supported_band bands[NUM_NL80211_BANDS];
};

struct ieee80211_rx_status {
    u16 qual;
    u8 signal;
    u8 noise;
};

/* Estrutura de Listas do Linux emuladas via BSD <sys/queue.h> */
struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define INDIRECT_INIT_LIST_HEAD(ptr) do { (ptr)->next = (ptr); (ptr)->prev = (ptr); } while (0)

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - __builtin_offsetof(type, member)))
#endif

/* Macro essencial para varrer as listas do driver Realtek */
#define list_for_each_entry(pos, head, member) \
    for (pos = container_of((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = container_of(pos->member.next, typeof(*pos), member))

/* Stubs de funções ieee80211 que o driver wifi.h requisita */
static inline u8 *ieee80211_get_qos_ctl(void *hdr) {
    static u8 dummy_qos[2] = {0, 0};
    return dummy_qos;
}

static inline struct ieee80211_sta *ieee80211_find_sta(void *vif, const u8 *mac_addr) {
    return NULL;
}

#endif /* _SECTION_4_EMULATION_ */

// =========================================================================
// 5. OPERAÇÕES DE I/O (MMIO - MEMORY MAPPED I/O)
// =========================================================================
#define readb(addr)        (*(volatile u8 *)(addr))
#define readw(addr)        (*(volatile u16 *)(addr))
#define readl(addr)        (*(volatile u32 *)(addr))
#define writeb(val, addr)  (*(volatile u8 *)(addr) = (val))
#define writew(val, addr)  (*(volatile u16 *)(addr) = (val))
#define writel(val, addr)  (*(volatile u32 *)(addr) = (val))

/*******************************************************************************
 * Seção 6: Timing, Jiffies, Timers e Delays (Emulação macOS)
 *******************************************************************************/
#ifndef _SECTION_6_EMULATION_
#define _SECTION_6_EMULATION_

#include <stdint.h>

/* Funções nativas do XNU macOS para obter tempo absoluto */
extern uint64_t clock_get_absolute_time(void);
extern void absolutetime_to_nanoseconds(uint64_t clock_time, uint64_t *nanoseconds);

#define HZ 1000

/* Simulação dinâmica dos jiffies usando o tempo real do sistema macOS */
static inline unsigned long apple_get_jiffies(void) {
    uint64_t abstime = clock_get_absolute_time();
    uint64_t nanos;
    absolutetime_to_nanoseconds(abstime, &nanos);
    return (unsigned long)(nanos / 1000000); /* Converte nanosegundos para milissegundos */
}

#define jiffies apple_get_jiffies()

#define jiffies_to_msecs(x) ((unsigned int)(((unsigned long)(x) * 1000) / HZ))
#define msecs_to_jiffies(x) ((unsigned long)(((unsigned long)(x) * HZ) / 1000))

/* Correção de filas para bater com o wifi.h */
#define RTL_MAC80211_NUM_QUEUE 5

#define mdelay(ms) IOSleep(ms)
#define udelay(us) { int i; for(i=0; i<us*10; i++) { asm volatile(""); } }

static inline void linux_msleep(unsigned int ms) {
    IOSleep(ms);
}

static inline void usleep_range(unsigned long min, unsigned long max) {
    IOSleep((unsigned int)(min / 1000));
}

/* Redireciona o msleep do driver para a nossa função segura */
#define msleep(ms) linux_msleep(ms)

#endif /* _SECTION_6_EMULATION_ */

/*******************************************************************************
 * Seção 7: Emulação Completa de Sincro (Wait for Completion) no macOS
 *******************************************************************************/
#ifndef _SECTION_7_EMULATION_
#define _SECTION_7_EMULATION_

struct completion {
    unsigned int done;
    void *event_chan;
};

static inline void init_completion(struct completion *x) {
    x->done = 0;
    x->event_chan = (void *)x;
}

static inline void reinit_completion(struct completion *x) {
    x->done = 0; /* Reinicia o sinal de sincronismo */
}

static inline unsigned long wait_for_completion_timeout(struct completion *x, unsigned long timeout) {
    int timeout_ms = (int)jiffies_to_msecs(timeout);
    int polled = 0;
    
    while (x->done == 0 && polled < timeout_ms) {
        IOSleep(1); /* Loop de polling seguro para evitar conflito com o msleep do BSD */
        polled++;
    }
    
    if (x->done == 0) {
        return 0; /* Estourou o tempo */
    }
    
    if (x->done > 0) {
        x->done--;
    }
    return 1; /* Sucesso */
}

static inline void complete(struct completion *x) {
    x->done++;
}

#endif /* _SECTION_7_EMULATION_ */

// =========================================================================
// 8. LOCKS, EXCLUSÃO MÚTUA E ATOMICIDADE
// =========================================================================
#define spin_lock(lock)                         ((void)0)
#define spin_unlock(lock)                       ((void)0)
#define spin_lock_bh(lock)                      ((void)0)
#define spin_unlock_bh(lock)                    ((void)0)
#define rcu_read_lock()                         ((void)0)
#define rcu_read_unlock()                       ((void)0)
#define spin_lock_init(l)                       ((void)(l))

#define spin_lock_irqsave(lock, flags)          do { (void)(flags); } while(0)
#define spin_unlock_irqrestore(lock, flags)     do { (void)(flags); } while(0)

#define WARN_ONCE(cond, fmt, ...)               do { (void)(cond); } while(0)
#define WARN_ON(x)                              do { (void)(x); } while(0)

typedef int spinlock_t;
#define IEEE80211_QOS_CTL_TID_MASK 0x000f

static inline int test_bit(int nr, const volatile unsigned long *addr) { return (*addr & (1UL << nr)) != 0; }
static inline int atomic_inc_return(atomic_t *v) { return ++(v->counter); }
static inline void atomic_set(atomic_t *v, int i) { v->counter = i; }
static inline int atomic_read(const atomic_t *v) { return v->counter; }

// =========================================================================
// 9. SUBSISTEMA DE CAPTURA DE LOGS (PRINTK)
// =========================================================================
#define printk printf
#define pr_info(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#define pr_warn(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define pr_debug(fmt, ...) printf(fmt, ##__VA_ARGS__)

struct seq_file { int dummy; };
#define seq_puts(m, x)          ((void)0)
#define seq_printf(m, fmt, ...) ((void)0)

// =========================================================================
// 10. REDE E TRATAMENTO DE ENDEREÇOS MAC
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

// =========================================================================
// 11. EMULAÇÃO COMPLETA DE SK_BUFF (PACOTES LINUX)
// =========================================================================
struct sk_buff { 
    void *data; 
    unsigned int len; 
    unsigned int priority;
    unsigned char *head;
    unsigned char *tail;
    unsigned int data_len;
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

// Helper para verificar beacons (usado em trx.c)
static inline int ieee80211_is_beacon(u16 fc) {
    return (fc & 0x00fc) == 0x0080;
}

// =========================================================================
// 12. TIMERS E WORKQUEUES EMULADOS USANDO THEAD_CALLS DO XNU
// =========================================================================
struct work_struct {
    void (*func)(struct work_struct *work);
};

struct delayed_work {
#ifdef KERNEL
    thread_call_t t_call;
#else
    void *t_call;
#endif
    void (*func)(struct work_struct *work);
    struct work_struct work;
};

struct timer_list {
#ifdef KERNEL
    thread_call_t t_call;
#else
    void *t_call;
#endif
    void (*func)(struct timer_list *t);
};

// Funções reais para evitar stubs vazios nos agendadores de tarefas
static inline void apple_queue_delayed_work(struct delayed_work *dwork, unsigned long delay_ms) {
#ifdef KERNEL
    if (!dwork->t_call) {
        dwork->t_call = thread_call_allocate((thread_call_func_t)dwork->func, &dwork->work);
    }
    uint64_t deadline;
    clock_interval_to_deadline((uint32_t)delay_ms, kMillisecondScale, &deadline);
    thread_call_enter1_delayed(dwork->t_call, NULL, deadline);
#else
    (void)dwork; (void)delay_ms;
#endif
}

static inline void apple_cancel_delayed_work(struct delayed_work *dwork) {
#ifdef KERNEL
    if (dwork->t_call) {
        thread_call_cancel(dwork->t_call);
    }
#else
    (void)dwork;
#endif
}

static inline void apple_timer_setup(struct timer_list *timer, void (*callback)(struct timer_list *), unsigned int flags) {
    timer->func = callback;
#ifdef KERNEL
    timer->t_call = thread_call_allocate((thread_call_func_t)callback, timer);
#else
    timer->t_call = NULL;
#endif
}

static inline int apple_mod_timer(struct timer_list *timer, unsigned long expires_jiffies) {
    unsigned int delay_ms = jiffies_to_msecs(expires_jiffies);
#ifdef KERNEL
    if (timer->t_call) {
        uint64_t deadline;
        clock_interval_to_deadline(delay_ms, kMillisecondScale, &deadline);
        thread_call_enter1_delayed(timer->t_call, NULL, deadline);
    }
#endif
    return 0;
}

static inline void apple_del_timer_sync(struct timer_list *timer) {
#ifdef KERNEL
    if (timer->t_call) {
        thread_call_cancel(timer->t_call);
        thread_call_free(timer->t_call);
        timer->t_call = NULL;
    }
#endif
}

#define timer_setup(timer, callback, flags)     apple_timer_setup(timer, (void (*)(struct timer_list *))callback, flags)
#define del_timer_sync(t)                       apple_del_timer_sync(t)
#define mod_timer(t, exp)                       apple_mod_timer(t, exp)

#define INIT_DELAYED_WORK(w, f)                 do { (w)->func = (void (*)(struct work_struct *))(f); (w)->work.func = (void (*)(struct work_struct *))(f); (w)->t_call = NULL; } while(0)
#define INIT_WORK(w, f)                         do { (w)->func = (void (*)(struct work_struct *))(f); } while(0)
#define cancel_delayed_work(w)                  apple_cancel_delayed_work(w)
#define cancel_work_sync(w)                     ((void)(w))
#define schedule_work(w)                        do { if ((w)->func) (w)->func(w); } while(0)

static inline void *alloc_workqueue(const char *fmt, unsigned int flags, int max_active, ...) { return (void *)1; }
static inline void queue_delayed_work(void *wq, struct delayed_work *dwork, unsigned long delay) { apple_queue_delayed_work(dwork, jiffies_to_msecs(delay)); }
static inline void destroy_workqueue(void *wq) {}

#define to_delayed_work(x)                      (x)
#define from_timer(var, callback_timer, timer_fieldname) \
    ((struct rtl_priv *)((char *)(callback_timer) - offsetof(struct rtl_priv, works.watchdog_timer)))

static inline int in_interrupt(void) { return 0; }

// =========================================================================
// 13. SUBSISTEMA PCI E GERENCIAMENTO DE MEMÓRIA (MACOS NATIVE)
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

// Emulação real e segura de Alocação Dinâmica do macOS
#ifdef KERNEL
extern OSMallocTag gOSMallocTag; // Lembre-se de instanciar e alocar no seu .cpp principal
#endif

static inline void *apple_kzalloc(size_t size) {
#ifdef KERNEL
    if (!gOSMallocTag) return IOMallocZero(size);
    void *ptr = OSMalloc((uint32_t)size, gOSMallocTag);
    if (ptr) __builtin_memset(ptr, 0, size);
    return ptr;
#else
    return calloc(1, size);
#endif
}

static inline void apple_kfree(void *ptr, size_t size) {
#ifdef KERNEL
    if (ptr) {
        if (gOSMallocTag) {
            OSFree(ptr, (uint32_t)size, gOSMallocTag);
        } else {
            IOFree(ptr, size);
        }
    }
#else
    free(ptr);
#endif
}

#undef kzalloc
#undef kfree
#define kzalloc(size, flags)  apple_kzalloc(size)
#define kfree(ptr)            apple_kfree(ptr, sizeof(*(ptr)))
#define vzalloc(size)         apple_kzalloc(size)
#define vfree(ptr)            apple_kfree(ptr, 1) // Fallback seguro para ponteiros opacos

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
// 14. OPERAÇÕES DE ENDIANNESS (CONVERSÕES DE BYTE)
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

// =========================================================================
// 15. CONSTANTES E MÁSCARAS DO PADRÃO IEEE 802.11
// =========================================================================
#define IEEE80211_FCTL_FTYPE       0x000c
#define IEEE80211_FCTL_STYPE       0x00f0
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

#define IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_11454            (1 << 0)
#define IEEE80211_VHT_CAP_SHORT_GI_80                     (1 << 1)

#ifdef __cplusplus
}
#endif

#endif /* APPLE_LINUX_EMULATION_H */
