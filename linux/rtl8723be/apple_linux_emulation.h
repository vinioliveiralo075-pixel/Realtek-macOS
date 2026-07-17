#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h>
#include <libkern/libkern.h>
#include <stddef.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/param.h> 
#include <stdbool.h>
#include <stdint.h>

// --- Macros de Otimização do Compilador GCC/Clang ---
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#ifdef KERNEL
#include <libkern/OSMalloc.h>
#include <sys/malloc.h>
#include <kern/thread_call.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Seção 1: Cabeçalhos Básicos, Tipos Primitivos e Módulos do Linux
 *******************************************************************************/
typedef unsigned char        u8;
typedef unsigned short       u16;
typedef unsigned int         u32;
typedef unsigned long long   u64;
typedef signed char          s8;
typedef short                s16;
typedef int                  s32;
typedef long long            s64;

typedef u32 __le32;
typedef u16 __le16;
typedef u32 __be32;
typedef u16 __be16;
typedef u64 __le64;
typedef u64 __be64;

typedef u16 __sum16;
typedef u32 __wsum;

typedef unsigned int gfp_t;
typedef long long            time64_t;
typedef unsigned long dma_addr_t;
typedef unsigned long kernel_ulong_t;

#define __init
#define __exit
#define __iomem
#define __packed __attribute__((packed))
#define __aligned(x) __attribute__((aligned(x)))
#define __always_unused __attribute__((unused))
#define __maybe_unused __attribute__((unused))
#define __must_check __attribute__((warn_unused_result))

#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_VERSION(x)
#define MODULE_PARM_DESC(x, y)
#define module_param(name, type, perm)
#define module_param_named(name, value, type, perm)
#define module_init(initfn) static inline void __init_##initfn(void) { (void)initfn; }
#define module_exit(exitfn) static inline void __exit_##exitfn(void) { (void)exitfn; }

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif
#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(5, 4, 0)
#endif
#ifndef __printf
#define __printf(a, b) __attribute__((format(printf, a, b)))
#endif

/*******************************************************************************
 * Seção 2: Macros Matemáticas, Alinhamento e Manipulação de Bits
 *******************************************************************************/
#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif
#ifndef BIT_ULL
#define BIT_ULL(nr) (1ULL << (nr))
#endif
#ifndef BITS_PER_LONG
#define BITS_PER_LONG 64
#endif

#define GENMASK(h, l) (((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))
#define GENMASK_ULL(h, l) (((~0ULL) - (1ULL << (l)) + 1) & (~0ULL >> (64 - 1 - (h))))

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define DIV_ROUND_CLOSEST(x, divisor)({\
    typeof(x) __x = x;\
    typeof(divisor) __d = divisor;\
    (((__x) + ((__d) / 2)) / (__d));\
})

#define clamp(val, lo, hi) (min(max(val, lo), hi))
#define abs(x) ({ long __x = (x); (__x < 0) ? -__x : __x; })

#undef memcpy
#define memcpy(dst, src, n) __builtin_memcpy(dst, src, n)
#undef memset
#define memset(dst, val, n) __builtin_memset(dst, val, n)
#undef memmove
#define memmove(dst, src, n) __builtin_memmove(dst, src, n)
#undef memcmp
#define memcmp(s1, s2, n) __builtin_memcmp(s1, s2, n)

/*******************************************************************************
 * Seção 3: Listas Duplamente Encadeadas do Linux (List e Hlist)
 *******************************************************************************/
