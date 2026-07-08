#include "RTL8723BE_MacDriver.hpp"
#include <IOKit/IOLib.h>
#include <IOKit/pci/IOPCIDevice.h>

// === TRUQUE DA ANATOMIA: Importando o mundo em C puro ===
extern "C" {
    #include "apple_linux_emulation.h"
    #include "sw.h"
    #include "hw.h"
}

// Configura o registro da classe no Kernel do macOS
OSDefineMetaClassAndStructors(RTL8723BE_MacDriver, IOEthernetController)

// Variavel global ou de escopo para manter o mapa da memoria PCI vivo
IOMemoryMap* mmioMap = nullptr;
volatile uint8_t* mmioAddress = nullptr;

// === 1. INICIALIZAÇÃO DA KEXT ===
bool RTL8723BE_MacDriver::init(OSDictionary* dictionary) {
    if (!IOEthernetController::init(dictionary)) {
        return false;
    }
    
    IOLog("RTL8723BE_Mac: Sistema operacional chamou o init. Começando o transplante...\n");
    pciDevice = nullptr;
    return true;
}

// === 2. SONDAGEM DO HARDWARE ===
IOService* RTL8723BE_MacDriver::probe(IOService* provider, SInt32* score) {
    IOPCIDevice* pciDev = OSDynamicCast(IOPCIDevice, provider);
    if (!pciDev) {
        return nullptr;
    }

    IOLog("RTL8723BE_Mac: Placa Realtek 8723BE detectada no slot PCI!\n");
    return this; // Informa ao Mac que nós aceitamos controlar essa placa
}

// === 3. O CORAÇÃO: LIGANDO O MOTOR ===
bool RTL8723BE_MacDriver::start(IOService* provider) {
    if (!IOEthernetController::start(provider)) {
        return false;
    }

    // Pega o controle físico do slot PCI
    pciDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!pciDevice) {
        return false;
    }

    // Ativa a eletricidade e permissões de barramento da placa PCI
    pciDevice->setBusMasterEnable(true);

    // === BLINDAGEM ANTI-CRASH: Mapeamento de Memória I/O (MMIO) ===
    // Mapeia o BAR 0 (registrador 0x10) para obtermos o endereço virtual estável
    mmioMap = pciDevice->mapDeviceMemoryWithRegister(0x10);
    if (mmioMap) {
        mmioAddress = (volatile uint8_t*)mmioMap->getVirtualAddress();
        IOLog("RTL8723BE_Mac: Memoria fisica MMIO mapeada com sucesso no endereco %p\n", mmioAddress);
    } else {
        IOLog("RTL8723BE_Mac: AVISO CRITICO: Falha ao mapear MMIO da placa Realtek!\n");
    }

    IOLog("RTL8723BE_Mac: Ligando o sistema nervoso da Realtek...\n");

    // === AQUI O C++ DO MAC CHAMA O C DO LINUX ===
    struct ieee80211_hw* fake_hw = (struct ieee80211_hw*)IOMallocZero(sizeof(struct ieee80211_hw));
    
    if (fake_hw) {
        int resultado = rtl8723be_init_sw_vars(fake_hw);
        
        if (resultado == 0) {
            IOLog("RTL8723BE_Mac: Sucesso! rtl8723be_init_sw_vars rodou dentro do macOS!\n");
        } else {
            IOLog("RTL8723BE_Mac: Erro ao iniciar variáveis de software.\n");
        }
    }

    // === FORÇA O STATUS DO LINK COMO ATIVO E VALIDO ===
    // Evita que o macOS jogue a interface no limbo do estado "inactive"
    setLinkStatus(kIONetworkLinkValid | kIONetworkLinkActive);

    return true;
}

// === IMPLEMENTAÇÃO DO MÉTODO OBRIGATÓRIO DO MAC ===
IOReturn RTL8723BE_MacDriver::getHardwareAddress(IOEthernetAddress * addrP) {
    if (!addrP) {
        return kIOReturnBadArgument;
    }

    // Injetando o endereço MAC real extraído do Windows: 70-8B-CD-C0-EB-14
    addrP->bytes[0] = 0x70;
    addrP->bytes[1] = 0x8B;
    addrP->bytes[2] = 0xCD;
    addrP->bytes[3] = 0xC0;
    addrP->bytes[4] = 0xEB;
    addrP->bytes[5] = 0x14;

    return kIOReturnSuccess;
}

// === 4. DESLIGANDO O MOTOR ===
void RTL8723BE_MacDriver::stop(IOService* provider) {
    IOLog("RTL8723BE_Mac: Driver parando...\n");
    
    if (pciDevice) {
        pciDevice->setBusMasterEnable(false);
    }
    
    // Desaloca o mapa de memória para evitar vazamento na RAM (Memory Leak)
    if (mmioMap) {
        mmioMap->release();
        mmioMap = nullptr;
    }
    
    IOEthernetController::stop(provider);
}

// === 5. LIMPEZA DA MEMÓRIA RAM ===
void RTL8723BE_MacDriver::free() {
    IOLog("RTL8723BE_Mac: Memória liberada.\n");
    IOEthernetController::free();
}
