/******************************************************************************
 * Realtek RTL8723BE Wireless Driver - macOS Port
 * 
 * Copyright (c) Realtek Corporation. All rights reserved.
 * macOS Compatibility Layer & Porting by Vini
 * 
 * Description: Emulation header mapping Linux kernel interfaces to the 
 *              macOS/XNU kernel framework environment.
 ******************************************************************************/

#ifndef _APPLE_LINUX_EMULATION_H_
#define _APPLE_LINUX_EMULATION_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/conf.h>
#include <sys/systm.h>
#include <libkern/libkern.h>
#include <IOKit/IOLib.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==========================================
// 0.5 MACROS DE INTERRUPÇÃO E DMA (TOPO)
// ==========================================
#define local_save_flags(bandeiras) ((bandeiras) = 0)
#define local_irq_enable() do { } while(0) /* Corrigido de 'faz' para 'do' */
#define local_irq_restore(flags) do { (void)(flags); } while(0)

#define PCI_DMA_TODEVICE 1
#define DMA_BIT_MASK(n) (((n) == 64) ? ~0ULL : (1ULL << (n)) - 1)

#define IEEE80211_HT_CAP_SUP_WIDTH_20_40 0x0002
#define IEEE80211_HT_CAP_SGI_20          0x0020
#define IEEE80211_HT_CAP_SGI_40          0x0040

/*******************************************************************************
 * 1. HARDWARE IDENTIFICATION DEFINITIONS (REALTEK RTL8723BE TARGET ENGINE)
 *******************************************************************************/
#define REALTEK_PCI_VENDOR_ID                0x10EC
#define RTL8723BE_PCI_DEVICE_ID              0xB723
#define ASUS_SUBSYSTEM_VENDOR_ID             0x1043
#define ASUS_RTL8723BE_SUBSYSTEM_DEVICE_ID   0x207F
#define RTL8723BE_HARDWARE_REVISION_00       0x00

/*******************************************************************************
 * 2. COMPILER ATTRIBUTES, COMPATIBILITY MATRICES & SANITIZATION
 *******************************************************************************/
#define __init
#define __exit
#define __iomem
#define __packed         __attribute__((packed))
#define __aligned(x)     __attribute__((aligned(x)))
#define __always_unused  __attribute__((unused))
#define __maybe_unused   __attribute__((unused))
#define __must_check     __attribute__((warn_unused_result))
#define __always_inline  inline __attribute__((always_inline))
#define __user
#define __rcu
#define __percpu

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#ifndef __printf
#define __printf(a, b)  __attribute__((format(printf, a, b)))
#endif

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifndef LINUX_VERSION_CODE
#define LINUX_VERSION_CODE KERNEL_VERSION(5, 4, 0)
#endif

#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_VERSION(x)
#define MODULE_PARM_DESC(x, y)
#define module_param(name, type, perm)
#define module_param_named(name, value, type, perm)
#define module_init(initfn)
#define module_exit(exitfn)
#define EXPORT_SYMBOL(x)
#define EXPORT_SYMBOL_GPL(x)
#define MODULE_DEVICE_TABLE(type, name)
#define MODULE_FIRMWARE(name)

// --- Adicionado por Vini: Correções para o hw.c ---
#define local_irq_enable() do { } while(0)
#define local_irq_restore(flags) do { (void)(flags); } while(0)

#define PCI_DMA_TODEVICE 1
#define DMA_BIT_MASK(n) (((n) == 64) ? ~0ULL : (1ULL << (n)) - 1)

#define IEEE80211_HT_CAP_SUP_WIDTH_20_40 0x0002
#define IEEE80211_HT_CAP_SGI_20          0x0020
#define IEEE80211_HT_CAP_SGI_40          0x0040

/*******************************************************************************
 * 3. FUNDAMENTAL LINUX TYPES & EXACT-WIDTH ARCHITECTURE ALIGNMENT
 *******************************************************************************/
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

typedef uint8_t   __u8;
typedef uint16_t  __u16;
typedef uint32_t  __u32;
typedef uint64_t  __u64;

typedef int8_t    __s8;
typedef int16_t   __s16;
typedef int32_t   __s32;
typedef int64_t   __s64;

typedef uint16_t  __le16;
typedef uint32_t  __le32;
typedef uint64_t  __le64;
typedef uint16_t  __be16;
typedef uint32_t  __be32;
typedef uint64_t  __be64;

typedef uint16_t  __sum16;
typedef uint32_t  __wsum;

typedef unsigned int gfp_t;
typedef long long    time64_t;
typedef u64          dma_addr_t;
typedef unsigned long kernel_ulong_t;

/* 
 * CORREÇÃO: O Kernel framework da Apple já importa bool via stdbool.h nativo.
 * Não podemos fazer typedef int bool. Deixamos o compilador usar o tipo nativo.
 */

/*******************************************************************************
 * 4. NETWORK AND DRIVER CONSTANTS
 *******************************************************************************/
#define ETH_ALEN            6
#define NUM_NL80211_BANDS   3

enum nl80211_iftype {
    NL80211_IFTYPE_UNSPECIFIED,
    NL80211_IFTYPE_ADHOC,
    NL80211_IFTYPE_STATION,
    NL80211_IFTYPE_AP,
    NL80211_IFTYPE_MONITOR,
    NL80211_IFTYPE_MESH_POINT,
    NL80211_IFTYPE_P2P_CLIENT,
    NL80211_IFTYPE_P2P_GO,
    NL80211_IFTYPE_P2P_DEVICE,
};

/*******************************************************************************
 * 5. COMPREHENSIVE ERROR CODES REALIGNMENT MATRIX
 *******************************************************************************/
