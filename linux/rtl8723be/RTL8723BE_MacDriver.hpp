#ifndef RTL8723BE_MACDRIVER_HPP
#define RTL8723BE_MACDRIVER_HPP

// Bloqueia infiltração do DriverKit moderno e ativa tipos legados de rede (como mbuf_t)
#ifndef XNU_KERNEL_PRIVATE
#define XNU_KERNEL_PRIVATE 1
#endif
#define __IOKIT_INLINE__ 1

// === TRUQUE PARA DESATIVAR A MACRO CONFLITANTE DO LINUX ===
#ifdef complete
    #define LINUX_COMPLETE_BACKUP complete
    #undef complete
#endif

#include <IOKit/network/IOEthernetController.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOInterruptEventSource.h>
#include <IOKit/IOUserClient.h>

// === DEVOLVE A MACRO PARA O CÓDIGO DA REALTEK EM C ===
#ifdef LINUX_COMPLETE_BACKUP
    #define complete LINUX_COMPLETE_BACKUP
    #undef LINUX_COMPLETE_BACKUP
#endif

// === CLASSE DA PONTE DE COMUNICAÇÃO (USER CLIENT) ===
class itlwmUserClient : public IOUserClient {
public:
    OSDeclareDefaultStructors(itlwmUserClient)
private:
    IOService* fProvider;
public:
    virtual bool start(IOService* provider) override;
    virtual IOReturn clientClose() override;
    virtual IOReturn externalMethod(uint32_t selector, IOExternalMethodArguments* arguments,
                                    IOExternalMethodDispatch* dispatch, OSObject* target, void* reference) override;
};

// === CLASSE FANTASMA PARA ENGANAR O HELIPORT ===
class itlwm : public IOService {
public:
    OSDeclareDefaultStructors(itlwm)
public:
    virtual bool start(IOService* provider) override;
    virtual IOReturn newUserClient(task_t owningTask, void* securityID, UInt32 type, OSDictionary* properties, IOUserClient** handler) override;
};

// === SEU DRIVER PRINCIPAL DA REALTEK ===
class RTL8723BE_MacDriver : public IOEthernetController {
public:
    OSDeclareDefaultStructors(RTL8723BE_MacDriver)

private:
    IOPCIDevice* pciDevice;

public:
    virtual bool init(OSDictionary* dictionary = nullptr) override;
    virtual void free() override;
    virtual IOService* probe(IOService* provider, SInt32* score) override;
    virtual bool start(IOService* provider) override;
    virtual void stop(IOService* provider) override;
    
    virtual IOReturn getHardwareAddress(IOEthernetAddress * addrP) override;
    virtual UInt32 outputPacket(mbuf_t packet, void* param) override;
    static void interruptHandler(OSObject* owner, IOInterruptEventSource* src, int count);
};

#endif // RTL8723BE_MACDRIVER_HPP
