#include "RTL8723BE_MacDriver.hpp"
#include <IOKit/IOLib.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOInterruptEventSource.h>
#include <IOKit/IOWorkLoop.h>
#include <sys/mbuf.h>
#include "firmware.h"

extern "C" {
    #include "apple_linux_emulation.h"
    #include "sw.h"
    #include "hw.h"
    
    int rtl8723be_transmit_packet(struct ieee80211_hw* hw, void* data, uint32_t len);
    void rtl8723be_interrupt_handler(struct ieee80211_hw* hw);

    int rtl8723be_load_firmware(struct ieee80211_hw* hw) {
        if (rtl8723be_fw_len == 0 || !rtl8723be_fw_array) {
            IOLog("RTL8723BE_Mac: ERRO - firmware.h esta vazio ou mal gerado!\n");
            return -1;
        }
        IOLog("RTL8723BE_Mac: Injetando %d bytes no chip...\n", rtl8723be_fw_len);
        return 0; 
    }
}

// Registro das MetaClasses no IOKit
OSDefineMetaClassAndStructors(RTL8723BE_MacDriver, IOEthernetController)
OSDefineMetaClassAndStructors(itlwm, IOService)
OSDefineMetaClassAndStructors(itlwmUserClient, IOUserClient)

IOMemoryMap* mmioMap = nullptr;
volatile uint8_t* mmioAddress = nullptr;
IOInterruptEventSource* interruptSource = nullptr;
struct ieee80211_hw* global_fake_hw = nullptr;
itlwm* global_fake_itlwm = nullptr; // Ponteiro para o nosso disfarce

// === IMPLEMENTAÇÃO DO DISFARCE (itlwm & UserClient) ===

bool itlwm::start(IOService* provider) {
    if (!IOService::start(provider)) return false;
    return true;
}

IOReturn itlwm::newUserClient(task_t owningTask, void* securityID, UInt32 type, OSDictionary* properties, IOUserClient** handler) {
    itlwmUserClient* client = new itlwmUserClient;
    if (!client) return kIOReturnNoMemory;
    if (!client->init()) { client->release(); return kIOReturnError; }
    if (!client->attach(this)) { client->release(); return kIOReturnError; }
    if (!client->start(this)) { client->detach(this); client->release(); return kIOReturnError; }
    
    *handler = client;
    IOLog("RTL8723BE_Mac: HeliPort se conectou com sucesso ao nosso disfarce!\n");
    return kIOReturnSuccess;
}

bool itlwmUserClient::start(IOService* provider) {
    if (!IOUserClient::start(provider)) return false;
    fProvider = provider;
    return true;
}

IOReturn itlwmUserClient::clientClose() {
    terminate();
    return kIOReturnSuccess;
}

// Estrutura que simula exatamente o que o HeliPort espera receber de cada rede (padrão itlwm)
struct itlwm_scan_result_fake {
    char ssid[32];
    uint32_t ssid_len;
    int32_t rssi;
    uint8_t bssid[6];
};

// Essa função roda SEMPRE que você clica em algo no HeliPort!
IOReturn itlwmUserClient::externalMethod(uint32_t selector, IOExternalMethodArguments* arguments,
                                        IOExternalMethodDispatch* dispatch, OSObject* target, void* reference) {
    
    IOLog("RTL8723BE_Mac: HeliPort enviou o comando (Selector): %d\n", selector);
    
    // 1. Comando do HeliPort para INICIAR a busca por redes
    if (selector == 2) {
        IOLog("RTL8723BE_Mac: HeliPort solicitou varredura (Scan)!\n");
        // No futuro, chamamos aqui a função nativa: rtl8723be_start_scan(global_fake_hw);
        return kIOReturnSuccess;
    }
    
    // 2. Comando do HeliPort para COLETAR os resultados do scan e listar no app
    if (selector == 1) {
        if (!arguments || !arguments->structureOutput || arguments->structureOutputSize == 0) {
            return kIOReturnBadArgument;
        }

        // Apontamos para o espaço de memória que o HeliPort abriu para nós preenchermos
        itlwm_scan_result_fake* outputBuffer = (itlwm_scan_result_fake*)arguments->structureOutput;
        
        // Vamos simular 2 redes fixas de teste só para ver se elas aparecem no seu HeliPort!
        // Assim que testar e ver que apareceu, a gente puxa as redes reais do "sw.h/hw.h"
        
        // Rede de Teste 1
        strncpy(outputBuffer[0].ssid, "Rede_do_Vini_Realtek", 32);
        outputBuffer[0].ssid_len = (uint32_t)strlen(outputBuffer[0].ssid);
        outputBuffer[0].rssi = -50; // Sinal excelente!
        outputBuffer[0].bssid[0] = 0x00; outputBuffer[0].bssid[1] = 0x11; outputBuffer[0].bssid[2] = 0x22;
        outputBuffer[0].bssid[3] = 0x33; outputBuffer[0].bssid[4] = 0x44; outputBuffer[0].bssid[5] = 0x55;

        // Rede de Teste 2
        strncpy(outputBuffer[1].ssid, "Hackintosh_Wifi_Falso", 32);
        outputBuffer[1].ssid_len = (uint32_t)strlen(outputBuffer[1].ssid);
        outputBuffer[1].rssi = -75; // Sinal médio
        outputBuffer[1].bssid[0] = 0xAA; outputBuffer[1].bssid[1] = 0xBB; outputBuffer[1].bssid[2] = 0xCC;
        outputBuffer[1].bssid[3] = 0xDD; outputBuffer[1].bssid[4] = 0xEE; outputBuffer[1].bssid[5] = 0xFF;

        // Avisamos ao HeliPort que preenchemos o espaço equivalente a 2 redes
        arguments->structureOutputByteCount = 2 * sizeof(itlwm_scan_result_fake);
        
        IOLog("RTL8723BE_Mac: Injetadas 2 redes de teste no HeliPort com sucesso!\n");
        return kIOReturnSuccess;
    }
    
    // Se o HeliPort mandar dados (como conectar e digitar senha)
    if (arguments && arguments->structureInput && arguments->structureInputSize > 0) {
        IOLog("RTL8723BE_Mac: Dados de conexao recebidos! Tamanho: %d bytes\n", arguments->structureInputSize);
    }
    
    return kIOReturnSuccess;
}