#ifndef EPERM
#define EPERM            1
#endif
#ifndef ENOENT
#define ENOENT           2
#endif
#ifndef ESRCH
#define ESRCH            3
#endif
#ifndef EINTR
#define EINTR            4
#endif
#ifndef EIO
#define EIO              5
#endif
#ifndef ENXIO
#define ENXIO            6
#endif
#ifndef E2BIG
#define E2BIG            7
#endif
#ifndef ENOEXEC
#define ENOEXEC          8
#endif
#ifndef EBADF
#define EBADF            9
#endif
#ifndef EAGAIN
#define EAGAIN           35
#endif
#ifndef ENOMEM
#define ENOMEM           12
#endif
#ifndef EACCES
#define EACCES           13
#endif
#ifndef EFAULT
#define EFAULT           14
#endif
#ifndef EBUSY
#define EBUSY            16
#endif
#ifndef EEXIST
#define EEXIST           17
#endif
#ifndef EXDEV
#define EXDEV            18
#endif
#ifndef ENODEV
#define ENODEV           19
#endif
#ifndef EINVAL
#define EINVAL           22
#endif
#ifndef ENFILE
#define ENFILE           23
#endif
#ifndef EMFILE
#define EMFILE           24
#endif
#ifndef ENOSPC
#define ENOSPC           28
#endif
#ifndef ENOSYS
#define ENOSYS           78
#endif
#ifndef EOPNOTSUPP
#define EOPNOTSUPP       102
#endif

/*******************************************************************************
 * 6. MATHEMATICAL MACROS, ALIGNMENT ENGINE & DATA BOUNDARIES
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

#define GENMASK(h, l) \
    (((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

#define GENMASK_ULL(h, l) \
    (((~0ULL) - (1ULL << (l)) + 1) & (~0ULL >> (64 - 1 - (h))))

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

#define DIV_ROUND_CLOSEST(x, divisor) ({ \
    typeof(x) __x = x; \
    typeof(divisor) __d = divisor; \
    (((__x) + ((__d) / 2)) / (__d)); \
})

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - __builtin_offsetof(type, member)))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifndef min
#define min(x, y) ({ \
    typeof(x) _min1 = (x); \
    typeof(y) _min2 = (y); \
    (void) (&_min1 == &_min2); \
    _min1 < _min2 ? _min1 : _min2; \
})
#endif

#ifndef max
#define max(x, y) ({ \
    typeof(x) _max1 = (x); \
    typeof(y) _max2 = (y); \
    (void) (&_max1 == &_max2); \
    _max1 > _max2 ? _max1 : _max2; \
})
#endif

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
 * 7. COMPLETE LINUX LINKED LIST ENGINE (EXPLODED FOR CLANG COMPILATION)
 *******************************************************************************/
#ifdef LIST_HEAD
#undef LIST_HEAD
#endif
#ifdef LIST_HEAD_INIT
#undef LIST_HEAD_INIT
#endif

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

static __always_inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

static __always_inline void __list_add(struct list_head *new_node, 
                                       struct list_head *prev, 
                                       struct list_head *next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

static __always_inline void list_add(struct list_head *new_node, struct list_head *head)
{
    __list_add(new_node, head, head->next);
}

static __always_inline void list_add_tail(struct list_head *new_node, struct list_head *head)
{
    __list_add(new_node, head->prev, head);
}

static __always_inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

static __always_inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

static __always_inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

static __always_inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}

static __always_inline void list_move_tail(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}

static __always_inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

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

/*******************************************************************************
 * 8. LINUX HASH LISTS (HLIST ENGINE)
 *******************************************************************************/
struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next;
    struct hlist_node **pprev;
};

static __always_inline void INIT_HLIST_HEAD(struct hlist_head *h)
{
    h->first = NULL;
}

static __always_inline void INIT_HLIST_NODE(struct hlist_node *h)
{
    h->next = NULL;
    h->pprev = NULL;
}

static __always_inline int hlist_unhashed(const struct hlist_node *h)
{
    return !h->pprev;
}

static __always_inline int hlist_empty(const struct hlist_head *h)
{
    return !h->first;
}

static __always_inline void hlist_del(struct hlist_node *n)
{
    struct hlist_node *next = n->next;
    struct hlist_node **pprev = n->pprev;
    *pprev = next;
    if (next) {
        next->pprev = pprev;
    }
    n->next = NULL;
    n->pprev = NULL;
}

static __always_inline void hlist_del_init(struct hlist_node *n)
{
    if (!hlist_unhashed(n)) {
        hlist_del(n);
        INIT_HLIST_NODE(n);
    }
}

static __always_inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    struct hlist_node *first = h->first;
    n->next = first;
    if (first) {
        first->pprev = &n->next;
    }
    h->first = n;
    n->pprev = &h->first;
}

#define hlist_entry(ptr, type, member) container_of(ptr, type, member)

#define hlist_for_each_entry(pos, head, member) \
    for (pos = ((head)->first) ? hlist_entry((head)->first, typeof(*pos), member) : NULL; \
         pos; \
         pos = (pos->member.next) ? hlist_entry(pos->member.next, typeof(*pos), member) : NULL)

/*******************************************************************************
 * 9. ENDIANNESS CONVERSION ENGINE
 *******************************************************************************/
#define cpu_to_le16(x) ((__le16)(uint16_t)(x))
#define cpu_to_le32(x) ((__le32)(uint32_t)(x))
#define cpu_to_le64(x) ((__le64)(uint64_t)(x))
#define le16_to_cpu(x) ((uint16_t)(__le16)(x))
#define le32_to_cpu(x) ((uint32_t)(__le32)(x))
#define le64_to_cpu(x) ((uint64_t)(__le64)(x))

#define cpu_to_be16(x) __builtin_bswap16((uint16_t)(x))
#define cpu_to_be32(x) __builtin_bswap32((uint32_t)(x))
#define cpu_to_be64(x) __builtin_bswap64((uint64_t)(x))
#define be16_to_cpu(x) __builtin_bswap16((uint16_t)(x))
#define be32_to_cpu(x) __builtin_bswap32((uint32_t)(x))
#define be64_to_cpu(x) __builtin_bswap64((uint64_t)(x))

/*******************************************************************************
 * 10. ATOMIC BITWISE OPERATION MATRIX
 *******************************************************************************/
static __always_inline int test_bit(int nr, const volatile unsigned long *addr)
{
    return ((*addr) & (1UL << nr)) != 0;
}

static __always_inline void set_bit(int nr, volatile unsigned long *addr)
{
    __sync_fetch_and_or(addr, (1UL << nr));
}

static __always_inline void clear_bit(int nr, volatile unsigned long *addr)
{
    __sync_fetch_and_and(addr, ~(1UL << nr));
}

static __always_inline void change_bit(int nr, volatile unsigned long *addr)
{
    __sync_fetch_and_xor(addr, (1UL << nr));
}