struct list_head {
    struct list_head *next, *prev;
};

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head *new_node, struct list_head *prev, struct list_head *next) {
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

static inline void list_add(struct list_head *new_node, struct list_head *head) {
    __list_add(new_node, head, head->next);
}

static inline void list_add_tail(struct list_head *new_node, struct list_head *head) {
    __list_add(new_node, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next) {
    next->prev = prev;
    prev->next = next;
}

static inline void list_del(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

static inline void list_del_init(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

static inline int list_empty(const struct list_head *head) {
    return head->next == head;
}

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - __builtin_offsetof(type, member)))
#endif

#define list_first_entry(ptr, type, member) \
    container_of((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) \
    container_of((ptr)->prev, type, member)

#define list_next_entry(pos, member) \
    container_of((pos)->member.next, typeof(*(pos)), member)

#define list_prev_entry(pos, member) \
    container_of((pos)->member.prev, typeof(*(pos)), member)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member) \
    for (pos = container_of((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = container_of(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member) \
    for (pos = container_of((head)->next, typeof(*pos), member), \
         n = container_of(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = container_of(n->member.next, typeof(*n), member))

#define list_for_each_entry_reverse(pos, head, member) \
    for (pos = container_of((head)->prev, typeof(*pos), member); \
         &pos->member != (head); \
         pos = container_of(pos->member.prev, typeof(*pos), member))

/*******************************************************************************
 * Seção 4: Alinhamentos de Tipos de Dados e Estruturas de Redes IEEE 802.11
 *******************************************************************************/
enum nl80211_iftype {
    NL80211_IFTYPE_UNSPECIFIED,
    NL80211_IFTYPE_ADHOC,
    NL80211_IFTYPE_STATION,
    NL80211_IFTYPE_AP,
    NL80211_IFTYPE_AP_VLAN,
    NL80211_IFTYPE_WDS,
    NL80211_IFTYPE_MONITOR,
    NL80211_IFTYPE_MESH_POINT,
    NL80211_IFTYPE_P2P_CLIENT,
    NL80211_IFTYPE_P2P_GO,
    NL80211_IFTYPE_NAN
};

enum nl80211_band {
    NL80211_BAND_2GHZ,
    NL80211_BAND_5GHZ,
    NL80211_BAND_60GHZ,
    NL80211_BAND_6GHZ
};

enum nl80211_channel_type {
    NL80211_CHAN_NO_HT,
    NL80211_CHAN_HT20,
    NL80211_CHAN_HT40MINUS,
    NL80211_CHAN_HT40PLUS
};

enum nl80211_key_attributes {
    NL80211_KEY_IDX,
    NL80211_KEY_DATA,
    NL80211_KEY_SEQ,
    NL80211_KEY_CIPHER
};

enum ieee80211_smps_mode {
    IEEE80211_SMPS_OFF,
    IEEE80211_SMPS_STATIC,
    IEEE80211_SMPS_DYNAMIC
};

enum ieee80211_cipher_type {
    IEEE80211_CIPHER_NONE,
    IEEE80211_CIPHER_WEP40,
    IEEE80211_CIPHER_WEP104,
    IEEE80211_CIPHER_TKIP,
    IEEE80211_CIPHER_CCMP,
    IEEE80211_CIPHER_AES_CMAC,
    IEEE80211_CIPHER_GCMP
};

#define NUM_NL80211_BANDS 4

struct ieee80211_supported_band {
    int bitrates;
    int channels;
    u8 n_channels;
    u32 band;
};

struct wiphy {
    char name[32];
    u8 perm_addr[6];
    u32 bands;
    u32 signal_type;
    u32 max_scan_ssids;
    u32 max_sched_scan_ssids;
    u32 interface_modes;
    void *priv;
};

struct regulatory_request {
    char alpha2[2];
    u8 initiator;
    u8 user_reg_hint_type;
};

struct ieee80211_tx_info {
    u8 flags;
    u8 control_rate;
    u8 rate_idx;
    void *cb;
};

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
} __packed;

struct ieee80211_tx_queue_params {
    u8 aifs;
    u16 cw_min;
    u16 cw_max;
    u16 txop_limit;
};

struct tasklet_struct {
    void (*func)(unsigned long);
    unsigned long data;
    unsigned long state;
};

struct urb {
    void *context;
    int status;
    u32 transfer_buffer_length;
    u32 actual_length;
};

struct ieee80211_sta {
    u8 mac_addr[6];
    void *driver_priv;
    void *drv_priv; 
    unsigned int aid;
    unsigned char supp_rates[16];
    struct {
        unsigned int cap;
        struct {
            unsigned char rx_mask[2];
        } mcs;
    } ht_cap;
    struct list_head list;
};

struct wireless_dev {
    int ifindex;
    enum nl80211_iftype iftype;
    struct wiphy *wiphy;
    u64 wdev_id;
};

struct ieee80211_hw {
    void *priv; 
    struct wiphy *wiphy;
    u8 mac_addr[6];
    enum nl80211_iftype opmode;
    struct ieee80211_supported_band bands[NUM_NL80211_BANDS];
    struct wireless_dev *wdev;
};

struct ieee80211_rx_status {
    u16 qual;
    u8 signal;
    u8 noise;
    u32 flags;
    u8 mcs;
    u8 bw;
};

/*******************************************************************************
 * Seção 5: Mutexes, Spinlocks e Sincronização Recursiva
 *******************************************************************************/
struct mutex {
    void *apple_mutex_ptr;
    int locked;
};

typedef int spinlock_t;
typedef int rwlock_t;
typedef int seqlock_t;

#define spin_lock(lock)                         ((void)0)
#define spin_unlock(lock)                       ((void)0)
#define spin_lock_bh(lock)                      ((void)0)
#define spin_unlock_bh(lock)                    ((void)0)
#define rcu_read_lock()                         ((void)0)
#define rcu_read_unlock()                       ((void)0)
#define spin_lock_init(l)                       ((void)(l))
#define mutex_init(m)                           do { (m)->locked = 0; (m)->apple_mutex_ptr = NULL; } while(0)
#define mutex_lock(m)                           do { (m)->locked = 1; } while(0)
#define mutex_unlock(m)                         do { (m)->locked = 0; } while(0)

#define spin_lock_irqsave(lock, flags)          do { (flags) = 0; (void)(lock); } while(0)
#define spin_unlock_irqrestore(lock, flags)     do { (void)(lock); (void)(flags); } while(0)

#define rcu_assign_pointer(p, v)                do { p = v; } while(0)
#define rcu_dereference(p)                      (p)
#define rcu_dereference_protected(p, c)         (p)

struct completion {
    unsigned int done;
    void *event_chan;
};

static inline void init_completion(struct completion *x) {
    x->done = 0;
    x->event_chan = (void *)x;
}

static inline void reinit_completion(struct completion *x) {
    x->done = 0; 
}

static inline void complete(struct completion *x) {
    x->done++;
}

/*******************************************************************************
 * Seção 6: Operações Atômicas e Bitmaps do Kernel
 *******************************************************************************/
typedef struct { volatile int counter; } atomic_t;
typedef struct { volatile long counter; } atomic_long_t;

#define ATOMIC_INIT(i) { (i) }

static inline int atomic_read(const atomic_t *v) { return v->counter; }
static inline void atomic_set(atomic_t *v, int i) { v->counter = i; }
static inline void atomic_inc(atomic_t *v) { __sync_fetch_and_add(&v->counter, 1); }
static inline void atomic_dec(atomic_t *v) { __sync_fetch_and_sub(&v->counter, 1); }
static inline int atomic_inc_return(atomic_t *v) { return __sync_add_and_fetch(&v->counter, 1); }
static inline int atomic_dec_return(atomic_t *v) { return __sync_sub_and_fetch(&v->counter, 1); }
static inline int atomic_add_return(int i, atomic_t *v) { return __sync_add_and_fetch(&v->counter, i); }
static inline int atomic_sub_return(int i, atomic_t *v) { return __sync_sub_and_fetch(&v->counter, i); }

static inline int test_bit(int nr, const volatile unsigned long *addr) {
    return ((*addr) & (1UL << nr)) != 0;
}
static inline void set_bit(int nr, volatile unsigned long *addr) {
    __sync_fetch_and_or(addr, (1UL << nr));
}
static inline void clear_bit(int nr, volatile unsigned long *addr) {
    __sync_fetch_and_and(addr, ~(1UL << nr));
}
static inline int test_and_set_bit(int nr, volatile unsigned long *addr) {
    unsigned long mask = (1UL << nr);
    return (__sync_fetch_and_or(addr, mask) & mask) != 0;
}
static inline int test_and_clear_bit(int nr, volatile unsigned long *addr) {
    unsigned long mask = (1UL << nr);
    return (__sync_fetch_and_and(addr, ~mask) & mask) != 0;
}

/*******************************************************************************
 * Seção 7: MMIO e Manipulação Registradores de Hardware (PCI Read/Write)
 *******************************************************************************/
#define readb(addr)        (*(volatile u8 *)(addr))
#define readw(addr)        (*(volatile u16 *)(addr))
#define readl(addr)        (*(volatile u32 *)(addr))
#define writeb(val, addr)  (*(volatile u8 *)(addr) = (val))
#define writew(val, addr)  (*(volatile u16 *)(addr) = (val))
#define writel(val, addr)  (*(volatile u32 *)(addr) = (val))

#define ioread8(addr)      readb(addr)
#define ioread16(addr)     readw(addr)
#define ioread32(addr)     readl(addr)
#define iowrite8(v, addr)  writeb(v, addr)
#define iowrite16(v, addr) writew(v, addr)
#define iowrite32(v, addr) writel(v, addr)

/*******************************************************************************
 * Seção 8: Timers do Kernel, Jiffies e Escala Temporal do macOS
 *******************************************************************************/
extern uint64_t clock_get_absolute_time(void);
extern void absolutetime_to_nanoseconds(uint64_t clock_time, uint64_t *nanoseconds);

#define HZ 1000

static inline unsigned long apple_get_jiffies(void) {
    uint64_t abstime = clock_get_absolute_time();
    uint64_t nanos;
    absolutetime_to_nanoseconds(abstime, &nanos);
    return (unsigned long)(nanos / 1000000); 
}

#define jiffies apple_get_jiffies()

#define jiffies_to_msecs(x) ((unsigned int)(((unsigned long)(x) * 1000) / HZ))
#define msecs_to_jiffies(x) ((unsigned long)(((unsigned long)(x) * HZ) / 1000))
#define usecs_to_jiffies(x) ((unsigned long)(((unsigned long)(x) * HZ) / 1000000))

#define time_after(a,b)        ((long)((b) - (a)) < 0)
#define time_before(a,b)       ((long)((a) - (b)) < 0)
#define time_after_eq(a,b)     ((long)((b) - (a)) <= 0)
#define time_before_eq(a,b)    ((long)((a) - (b)) <= 0)

#define mdelay(ms) IOSleep(ms)
#define udelay(us) do { \
    int _i; \
    for(_i = 0; _i < (int)(us) * 10; _i++) { \
        asm volatile(""); \
    } \
} while(0)

static inline void linux_msleep(unsigned int ms) { IOSleep(ms); }
static inline void usleep_range(unsigned long min, unsigned long max) { IOSleep((unsigned int)(min / 1000)); }
#define msleep(ms) linux_msleep(ms)
#define ssleep(sec) linux_msleep((sec) * 1000)

static inline unsigned long wait_for_completion_timeout(struct completion *x, unsigned long timeout) {
    int timeout_ms = (int)jiffies_to_msecs(timeout);
    int polled = 0;
    while (x->done == 0 && polled < timeout_ms) {
        IOSleep(1); 
        polled++;
    }
    if (x->done == 0) return 0; 
    if (x->done > 0) x->done--;
    return 1; 
}

/*******************************************************************************
 * Seção 9: Gestão Assíncrona de Trabalho (Thread Calls / Workqueues)
 *******************************************************************************/
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
    unsigned long expires;
};

#ifdef KERNEL
static inline void apple_work_wrapper(thread_call_param_t param0, thread_call_param_t param1) {
    struct work_struct *work = (struct work_struct *)param0;
    if (work && work->func) work->func(work);
}
static inline void apple_timer_wrapper(thread_call_param_t param0, thread_call_param_t param1) {
    struct timer_list *timer = (struct timer_list *)param0;
    if (timer && timer->func) timer->func(timer);
}
#endif

static inline void apple_queue_delayed_work(struct delayed_work *dwork, unsigned long delay_ms) {
#ifdef KERNEL
    if (!dwork->t_call) {
        dwork->t_call = thread_call_allocate(apple_work_wrapper, (thread_call_param_t)&dwork->work);
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
    if (dwork->t_call) thread_call_cancel(dwork->t_call);
#else
    (void)dwork;
#endif
}

static inline void apple_timer_setup(struct timer_list *timer, void (*callback)(struct timer_list *), unsigned int flags) {
    timer->func = callback;
#ifdef KERNEL
    timer->t_call = thread_call_allocate(apple_timer_wrapper, (thread_call_param_t)timer);
#else
    timer->t_call = NULL;
#endif
}

static inline int apple_mod_timer(struct timer_list *timer, unsigned long expires_jiffies) {
    timer->expires = expires_jiffies;
    long diff = (long)(expires_jiffies - jiffies);
    unsigned int delay_ms = diff > 0 ? jiffies_to_msecs(diff) : 0;
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
#define del_timer(t)                            apple_del_timer_sync(t)
#define mod_timer(t, exp)                       apple_mod_timer(t, exp)

#define INIT_DELAYED_WORK(w, f)                 do { (w)->func = (void (*)(struct work_struct *))(f); (w)->work.func = (void (*)(struct work_struct *))(f); (w)->t_call = NULL; } while(0)
#define INIT_WORK(w, f)                         do { (w)->func = (void (*)(struct work_struct *))(f); } while(0)
#define cancel_delayed_work(w)                  (apple_cancel_delayed_work(w), true)
#define cancel_delayed_work_sync(w)             (apple_cancel_delayed_work(w), true)
#define cancel_work_sync(w)                     ((void)(w))
#define schedule_work(w)                        do { if ((w)->func) (w)->func(w); } while(0)

static inline void *alloc_workqueue(const char *fmt, unsigned int flags, int max_active, ...) { return (void *)1; }
static inline void queue_delayed_work(void *wq, struct delayed_work *dwork, unsigned long delay) { apple_queue_delayed_work(dwork, jiffies_to_msecs(delay)); }
static inline bool queue_work(void *wq, struct work_struct *work) { if (work && work->func) { work->func(work); return true; } return false; }
static inline void destroy_workqueue(void *wq) {}
static inline void flush_workqueue(void *wq) {}

#define to_delayed_work(x)                      (x)

struct rtl_priv;
#define from_timer(var, callback_timer, timer_fieldname) \
    ((struct rtl_priv *)((char *)(callback_timer) - __builtin_offsetof(struct rtl_priv, works.watchdog_timer)))

static inline int in_interrupt(void) { return 0; }

/*******************************************************************************
 * Seção 10: Subsistema de Logs Avançados do Driver (Printk Dinâmico)
 *******************************************************************************/
#define printk printf
#define pr_info(fmt, ...)  printf("rtl8723be: [INFO] " fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   printf("rtl8723be: [ERR] " fmt, ##__VA_ARGS__)
#define pr_warn(fmt, ...)  printf("rtl8723be: [WARN] " fmt, ##__VA_ARGS__)
#define pr_debug(fmt, ...) printf("rtl8723be: [DBG] " fmt, ##__VA_ARGS__)

#define dev_info(dev, fmt, ...)  printf("rtl8723be_dev: " fmt, ##__VA_ARGS__)
#define dev_err(dev, fmt, ...)   printf("rtl8723be_dev_err: " fmt, ##__VA_ARGS__)
#define dev_warn(dev, fmt, ...)  printf("rtl8723be_dev_warn: " fmt, ##__VA_ARGS__)
#define dev_dbg(dev, fmt, ...)   printf("rtl8723be_dev_dbg: " fmt, ##__VA_ARGS__)

#define WARN_ONCE(cond, fmt, ...)               do { (void)(cond); } while(0)
#define WARN_ON(x)                              ({ int __ret_warn = !!(x); (void)__ret_warn; __ret_warn; })
#define BUG_ON(cond)                            do { if (cond) { panic("Linux BUG_ON executado!"); } } while(0)

struct seq_file { int dummy; };
#define seq_puts(m, x)          ((void)0)
#define seq_printf(m, fmt, ...) ((void)0)

/*******************************************************************************
 * Seção 11: Endereçamento Mac, Protocolos e Redes
 *******************************************************************************/
#define ETH_ALEN 6
#define ETH_HLEN 14
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
static inline bool is_valid_ether_addr(const unsigned char *addr) { 
    if (addr[0] & 1) return false;
    if (addr[0] == 0 && addr[1] == 0 && addr[2] == 0 && addr[3] == 0 && addr[4] == 0 && addr[5] == 0) return false;
    return true; 
}
static inline void eth_zero_addr(u8 *addr) { __builtin_memset(addr, 0, ETH_ALEN); }
static inline void eth_broadcast_addr(u8 *addr) { __builtin_memset(addr, 0xff, ETH_ALEN); }

/*******************************************************************************
 * Seção 12: Emulação das Estruturas e Estratégias do sk_buff (Buffers Linux)
 *******************************************************************************/
struct skb_frag_struct {
    void *page;
    u32 page_offset;
    u32 size;
};

struct skb_shared_info {
    u8 nr_frags;
    u8 tx_flags;
    u16 gso_size;
    u16 gso_segs;
    struct skb_frag_struct frags[16];
};

struct sk_buff { 
    struct sk_buff *next;
    struct sk_buff *prev;
    void *dev;
    void *data; 
    unsigned int len; 
    unsigned int priority;
    unsigned char *head;
    unsigned char *data_ptr;
    unsigned char *tail;
    unsigned char *end;
    unsigned int data_len;
    unsigned int truesize;
    u16 protocol;
    u16 queue_mapping;
    u8 ip_summed;
    __u32 mark;
};

struct sk_buff_head { 
    struct sk_buff *next;
    struct sk_buff *prev;
    u32 qlen;
    spinlock_t lock;
};

static inline void skb_queue_head_init(struct sk_buff_head *list) {
    list->next = (struct sk_buff *)list;
    list->prev = (struct sk_buff *)list;
    list->qlen = 0;
}

static inline struct sk_buff *dev_alloc_skb(unsigned int length) { 
    struct sk_buff *skb = (struct sk_buff *)IOMallocZero(sizeof(struct sk_buff));
    if (!skb) return NULL;
    void *payload = IOMallocZero(length + 64);
    if (!payload) { IOFree(skb, sizeof(struct sk_buff)); return NULL; }
    skb->head = (unsigned char *)payload;
    skb->data = (unsigned char *)payload + 32;
    skb->tail = (unsigned char *)payload + 32;
    skb->end = (unsigned char *)payload + length + 64;
    skb->len = 0;
    skb->truesize = length + 64 + sizeof(struct sk_buff);
    return skb; 
}

static inline void kfree_skb(struct sk_buff *skb) {
    if (skb) {
        if (skb->head) {
            IOFree(skb->head, skb->end - skb->head);
        }
        IOFree(skb, sizeof(struct sk_buff));
    }
}
static inline void dev_kfree_skb(struct sk_buff *skb) { kfree_skb(skb); }
static inline void dev_kfree_skb_any(struct sk_buff *skb) { kfree_skb(skb); }
static inline void dev_kfree_skb_irq(struct sk_buff *skb) { kfree_skb(skb); }

static inline unsigned char *skb_put(struct sk_buff *skb, unsigned int len) {
    unsigned char *tmp = skb->tail;
    skb->tail += len;
    skb->len += len;
    return tmp;
}

static inline void skb_put_data(struct sk_buff *skb, const void *data, unsigned int len) {
    __builtin_memcpy(skb_put(skb, len), data, len);
}

static inline unsigned char *skb_push(struct sk_buff *skb, unsigned int len) {
    skb->data = (void *)((char *)skb->data - len);
    skb->len += len;
    return (unsigned char *)skb->data;
}

static inline unsigned char *skb_pull(struct sk_buff *skb, unsigned int len) {
    skb->data = (void *)((char *)skb->data + len);
    skb->len -= len;
    return (unsigned char *)skb->data;
}

static inline void skb_reserve(struct sk_buff *skb, int len) {
    skb->data += len;
    skb->tail += len;
}

static inline int skb_queue_len(const struct sk_buff_head *list) { return list->qlen; }

static inline void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk) {
    struct sk_buff *prev = list->prev;
    newsk->next = (struct sk_buff *)list;
    newsk->prev = prev;
    prev->next = newsk;
    list->prev = newsk;
    list->qlen++;
}

static inline struct sk_buff *skb_dequeue(struct sk_buff_head *list) {
    if (list->qlen == 0) return NULL;
    struct sk_buff *skb = list->next;
    struct sk_buff *next = skb->next;
    list->next = next;
    next->prev = (struct sk_buff *)list;
    list->qlen--;
    return skb;
}

static inline void skb_queue_purge(struct sk_buff_head *list) {
    struct sk_buff *skb;
    while ((skb = skb_dequeue(list)) != NULL) {
        kfree_skb(skb);
    }
}

static inline struct skb_shared_info *skb_shinfo(const struct sk_buff *skb) {
    static struct skb_shared_info dummy_shinfo;
    return &dummy_shinfo;
}

struct iphdr { u8 ihl:4, version:4; u8 tos; u16 tot_len; u16 id; u16 frag_off; u8 ttl; u8 protocol; u16 check; u32 saddr; u32 daddr; };

static inline int ieee80211_is_beacon(u16 fc) { return (fc & 0x00fc) == 0x0080; }

/*******************************************************************************
 * Seção 13: Subsistema de Memória Protegido (Prevenção Absoluta de Kernel Panic)
 *******************************************************************************/
static inline void *apple_kzalloc(size_t size) {
#ifdef KERNEL
    size_t total_size = size + 16;
    void *raw_ptr = IOMallocZero(total_size);
    if (!raw_ptr) return NULL;
    *(size_t *)raw_ptr = total_size; 
    return (void *)((char *)raw_ptr + 16);
#else
    return calloc(1, size);
#endif
}

static inline void apple_kfree(void *ptr) {
#ifdef KERNEL
    if (ptr) {
        void *raw_ptr = (void *)((char *)ptr - 16);
        size_t total_size = *(size_t *)raw_ptr;
        IOFree(raw_ptr, total_size); 
    }
#else
    free(ptr);
#endif
}

#undef kzalloc
#undef kfree
#define kzalloc(size, flags)  apple_kzalloc(size)
#define kfree(ptr)            apple_kfree(ptr)
#define kmalloc(size, flags)  apple_kzalloc(size)
#define vzalloc(size)         apple_kzalloc(size)
#define vfree(ptr)            apple_kfree(ptr) 
#define kcalloc(n, size, f)   apple_kzalloc((n) * (size))

/*******************************************************************************
 * Seção 14: Barramento PCI e Camada Estrutural de Driver Linux
 *******************************************************************************/
struct pci_device_id {
    unsigned int vendor, device;
    unsigned int subvendor, subdevice;
    union {
        unsigned int class_id;  
        unsigned int class;     
    };
    unsigned int class_mask;
    kernel_ulong_t driver_data;
};

#define PCI_VENDOR_ID_REALTEK 0x10ec
#define PCI_ANY_ID            (~0U)
#define THIS_MODULE           NULL
#define KBUILD_MODNAME        "rtl8723be"

#define MODULE_DEVICE_TABLE(type, name)
#define MODULE_FIRMWARE(name)
#define module_pci_driver(driver)
#define EXPORT_SYMBOL_GPL(x)
#define EXPORT_SYMBOL(x)

struct device {
    void *driver_data;
    struct device *parent;
};

struct pci_dev {
    unsigned int devfn;
    unsigned short vendor;
    unsigned short device;
    unsigned short subsystem_vendor;
    unsigned short subsystem_device;
    unsigned int class;
    struct device dev;
    u8 revision;
    bool msix_enabled;
};

static inline void *pci_get_drvdata(struct pci_dev *pdev) { return pdev->dev.driver_data; }
static inline void pci_set_drvdata(struct pci_dev *pdev, void *data) { pdev->dev.driver_data = data; }

static inline dma_addr_t pci_map_single(struct pci_dev *pdev, void *ptr, size_t size, int direction) { return (dma_addr_t)ptr; }
static inline void pci_unmap_single(struct pci_dev *pdev, dma_addr_t dma_addr, size_t size, int direction) { }
static inline int pci_dma_mapping_error(struct pci_dev *pdev, dma_addr_t dma_addr) { return 0; }

static inline void *pci_alloc_consistent(struct pci_dev *pdev, size_t size, dma_addr_t *dma_handle) {
    void *ptr = apple_kzalloc(size);
    *dma_handle = (dma_addr_t)ptr;
    return ptr;
}
static inline void pci_free_consistent(struct pci_dev *pdev, size_t size, void *vaddr, dma_addr_t dma_handle) {
    apple_kfree(vaddr);
}

#define pci_resource_start(pdev, bar) 0
#define pci_resource_end(pdev, bar)   0
#define pci_resource_len(pdev, bar)   0
#define pci_resource_flags(pdev, bar) 0

static inline int pci_enable_device(struct pci_dev *pdev) { return 0; }
static inline void pci_disable_device(struct pci_dev *pdev) {}
static inline void pci_set_master(struct pci_dev *pdev) {}
static inline int pci_set_dma_mask(struct pci_dev *pdev, u64 mask) { return 0; }
static inline int pci_set_consistent_dma_mask(struct pci_dev *pdev, u64 mask) { return 0; }
static inline int pci_request_regions(struct pci_dev *pdev, const char *res_name) { return 0; }
static inline void pci_release_regions(struct pci_dev *pdev) {}
static inline void *pci_ioremap_bar(struct pci_dev *pdev, int bar) { return (void *)0xDEADBEEF; }

struct dev_pm_ops { 
    int (*suspend)(struct device *dev);
    int (*resume)(struct device *dev);
    int (*freeze)(struct device *dev);
    int (*thaw)(struct device *dev);
    int (*poweroff)(struct device *dev);
    int (*restore)(struct device *dev);
};
#define SIMPLE_DEV_PM_OPS(name, suspend_fn, resume_fn) \
static const struct dev_pm_ops name = { .suspend = suspend_fn, .resume = resume_fn }

struct mac_dummy_driver { const struct dev_pm_ops *pm; };
struct pci_driver {
    const char *name;
    const struct pci_device_id *id_table;
    int (*probe)(struct pci_dev *dev, const struct pci_device_id *id);
    void (*remove)(struct pci_dev *dev);
    void (*shutdown)(struct pci_dev *dev);
    struct mac_dummy_driver driver;
};

/*******************************************************************************
 * Seção 15: Macros Otimizadas de Conversão Inversa de Bytes (Endianness)
 *******************************************************************************/
#ifndef cpu_to_le32
#define cpu_to_le32(x) ((__le32)(u32)(x))
#endif
#ifndef le32_to_cpu
#define le32_to_cpu(x) ((u32)(__le32)(x))
#endif
#ifndef cpu_to_le16
#define cpu_to_le16(x) ((__le16)(u16)(x))
#endif
#ifndef le16_to_cpu
#define le16_to_cpu(x) ((u16)(__le16)(x))
#endif
#ifndef cpu_to_be32
#define cpu_to_be32(x) __builtin_bswap32((u32)(x))
#endif
#ifndef be32_to_cpu
#define be32_to_cpu(x) __builtin_bswap32((u32)(x))
#endif
#ifndef cpu_to_be16
#define cpu_to_be16(x) __builtin_bswap16((u16)(x))
#endif
#ifndef be16_to_cpu
#define be16_to_cpu(x) __builtin_bswap16((u16)(x))
#endif

static inline unsigned short be16_to_cpup(const __be16 *p) { return be16_to_cpu(*p); }
static inline unsigned int be32_to_cpup(const __be32 *p) { return be32_to_cpu(*p); }
static inline unsigned short le16_to_cpup(const __le16 *p) { return le16_to_cpu(*p); }
static inline unsigned int le32_to_cpup(const __le32 *p) { return le32_to_cpu(*p); }

static inline u64 div64_u64(u64 dividend, u64 divisor) { return dividend / divisor; }

/*******************************************************************************
 * Seção 16: Constantes Mac/PHY, Máscaras de Bits e Registros IEEE 802.11
 *******************************************************************************/
#define IEEE80211_FCTL_FTYPE       0x000c
#define IEEE80211_FCTL_STYPE       0x00f0
#define IEEE80211_FTYPE_MGMT       0x0000
#define IEEE80211_FTYPE_CTL        0x0004
#define IEEE80211_FTYPE_DATA       0x0008
#define IEEE80211_SCTL_FRAG        0x000f
#define IEEE80211_SCTL_SEQ         0xfff0
#define IEEE80211_FCTL_TODS        0x0100
#define IEEE80211_FCTL_FROMDS      0x0200
#define IEEE80211_FCTL_MOREFRAGS   0x0400
#define IEEE80211_TX_CTL_AMPDU     0x00000002
#define IEEE80211_STYPE_ACTION     0x00d0
#define IEEE80211_STYPE_BEACON     0x0080

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

#define RTL_MAC80211_NUM_QUEUE 5
#define IEEE80211_QOS_CTL_TID_MASK 0x000f

/*******************************************************************************
 * Seção 17: Constantes e Métricas Adicionais de Alocação de Redes DMA
 *******************************************************************************/
#define PCI_DMA_BIDIRECTIONAL  0
#define PCI_DMA_TODEVICE       1
#define PCI_DMA_FROMDEVICE     2
#define PCI_DMA_NONE           3

#ifndef DMA_BIT_MASK
#define DMA_BIT_MASK(n) (((n) == 64) ? ~0ULL : ((1ULL << (n)) - 1))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#define local_save_flags(flags)     do { (flags) = 0; } while(0)
#define local_irq_enable()          do { } while(0)
#define local_irq_disable()         do { } while(0)
#define local_irq_restore(flags)    do { (void)(flags); } while(0)

#define eth_type_trans(skb, dev) 0
#define netif_rx(skb) 0
#define netif_rx_ni(skb) 0

static inline void *netdev_priv(void *dev) { return (void *)((char *)dev + 256); }
static inline void netif_start_queue(void *dev) {}
static inline void netif_stop_queue(void *dev) {}
static inline void netif_wake_queue(void *dev) {}
static inline bool netif_queue_stopped(void *dev) { return false; }

#ifdef __cplusplus
}
#endif

#endif /* APPLE_LINUX_EMULATION_H */