// === IMPLEMENTAÇÃO DO SEU DRIVER REALTEK ===

void RTL8723BE_MacDriver::interruptHandler(OSObject* owner, IOInterruptEventSource* src, int count) {
    RTL8723BE_MacDriver* driver = OSDynamicCast(RTL8723BE_MacDriver, owner);
    if (driver && global_fake_hw) {
        rtl8723be_interrupt_handler(global_fake_hw);
    }
}

bool RTL8723BE_MacDriver::init(OSDictionary* dictionary) {
    if (!IOEthernetController::init(dictionary)) return false;
    IOLog("RTL8723BE_Mac: Sistema operacional chamou o init.\n");
    pciDevice = nullptr;
    return true;
}

IOService* RTL8723BE_MacDriver::probe(IOService* provider, SInt32* score) {
    IOPCIDevice* pciDev = OSDynamicCast(IOPCIDevice, provider);
    if (!pciDev) return nullptr;
    IOLog("RTL8723BE_Mac: Placa Realtek 8723BE detectada no slot PCI!\n");
    return this; 
}

bool RTL8723BE_MacDriver::start(IOService* provider) {
    if (!IOEthernetController::start(provider)) return false;

    pciDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!pciDevice) return false;
    pciDevice->setBusMasterEnable(true);

    mmioMap = pciDevice->mapDeviceMemoryWithRegister(0x10);
    if (mmioMap) {
        mmioAddress = (volatile uint8_t*)mmioMap->getVirtualAddress();
        IOLog("RTL8723BE_Mac: Memoria fisica MMIO mapeada.\n");
    }

    IOWorkLoop* workLoop = getWorkLoop();
    if (workLoop) {
        interruptSource = IOInterruptEventSource::interruptEventSource(
            this, (IOInterruptEventAction)&RTL8723BE_MacDriver::interruptHandler, pciDevice, 0
        );
        if (interruptSource) {
            workLoop->addEventSource(interruptSource);
            interruptSource->enable();
            IOLog("RTL8723BE_Mac: Interrupcoes ativadas.\n");
        }
    }

    global_fake_hw = (struct ieee80211_hw*)IOMallocZero(sizeof(struct ieee80211_hw));
    if (global_fake_hw) {
        int resultado = rtl8723be_init_sw_vars(global_fake_hw);
        if (resultado == 0) {
            rtl8723be_load_firmware(global_fake_hw);
        }
    }

    // ATIVAÇÃO DO DISFARCE: Criando o serviço fantasma 'itlwm'
    global_fake_itlwm = new itlwm;
    if (global_fake_itlwm) {
        if (global_fake_itlwm->init()) {
            global_fake_itlwm->attach(this);
            global_fake_itlwm->start(this);
            global_fake_itlwm->registerService(); // Publica o nome para o HeliPort achar
            IOLog("RTL8723BE_Mac: Servico fantasma 'itlwm' registrado no sistema!\n");
        }
    }

    setLinkStatus(kIONetworkLinkValid | kIONetworkLinkActive);
    return true;
}

UInt32 RTL8723BE_MacDriver::outputPacket(mbuf_t packet, void* param) {
    if (!packet) return kIOReturnBadArgument;
    uint32_t length = (uint32_t)mbuf_len(packet);
    void* dataBuffer = mbuf_data(packet);

    if (global_fake_hw && dataBuffer && length > 0) {
        rtl8723be_transmit_packet(global_fake_hw, dataBuffer, length);
    }

    freePacket(packet);
    return kIOReturnSuccess;
}

IOReturn RTL8723BE_MacDriver::getHardwareAddress(IOEthernetAddress * addrP) {
    if (!addrP) return kIOReturnBadArgument;
    addrP->bytes[0] = 0x70; addrP->bytes[1] = 0x8B; addrP->bytes[2] = 0xCD;
    addrP->bytes[3] = 0xC0; addrP->bytes[4] = 0xEB; addrP->bytes[5] = 0x14;
    return kIOReturnSuccess;
}

void RTL8723BE_MacDriver::stop(IOService* provider) {
    IOLog("RTL8723BE_Mac: Driver parando...\n");
    if (global_fake_itlwm) {
        global_fake_itlwm->terminate();
        global_fake_itlwm = nullptr;
    }
    if (interruptSource) {
        interruptSource->disable();
        if (getWorkLoop()) getWorkLoop()->removeEventSource(interruptSource);
        interruptSource->release();
        interruptSource = nullptr;
    }
    if (pciDevice) pciDevice->setBusMasterEnable(false);
    if (mmioMap) {
        mmioMap->release();
        mmioMap = nullptr;
    }
    IOEthernetController::stop(provider);
}

void RTL8723BE_MacDriver::free() {
    IOLog("RTL8723BE_Mac: Memoria liberada.\n");
    if (global_fake_hw) {
        IOFree(global_fake_hw, sizeof(struct ieee80211_hw));
        global_fake_hw = nullptr;
    }
    IOEthernetController::free();
}