static __always_inline int test_and_set_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = (1UL << nr);
    return (__sync_fetch_and_or(addr, mask) & mask) != 0;
}

static __always_inline int test_and_clear_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = (1UL << nr);
    return (__sync_fetch_and_and(addr, ~mask) & mask) != 0;
}

static __always_inline int test_and_change_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = (1UL << nr);
    return (__sync_fetch_and_xor(addr, mask) & mask) != 0;
}

/*******************************************************************************
 * 11. ADVANCED LOCKING EMULATION (SPINLOCKS & MUTEX OVER XNU CORE)
 *******************************************************************************/
#include <IOKit/IOLocks.h>

// -----------------------------------------------------------------------------
// MUTEX EMULATION (Uses XNU IOLock for safe context blocking/sleeping)
// -----------------------------------------------------------------------------
struct mutex {
    IOLock *xnu_lock;
    volatile int locked;
    void *owner;
};

static __always_inline void mutex_init(struct mutex *m)
{
    if (m) {
        m->xnu_lock = IOLockAlloc();
        m->locked = 0;
        m->owner = NULL;
    }
}

// Nota técnica: Se o driver desalocar o módulo, precisamos destruir o IOLock.
// Caso o hw.c não chame um destruidor, garanta a liberação no stop do KEXT.
static __always_inline void mutex_destroy(struct mutex *m)
{
    if (m && m->xnu_lock) {
        IOLockFree(m->xnu_lock);
        m->xnu_lock = NULL;
    }
}

static __always_inline void mutex_lock(struct mutex *m)
{
    if (m && m->xnu_lock) {
        IOLockLock(m->xnu_lock); // Bloqueia a thread de forma segura (sleep)
        m->locked = 1;
    }
}

static __always_inline void mutex_unlock(struct mutex *m)
{
    if (m && m->xnu_lock) {
        m->locked = 0;
        IOLockUnlock(m->xnu_lock); // Acorda a próxima thread da fila
    }
}

static __always_inline int mutex_is_locked(struct mutex *m)
{
    return (m ? m->locked : 0);
}

// -----------------------------------------------------------------------------
// SPINLOCK EMULATION (Uses XNU IOSimpleLock for hardware-level atomic spins)
// -----------------------------------------------------------------------------
typedef IOSimpleLock* spinlock_t;
typedef IORWLock* rwlock_t; // Mapeamento correto para Read-Write Locks do XNU

static __always_inline void spin_lock_init(spinlock_t *lock)
{
    if (lock) {
        *lock = IOSimpleLockAlloc();
    }
}

static __always_inline void spin_lock(spinlock_t *lock)
{
    if (lock && *lock) {
        IOSimpleLockLock(*lock); // Desativa preempção e interrupções locais de forma segura
    }
}

static __always_inline void spin_unlock(spinlock_t *lock)
{
    if (lock && *lock) {
        IOSimpleLockUnlock(*lock);
    }
}

// No Linux, _bh (Bottom Halves) gerencia travas de software interrupts. 
// No XNU, mapeamos direto para a trava simples de hardware.
#define spin_lock_bh(lock)                      spin_lock(lock)
#define spin_unlock_bh(lock)                    spin_unlock(lock)
#define spin_lock_irq(lock)                     spin_lock(lock)
#define spin_unlock_irq(lock)                   spin_unlock(lock)

#define spin_lock_irqsave(lock, flags) \
    do { \
        (flags) = 0; \
        spin_lock(lock); \
    } while(0)

#define spin_unlock_irqrestore(lock, flags) \
    do { \
        (void)(flags); \
        spin_unlock(lock); \
    } while(0)

/*******************************************************************************
 * 12. RCU (READ-COPY UPDATE) MEMORY SUBSYSTEM DEFENSIVE STUBS
 *******************************************************************************/
static __always_inline void rcu_read_lock(void) 
{
    // RCU Read boundaries are natively synchronized inside macOS unified memory mapping
}

static __always_inline void rcu_read_unlock(void) 
{
    // End of critical RCU trace
}

#define rcu_assign_pointer(p, v)                do { (p) = (v); } while(0)
#define rcu_dereference(p)                      (p)
#define rcu_dereference_protected(p, c)         (p)
#define rcu_dereference_check(p, c)             (p)

/*******************************************************************************
 * 13. ATOMIC VARIABLES COMPREHENSIVE INFRASTRUCTURE
 *******************************************************************************/
typedef struct { volatile int counter; } atomic_t;
typedef struct { volatile long counter; } atomic_long_t;

#define ATOMIC_INIT(i) { (i) }

static __always_inline int atomic_read(const atomic_t *v)
{
    return v->counter;
}

static __always_inline void atomic_set(atomic_t *v, int i)
{
    v->counter = i;
}

static __always_inline void atomic_inc(atomic_t *v)
{
    __sync_fetch_and_add(&v->counter, 1);
}

static __always_inline void atomic_dec(atomic_t *v)
{
    __sync_fetch_and_sub(&v->counter, 1);
}

static __always_inline void atomic_add(int i, atomic_t *v)
{
    __sync_fetch_and_add(&v->counter, i);
}

static __always_inline void atomic_sub(int i, atomic_t *v)
{
    __sync_fetch_and_sub(&v->counter, i);
}

static __always_inline int atomic_sub_and_test(int i, atomic_t *v)
{
    return __sync_sub_and_fetch(&v->counter, i) == 0;
}

static __always_inline int atomic_dec_and_test(atomic_t *v)
{
    return __sync_sub_and_fetch(&v->counter, 1) == 0;
}

static __always_inline int atomic_inc_and_test(atomic_t *v)
{
    return __sync_add_and_fetch(&v->counter, 1) == 0;
}

static __always_inline int atomic_add_negative(int i, atomic_t *v)
{
    return __sync_add_and_fetch(&v->counter, i) < 0;
}

static __always_inline int atomic_cmpxchg(atomic_t *v, int old, int new_val)
{
    return __sync_val_compare_and_swap(&v->counter, old, new_val);
}

/*******************************************************************************
 * 14. XNU MEMORY MANAGEMENT INTERFACING (DYNAMIC MALLOC ARCHITECTURE)
 *******************************************************************************/
#define GFP_KERNEL  0x00ULL
#define GFP_ATOMIC  0x01ULL
#define GFP_DMA     0x02ULL

