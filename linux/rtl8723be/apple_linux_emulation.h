#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h> // O coração do Kernel do macOS (substitui o linux/kernel.h)

// === 1. DICIONÁRIO DE TIPOS: Traduzindo o "idioma" do Linux para o Mac ===
typedef unsigned char       u8;
typedef signed char         s8;
typedef unsigned short      u16;
typedef signed short        s16;
typedef unsigned int        u32;
typedef signed int          s32;
typedef unsigned long long  u64;
typedef signed long long    s64;

// Tipos de Endianness (Mapeados direto já que o Mac x86_64 é Little-Endian)
typedef u16 __le16;
typedef u32 __le32;
typedef u64 __le64;
typedef u16 __be16; 
typedef u32 __be32;
typedef u64 __be64;

// === 2. FERRAMENTAS DE REDE E COMPILADOR DO LINUX ===
#define __packed __attribute__((packed)) // Corrige os erros de redefinição estrutural
#define BIT(x) (1ULL << (x))             // Define a macro de manipulação de bits
#define ETH_ALEN 6                       // Tamanho do endereço MAC (6 bytes)
#define NUM_NL80211_BANDS 3              // Quantidade de bandas de frequência Wi-Fi

// === 3. ESTRUTURAS DE CONTROLE ESSENCIAIS ===
// Emula o sistema de listas encadeadas duplas do Linux
struct list_head {
    struct list_head *next, *prev;
};

// Emula a estrutura de trava (mutex) do Linux para compatibilidade de layout
struct mutex {
    void* owner;
};

// === 4. EMULADOR DE VERSÃO DO KERNEL DO LINUX ===
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define LINUX_VERSION_CODE KERNEL_VERSION(4, 19, 0) 

// === 5. TIPOS E MACROS ADICIONAIS ===
#define THIS_MODULE NULL
#define GFP_KERNEL  0
#define __printf(a, b) // Neutraliza a macro de checagem do GCC para o Mac ignorar

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

// === 6. TRADUÇÃO DE LOGS (PRINTK / PR_INFO) ===
#define pr_info(fmt, ...)  IOLog(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   IOLog(fmt, ##__VA_ARGS__)

// === 7. O TRUQUE DO VZALLOC E VFREE ===
static inline void* apple_vzalloc(unsigned long size) {
    void* mem = IOMallocZero(size + sizeof(unsigned long));
    if (!mem) return NULL;
    *(unsigned long*)mem = size; // Salva o tamanho no cabeçalho invisível
    return (void*)((char*)mem + sizeof(unsigned long));
}

static inline void apple_vfree(void* ptr) {
    if (!ptr) return;
    void* real_ptr = (void*)((char*)ptr - sizeof(unsigned long));
    unsigned long size = *(unsigned long*)real_ptr;
    IOFree(real_ptr, size + sizeof(unsigned long)); // Devolve tudo limpo pro macOS
}

#define vzalloc(size) apple_vzalloc(size)
#define vfree(ptr)    apple_vfree(ptr)

// === 8. NEUTRALIZADOR DE MACROS DO LINUX ===
#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_FIRMWARE(x)
#define MODULE_DEVICE_TABLE(x, y)
#define module_param_named(name, value, type, perm)
#define MODULE_PARM_DESC(name, desc)
#define SIMPLE_DEV_PM_OPS(name, suspend, resume)

#endif // APPLE_LINUX_EMULATION_H
