#ifndef APPLE_LINUX_EMULATION_H
#define APPLE_LINUX_EMULATION_H

#include <IOKit/IOLib.h> // O coração do Kernel do macOS (substitui o linux/kernel.h)

// === 1. TIPOS E MACROS ESSENCIAIS ===
#define THIS_MODULE NULL
#define GFP_KERNEL  0

// Substituto para o linux/bug.h (calcula a quantidade de itens em uma lista)
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

// === 2. TRADUÇÃO DE LOGS (PRINTK / PR_INFO) ===
// O Linux usa pr_info, o Mac usa IOLog para registrar as mensagens no console do sistema
#define pr_info(fmt, ...)  IOLog(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...)   IOLog(fmt, ##__VA_ARGS__)

// === 3. O TRUQUE DO VZALLOC E VFREE ===
// O Linux limpa a memória passando só o ponteiro. O Mac exige o tamanho exato ao liberar.
// Criamos um mini-gerenciador que esconde o tamanho da memória antes do ponteiro real.
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

// === 4. NEUTRALIZADOR DE MACROS DO LINUX ===
// Engana o compilador do Mac fazendo ele ignorar os comandos de metadados do Linux
#define MODULE_LICENSE(x)
#define MODULE_AUTHOR(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_FIRMWARE(x)
#define MODULE_DEVICE_TABLE(x, y)
#define module_param_named(name, value, type, perm)
#define MODULE_PARM_DESC(name, desc)
#define SIMPLE_DEV_PM_OPS(name, suspend, resume)

#endif // APPLE_LINUX_EMULATION_H