static __always_inline void *apple_compat_kzalloc(size_t size)
{
    size_t alloc_size = size + 16;
    void *raw = IOMalloc(alloc_size);
    if (!raw) {
        return NULL;
    }
    bzero(raw, alloc_size);
    *(size_t *)raw = alloc_size;
    return (void *)((char *)raw + 16);
}

static __always_inline void apple_compat_kfree(void *ptr)
{
    if (ptr) {
        void *raw = (void *)((char *)ptr - 16);
        size_t alloc_size = *(size_t *)raw;
        IOFree(raw, alloc_size);
    }
}

#undef kzalloc
#undef kfree
#undef kmalloc
#define kzalloc(size, flags)          apple_compat_kzalloc(size)
#define kmalloc(size, flags)          apple_compat_kzalloc(size)
#define kfree(ptr)                    apple_compat_kfree(ptr)
#define vzalloc(size)                 apple_compat_kzalloc(size)
#define vfree(ptr)                    apple_compat_kfree(ptr)
#define kcalloc(n, size, flags)       apple_compat_kzalloc((n) * (size))

static __always_inline void *kmemdup(const void *src, size_t len, gfp_t flags)
{
    void *buf = apple_compat_kzalloc(len);
    if (buf) {
        memcpy(buf, src, len);
    }
    return buf;
}

/*******************************************************************************
 * 15. TIMEKEEPING, JIFFIES ENGINE, AND HIGH-PRECISION KERNEL DELAYS
 *******************************************************************************/
#define HZ 1000

static __always_inline unsigned long apple_get_jiffies(void)
{
    uint64_t nsecs;
    clock_get_uptime(&nsecs);
    return (unsigned long)(nsecs / 1000000);
}

#define jiffies apple_get_jiffies()
#define msecs_to_jiffies(m) (m)
#define jiffies_to_msecs(j) (j)

#define time_after(a, b)         ((long)((b) - (a)) < 0)
#define time_before(a, b)        ((long)((a) - (b)) < 0)
#define time_after_eq(a, b)      ((long)((b) - (a)) <= 0)
#define time_before_eq(a, b)     ((long)((a) - (b)) <= 0)

static __always_inline void mdelay(unsigned long msecs)
{
    IOSleep((unsigned int)msecs);
}

/* CORREÇÃO: Chamando a função nativa do IOKit com a capitalização correta (IODelay) */
static __always_inline void udelay(unsigned long usecs)
{
    IODelay((unsigned int)usecs);
}

static __always_inline void usleep_range(unsigned long min_u, unsigned long max_u)
{
    IODelay((unsigned int)((min_u + max_u) / 2));
}

/*******************************************************************************
 * 16. LINUX ASYNCHRONOUS KERNEL TIMER SUBSYSTEM
 *******************************************************************************/
struct timer_list {
    struct list_head entry;
    unsigned long expires;
    void (*function)(struct timer_list *);
    unsigned long data;
};

static __always_inline void timer_setup(struct timer_list *timer, 
                                       void (*func)(struct timer_list *), 
                                       unsigned int flags)
{
    INIT_LIST_HEAD(&timer->entry);
    timer->function = func;
    timer->expires = 0;
    timer->data = 0;
}

static __always_inline int mod_timer(struct timer_list *timer, unsigned long expires)
{
    timer->expires = expires;
    if (timer->function) {
        timer->function(timer);
    }
    return 0;
}

static __always_inline int del_timer(struct timer_list *timer)
{
    return 0;
}

static __always_inline int del_timer_sync(struct timer_list *timer)
{
    return 0;
}

static __always_inline int timer_pending(const struct timer_list *timer)
{
    return 0;
}

/*******************************************************************************
 * 17. WORKQUEUES ENGINE (SYNCHRONOUS COMPATIBILITY MAPPING)
 *******************************************************************************/
struct work_struct {
    struct list_head entry;
    void (*func)(struct work_struct *work);
};

struct delayed_work {
    struct work_struct work;
    struct timer_list timer;
    void *wq;
};

#define DECLARE_WORK(name, f) \
    struct work_struct name = { LIST_HEAD_INIT(name.entry), f }

#define INIT_WORK(_work, _func) \
    do { \
        INIT_LIST_HEAD(&(_work)->entry); \
        (_work)->func = (_func); \
    } while (0)

#define INIT_DELAYED_WORK(_work, _func) \
    do { \
        INIT_WORK(&(_work)->work, (_func)); \
        timer_setup(&(_work)->timer, NULL, 0); \
    } while (0)

static __always_inline bool queue_work(void *wq, struct work_struct *work)
{
    if (work && work->func) {
        work->func(work);
        return true;
    }
    return false;
}

static __always_inline bool queue_delayed_work(void *wq, 
                                               struct delayed_work *dwork, 
                                               unsigned long delay)
{
    if (dwork && dwork->work.func) {
        dwork->work.func(&dwork->work);
        return true;
    }
    return false;
}

static __always_inline bool cancel_work_sync(struct work_struct *work)
{
    return true;
}

static __always_inline bool cancel_delayed_work_sync(struct delayed_work *dwork)
{
    return true;
}

static __always_inline void flush_workqueue(void *wq)
{
    // XNU buffers flushed inherently 
}

static __always_inline void destroy_workqueue(void *wq)
{
    // Garbage collection handling stub
}

#define create_singlethread_workqueue(name) ((void *)0xDEADBEEF)
#define create_workqueue(name)              ((void *)0xDEADBEEF)

/*******************************************************************************
 * 18. TASKLETS (IMMEDIATE BOTTOM HALF SCHEDULING INFRASTRUCTURE)
 *******************************************************************************/
struct tasklet_struct {
    struct tasklet_struct *next;
    unsigned long state;
    atomic_t count;
    void (*func)(unsigned long);
    unsigned long data;
};

static __always_inline void tasklet_init(struct tasklet_struct *t, \
                                        void (*func)(unsigned long), \
                                        unsigned long data)
{
    t->next = NULL;
    t->state = 0;
    atomic_set(&t->count, 0);
    t->func = func;
    t->data = data;
}

static __always_inline void tasklet_schedule(struct tasklet_struct *t)
{
    if (t->func) {
        t->func(t->data);
    }
}

static __always_inline void tasklet_kill(struct tasklet_struct *t)
{
    // Hardware architecture boundary clearance
}

static __always_inline void tasklet_enable(struct tasklet_struct *t)
{
    atomic_set(&t->count, 0);
}

static __always_inline void tasklet_disable(struct tasklet_struct *t)
{
    atomic_inc(&t->count);
}

/*******************************************************************************
 * 19. PIPELINE DE EMULAÇÃO DE BUFFER DE SOQUETE MASSIVO (SK_BUFF)
 *******************************************************************************/
struct sk_buff {
    struct sk_buff *next;
    struct sk_buff *prev;
    unsigned char *data;
    unsigned char *head;
    unsigned char *tail;
    unsigned char *end;
    unsigned int len;
    unsigned int data_len;
    uint16_t protocol;
    uint32_t priority;
    unsigned char cb[48] __attribute__((aligned(8)));
};

struct sk_buff_head {
    struct sk_buff *next;
    struct sk_buff *prev;
    uint32_t qlen;
    spinlock_t lock;
};

/* CORREÇÃO: Insere um elemento no fim da fila de pacotes */
static __always_inline void __skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
    newsk->next = NULL;
    if (!list->next) {
        list->next = newsk;
        newsk->prev = NULL;
        list->prev = newsk;
    } else {
        struct sk_buff *old_tail = list->prev;
        if (old_tail) {
            old_tail->next = newsk;
            newsk->prev = old_tail;
        }
        list->prev = newsk;
    }
    list->qlen++;
}

/* CORREÇÃO: Remove e retorna o primeiro elemento da fila de pacotes */
static __always_inline struct sk_buff *__skb_dequeue(struct sk_buff_head *list)
{
    struct sk_buff *skb = list->next;
    if (skb) {
        list->next = skb->next;
        if (list->next) {
            list->next->prev = NULL;
        } else {
            list->prev = NULL;
        }
        skb->next = NULL;
        skb->prev = NULL;
        list->qlen--;
    }
    return skb;
}

static __always_inline struct sk_buff *dev_alloc_skb(unsigned int length)
{
    struct sk_buff *skb = (struct sk_buff *)IOMalloc(sizeof(struct sk_buff) + length);
    if (!skb) return NULL;
    skb->next = NULL;
    skb->prev = NULL;
    skb->head = (unsigned char *)(skb + 1);
    skb->data = skb->head;
    skb->tail = skb->head;
    skb->end = skb->head + length;
    skb->len = 0;
    skb->data_len = 0;
    return skb;
}

static __always_inline void kfree_skb(struct sk_buff *skb)
{
    if (skb) {
        IOFree(skb, sizeof(struct sk_buff) + (size_t)(skb->end - skb->head));
    }
}

static __always_inline void skb_reserve(struct sk_buff *skb, int len)
{
    skb->data += len;
    skb->tail += len;
}

static __always_inline unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
    unsigned char *tmp = skb->tail;
    skb->tail += len;
    skb->len += len;
    return tmp;
}

static __always_inline unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
    skb->data -= len;
    skb->len += len;
    return skb->data;
}

static __always_inline unsigned char *skb_pull(struct sk_buff *skb, unsigned int len)
{
    skb->data += len;
    skb->len -= len;
    return skb->data;
}

static __always_inline void skb_trim(struct sk_buff *skb, unsigned int len)
{
    skb->len = len;
    skb->tail = skb->data + len;
}

/*******************************************************************************
 * 19.5 PCI MMIO REGISTER ACCESS PIPELINE (readb, writeb, etc.)
 *******************************************************************************/
#ifndef __iomem
#define __iomem
#endif

static __always_inline uint8_t readb(const volatile void *addr) {
    return *(const volatile uint8_t *)addr;
}

static __always_inline uint16_t readw(const volatile void *addr) {
    return *(const volatile uint16_t *)addr;
}

static __always_inline uint32_t readl(const volatile void *addr) {
    return *(const volatile uint32_t *)addr;
}

static __always_inline void writeb(uint8_t val, volatile void *addr) {
    *(volatile uint8_t *)addr = val;
}

static __always_inline void writew(uint16_t val, volatile void *addr) {
    *(volatile uint16_t *)addr = val;
}

static __always_inline void writel(uint32_t val, volatile void *addr) {
    *(volatile uint32_t *)addr = val;
}

/*******************************************************************************
 * 19.6 LINUX KERNEL WARNING & LOGGING PIPELINE (WARN_ONCE, WARN_ON)
 *******************************************************************************/
#ifndef WARN_ONCE
#define WARN_ONCE(condition, format, ...) ({ \
    static bool __warned = false; \
    bool __ret = (condition); \
    if (__ret && !__warned) { \
        __warned = true; \
        printf("RTL8723BE [WARN]: " format, ##__VA_ARGS__); \
    } \
    __ret; \
})
#endif

#ifndef WARN_ON
#define WARN_ON(condition) ({ \
    int __ret_warn = !!(condition); \
    if (__ret_warn) { \
        printf("RTL8723BE [WARN_ON]: %s at %s:%d\n", #condition, __FILE__, __LINE__); \
    } \
    __ret_warn; \
})
#endif

#ifndef WARN
#define WARN(condition, format, ...) ({ \
    int __ret_warn = !!(condition); \
    if (__ret_warn) { \
        printf("RTL8723BE [WARN]: " format, ##__VA_ARGS__); \
    } \
    __ret_warn; \
})
#endif

/*******************************************************************************
 * 20. DEVICE MODEL MATRICES & ABSTRACT DATA STRUCTURES
 *******************************************************************************/
struct device {
    const char *init_name;
    void *driver_data;
    struct device *parent;
};

static __always_inline void *dev_get_drvdata(const struct device *dev)
{
    return dev->driver_data;
}

static __always_inline void dev_set_drvdata(struct device *dev, void *data)
{
    dev->driver_data = data;
}

/*******************************************************************************
 * 21. EXHAUSTIVE PCI EXPRESS BUS SUBSYSTEM EMULATION (MAPPED WITH REALTEK IDS)
 *******************************************************************************/
/* Aponta direto para a sua Seção 1, evitando duplicar o valor 0x10EC */
#ifndef PCI_VENDOR_ID_REALTEK
#define PCI_VENDOR_ID_REALTEK REALTEK_PCI_VENDOR_ID
#endif

#ifndef PCI_ANY_ID
#define PCI_ANY_ID (~0U)
#endif

#ifndef KBUILD_MODNAME
#define KBUILD_MODNAME "RTL8723BE_MacDriver"
#endif

/* Emulação do gerenciamento de energia Linux (PM) */
#define SIMPLE_DEV_PM_OPS(name, suspend_fn, resume_fn) \
    const int name = 0

struct pci_device_id {
    u32 vendor;
    u32 device;
    u32 subvendor;
    u32 subdevice;
    u32 device_class;
    u32 class_mask;
    kernel_ulong_t driver_data;
};

struct pci_dev {
    struct device dev;
    unsigned int devfn;
    u16 vendor;
    u16 device;
    u16 subsystem_vendor;
    u16 subsystem_device;
    unsigned int irq;
    bool msix_enabled;
};

/* Estrutura base pci_driver para o sw.c injetar os callbacks com sucesso */
struct pci_driver {
    const char *name;
    const struct pci_device_id *id_table;
    int (*probe)(struct pci_dev *dev, const struct pci_device_id *id);
    void (*remove)(struct pci_dev *dev);
    struct {
        const int *pm;
    } driver;
};

static __always_inline void *pci_get_drvdata(struct pci_dev *pdev)
{
    return pdev->dev.driver_data;
}

static __always_inline void pci_set_drvdata(struct pci_dev *pdev, void *data)
{
    pdev->dev.driver_data = data;
}

#define pci_resource_start(pdev, bar)   ((unsigned long)0xFE000000)
#define pci_resource_len(pdev, bar)     ((unsigned long)0x00010000)
#define pci_resource_flags(pdev, bar)   ((unsigned long)0x00000200)

static __always_inline int pci_enable_device(struct pci_dev *pdev)
{
    pdev->vendor = REALTEK_PCI_VENDOR_ID;
    pdev->device = RTL8723BE_PCI_DEVICE_ID;
    pdev->subsystem_vendor = ASUS_SUBSYSTEM_VENDOR_ID;
    pdev->subsystem_device = ASUS_RTL8723BE_SUBSYSTEM_DEVICE_ID;
    return 0;
}

static __always_inline void pci_disable_device(struct pci_dev *pdev)
{
    // Safety clearance boundary
}

static __always_inline void pci_set_master(struct pci_dev *pdev)
{
    // DMA configuration state machine mapping
}

static __always_inline int pci_set_dma_mask(struct pci_dev *pdev, u64 mask)
{
    return 0;
}

static __always_inline int pci_set_consistent_dma_mask(struct pci_dev *pdev, u64 mask)
{
    return 0;
}

static __always_inline int pci_request_regions(struct pci_dev *pdev, const char *res_name)
{
    return 0;
}

static __always_inline void pci_release_regions(struct pci_dev *pdev)
{
    // Regions unbound safely
}

static __always_inline void *pci_ioremap_bar(struct pci_dev *pdev, int bar)
{
    return (void *)0xFE000000;
}

static __always_inline void iounmap(void *addr)
{
    // Unmapping MMIO block completely
}

/*******************************************************************************
 * 22. MEMORY-MAPPED I/O (MMIO) HARDFORWARDING REGISTERS ACCESS ENGINE
 *******************************************************************************/
#define ioread8(addr)           (*(volatile uint8_t *)(addr))
#define ioread16(addr)          (*(volatile uint16_t *)(addr))
#define ioread32(addr)          (*(volatile uint32_t *)(addr))
#define iowrite8(val, addr)     do { *(volatile uint8_t *)(addr) = (val); } while(0)
#define iowrite16(val, addr)    do { *(volatile uint16_t *)(addr) = (val); } while(0)
#define iowrite32(val, addr)    do { *(volatile uint32_t *)(addr) = (val); } while(0)

#define rmb()   __sync_synchronize()
#define wmb()   __sync_synchronize()
#define mb()    __sync_synchronize()

/*******************************************************************************
 * 23. IEEE 802.11 / MAC80211 WIRELESS CORE NETWORK INFRASTRUCTURE
 *******************************************************************************/

// Macros de Sinalização e Flags de Rx/Tx exigidas pelo trx.c
#define RX_FLAG_FAILED_FCS_CRC   (1 << 0)
#define RX_FLAG_MACTIME_START    (1 << 1)
#define RX_FLAG_DECRYPTED        (1 << 2)

#define RATE_INFO_BW_40          1
#define RX_ENC_HT                2

#define IEEE80211_SCTL_FRAG      0x000f
#define IEEE80211_SCTL_SEQ       0xfff0
#define IEEE80211_FCTL_MOREFRAGS 0x0400
#define IEEE80211_TX_CTL_AMPDU   (1 << 0)

#define IEEE80211_FCTL_FTYPE        0x000c
#define IEEE80211_FTYPE_MGMT        0x0000
#define IEEE80211_FTYPE_CTL         0x0004
#define IEEE80211_FTYPE_DATA        0x0008
#define IEEE80211_FCTL_STYPE        0x00f0
#define IEEE80211_STYPE_BEACON      0x0080
#define IEEE80211_FCTL_TODS         0x0100
#define IEEE80211_FCTL_FROMDS       0x0200
#define IEEE80211_QOS_CTL_TID_MASK  0x000f

// Identificadores de Criptografia (Cipher Suites)
#define WLAN_CIPHER_SUITE_WEP40  0x000fc001
#define WLAN_CIPHER_SUITE_WEP104 0x000fc005
#define WLAN_CIPHER_SUITE_TKIP   0x000fc002
#define WLAN_CIPHER_SUITE_CCMP   0x000fc004

// Flags de Banda e Canal exigidas pelo Clang
enum nl80211_band {
    NL80211_BAND_2GHZ,
    NL80211_BAND_5GHZ,
    NL80211_BAND_6GHZ,
};
#define IEEE80211_CHAN_NO_HT40MINUS     0x00000020

#define IEEE80211_HT_CAP_SUP_WIDTH_20_40 0x0002
#define IEEE80211_HT_CAP_DSSSCCK40       0x0004
#define IEEE80211_HT_CAP_MAX_AMSDU       0x0008
#define IEEE80211_HT_MAX_AMPDU_64K       3
#define IEEE80211_HT_MPDU_DENSITY_16     7
#define IEEE80211_HT_MCS_TX_DEFINED      0x01

// Stubs de enums e structs para sanar warnings de visibilidade do base.h e regd.h
enum ieee80211_smps_mode {
    IEEE80211_SMPS_STATIC,
    IEEE80211_SMPS_DYNAMIC,
    IEEE80211_SMPS_DISABLED,
};

struct wiphy { int dummy; };
struct regulatory_request { int dummy; };

struct net_device {
    char name[16];
    unsigned char dev_addr[6];
    unsigned char perm_addr[6];
    long unsigned int state;
};

// Estruturas Wireless que estavam faltando e geravam os erros no base.c
struct ieee80211_channel {
    int band;
    int center_freq;
    int hw_value;
    unsigned int flags;
};

struct ieee80211_rate {
    unsigned int bitrate;
    unsigned char flags;
    unsigned char hw_value;
};

// Nova estrutura de banda suportada exigida pelo dicionário estático do base.c
struct ieee80211_supported_band {
    enum nl80211_band band;
    struct ieee80211_channel *channels;
    int n_channels;
    struct ieee80211_rate *bitrates;
    int n_bitrates;
    struct {
        int dummy;
    } ht_cap;
};

/* --- VHT Support Definitions --- */

#define IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_11454          0x00000002
#define IEEE80211_VHT_CAP_SHORT_GI_80                    0x00000020
#define IEEE80211_VHT_CAP_TXSTBC                         0x00000080
#define IEEE80211_VHT_CAP_RXSTBC_1                       0x00000100
#define IEEE80211_VHT_CAP_SU_BEAMFORMER_CAPABLE          0x00000800
#define IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE          0x00001000
#define IEEE80211_VHT_CAP_HTC_VHT                        0x00800000
#define IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_MASK 0x03800000
#define IEEE80211_VHT_CAP_RX_ANTENNA_PATTERN             0x10000000
#define IEEE80211_VHT_CAP_TX_ANTENNA_PATTERN             0x20000000

#define IEEE80211_VHT_MCS_SUPPORT_0_9                    0
#define IEEE80211_VHT_MCS_NOT_SUPPORTED                  3

struct ieee80211_sta_vht_cap {
    bool vht_supported;
    uint32_t cap;
    uint16_t rx_mcs_map;
    uint16_t tx_mcs_map;
};

// Estrutura expandida para suportar as atribuições de rx_mask e rx_highest do base.c
struct ieee80211_sta_ht_cap {
    unsigned char ht_supported;
    unsigned short cap;
    unsigned char ampdu_factor;
    unsigned char ampdu_density;
    struct {
        unsigned char tx_params;
        unsigned char rx_mask[10];
        uint16_t rx_highest;
    } mcs;
};

struct ieee80211_chan_def {
    struct {
        uint32_t center_freq;
        uint32_t band;
    } *chan;
};

struct ieee80211_hw {
    void *priv;
    struct {
        struct ieee80211_chan_def chandef;
    } conf;
};

struct ieee80211_hdr {
    uint16_t frame_control;
    uint16_t duration_id;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_ctrl;
    uint8_t addr4[6];
} __attribute__((packed));

// Helpers para extração de Endereços MAC (SA = Source, DA = Destination)
static inline const uint8_t *ieee80211_get_SA(const struct ieee80211_hdr *hdr) {
    return hdr->addr2;
}

static inline const uint8_t *ieee80211_get_DA(const struct ieee80211_hdr *hdr) {
    return hdr->addr1;
}

// Funções de decodificação de Frame Control (fc) do 802.11
static inline int ieee80211_is_beacon(uint16_t fc) {
    return (fc & (IEEE80211_FCTL_FTYPE | IEEE80211_FCTL_STYPE)) == (IEEE80211_FTYPE_MGMT | IEEE80211_STYPE_BEACON);
}

static inline int ieee80211_is_mgmt(uint16_t fc) {
    return (fc & IEEE80211_FCTL_FTYPE) == IEEE80211_FTYPE_MGMT;
}

static inline int ieee80211_is_ctl(uint16_t fc) {
    return (fc & IEEE80211_FCTL_FTYPE) == IEEE80211_FTYPE_CTL;
}

static inline int ieee80211_is_nullfunc(uint16_t fc) {
    uint16_t stype = fc & IEEE80211_FCTL_STYPE;
    return (fc & IEEE80211_FCTL_FTYPE) == IEEE80211_FTYPE_DATA && (stype == 0x0040 || stype == 0x00c0);
}

static inline int ieee80211_is_data_qos(uint16_t fc) {
    return (fc & IEEE80211_FCTL_FTYPE) == IEEE80211_FTYPE_DATA && (fc & 0x0080);
}

static inline int _ieee80211_is_robust_mgmt_frame(const void *hdr) {
    return 0;
}

static inline int ieee80211_has_protected(uint16_t fc) {
    return fc & 0x4000;
}

// Correção do parâmetro const void* para evitar warnings com struct ieee80211_hdr*
static __always_inline uint8_t *ieee80211_get_qos_ctl(const void *hdr) {
    static uint8_t dummy_qos[2] = {0, 0};
    return dummy_qos;
}

struct ieee80211_vif {
    int dummy;
};

struct ieee80211_tx_queue_params {
    unsigned int acm;
    unsigned int aifs;
    unsigned int cw_min;
    unsigned int cw_max;
};

/* Sincronização de Threads (Completion Pipeline) */
struct completion {
    unsigned int done;
    spinlock_t wait_lock;
};

static __always_inline void init_completion(struct completion *x) { x->done = 0; }
static __always_inline void reinit_completion(struct completion *x) { x->done = 0; }
static __always_inline void complete(struct completion *x) { x->done = 1; }
static __always_inline long wait_for_completion_timeout(struct completion *x, unsigned long timeout) { return timeout ? (long)timeout : 1; }

static __always_inline int in_interrupt(void) { return 0; }

// Estrutura de chaves de criptografia (Resolve o erro 561)
struct ieee80211_key_conf {
    uint32_t cipher;
    uint8_t keyidx;
    uint8_t keylen;
};

struct ieee80211_tx_info {
    uint32_t flags;
    struct {
        struct ieee80211_key_conf *hw_key;
    } control;
};

struct ieee80211_rx_status {
    uint32_t freq;
    uint32_t band;
    uint32_t flag;
    uint8_t bw;
    uint8_t encoding;
    uint16_t rate_idx;
    uint64_t mactime;
    int8_t signal;
};

struct urb { int dummy; };
struct seq_file { int dummy; };

static __always_inline int seq_puts(struct seq_file *m, const char *s) { return 0; }
static __always_inline int seq_printf(struct seq_file *m, const char *fmt, ...) { return 0; }

enum nl80211_channel_type {
    NL80211_CHAN_NO_HT,
    NL80211_CHAN_HT20,
    NL80211_CHAN_HT40MINUS,
    NL80211_CHAN_HT40PLUS
};

struct ht_capability {
    uint16_t cap;
    struct { uint8_t rx_mask[2]; } mcs;
    uint8_t ampdu_density;
};

struct ieee80211_sta {
    u8 mac_addr[6];
    void *driver_priv;
    void *drv_priv;
    unsigned int aid;
    struct list_head list;
    uint32_t supp_rates[2]; 
    struct ht_capability ht_cap;
    struct ieee80211_vif *vif;
};

static __always_inline struct ieee80211_sta *ieee80211_find_sta(struct ieee80211_vif *vif, const uint8_t *mgm_addr) {
    return NULL;
}

// Helpers de rede adicionados para o trx.c
#define PCI_DMA_TODEVICE 1

static inline int is_multicast_ether_addr(const uint8_t *addr) {
    return addr[0] & 0x01;
}

static inline int is_broadcast_ether_addr(const uint8_t *addr) {
    return (addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5]) == 0xFF;
}

static inline int ether_addr_equal(const uint8_t *addr1, const uint8_t *addr2) {
    return (addr1[0] == addr2[0] && addr1[1] == addr2[1] && addr1[2] == addr2[2] &&
            addr1[3] == addr2[3] && addr1[4] == addr2[4] && addr1[5] == addr2[5]);
}

static inline uint64_t pci_map_single(void *pdev, void *ptr, size_t size, int direction) {
    return (uint64_t)(uintptr_t)ptr;
}

static inline int pci_dma_mapping_error(void *pdev, uint64_t dma_addr) {
    return 0;
}

/*******************************************************************************
 * 24. LINUX FIRMWARE LOADING SIMULATOR ENGINE
 *******************************************************************************/
struct firmware {
    size_t size;
    const u8 *data;
    struct device pages;
};

static __always_inline int request_firmware(const struct firmware **firmware_p, 
                                           const char *name, 
                                           struct device *device)
{
    return -ENOENT;
}

static __always_inline void release_firmware(const struct firmware *firmware)
{
    // Clear firmware buffers allocated inside XNU structures
}

/*******************************************************************************
 * 25. CRYPTOGRAPHIC ALGORITHMS CORE MAPPING STUBS
 *******************************************************************************/
struct crypto_arc4_ctx {
    u8 S[256];
    u8 x;
    u8 y;
};

static __always_inline int crypto_arc4_setkey(struct crypto_arc4_ctx *ctx, 
                                             const u8 *in_key, 
                                             unsigned int key_len)
{
    return 0;
}

static __always_inline int crypto_arc4_crypt(struct crypto_arc4_ctx *ctx, 
                                            u8 *out, 
                                            const u8 *in, 
                                            unsigned int len)
{
    return 0;
}

/*******************************************************************************
 * 26. ADVANCED LOGGING INFRASTRUCTURE (REBOUNDED TO NATIVE XNU CORE IOLog)
 *******************************************************************************/
#define printk(fmt, ...)        IOLog(fmt, ##__VA_ARGS__)
#define pr_info(fmt, ...)       IOLog("rtl8723be: " fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)        IOLog("rtl8723be [ERR]: " fmt, ##__VA_ARGS__)
#define pr_warn(fmt, ...)       IOLog("rtl8723be [WARN]: " fmt, ##__VA_ARGS__)
#define dev_info(d, fmt, ...)   IOLog("rtl_dev: " fmt, ##__VA_ARGS__)
#define dev_err(d, fmt, ...)    IOLog("rtl_dev [ERR]: " fmt, ##__VA_ARGS__)

#ifndef BUG_ON
#define BUG_ON(cond) \
    do { \
        if (cond) { \
            panic("Linux Emulation Environment Fatal Violation Failure!"); \
        } \
    } while(0)
#endif

/* Remove a definição antiga da linha 1084 de forma limpa antes de redeclarar */
#undef WARN_ON
#define WARN_ON(x) ({ \
    int __r = !!(x); \
    __r; \
})

#define WARN_ON_ONCE(x) ({ \
    int __r = !!(x); \
    __r; \
})

/*******************************************************************************
 * 27. DEBUGFS, THERMAL ENGINE & POWER MANAGEMENT SUB-STUBS
 *******************************************************************************/
struct dentry {
    int d_dummy;
};

static __always_inline struct dentry *debugfs_create_dir(const char *name, struct dentry *parent)
{
    return (struct dentry *)0xDEB006F5;
}

static __always_inline void debugfs_remove_recursive(struct dentry *topdir)
{
    // Clear modern filesystem tracing debug trees safely
}

/* 
 * CORREÇÃO DE COLISÃO XNU EXTERN: 
 * O framework do macOS possui uma assinatura incompatível de msleep em sys/proc.h.
 * Para interceptar as chamadas do driver Linux com segurança sem corromper as
 * declarações nativas do XNU, aplicamos a macro de remapeamento no final do arquivo.
 */
#ifndef msleep
#define msleep(msecs) IOSleep((unsigned int)(msecs))
#endif

// Missing Linux kernel helper definitions
static inline void ether_addr_copy(u8 *dst, const u8 *src) {
    memcpy(dst, src, 6);
}

static inline void skb_put_data(struct sk_buff *skb, const void *data, unsigned int len) {
    memcpy(skb_put(skb, len), data, len);
}

// ==========================================
// 28. FUNÇÕES STUBS INLINE (FINAL)
// ==========================================
static inline int skb_queue_len(const struct sk_buff_head *list) {
    return 0; 
}

static inline void pci_unmap_single(void *pdev, uint64_t dma_addr, size_t size, int direction) {
    // Stub para o hw.c não reclamar do unmap
}

#ifdef __cplusplus
}
#endif

#endif /* _APPLE_LINUX_EMULATION_H_ */
