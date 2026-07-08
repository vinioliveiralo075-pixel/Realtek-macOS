#ifndef RTL8723BE_MACDRIVER_HPP
#define RTL8723BE_MACDRIVER_HPP

#include <IOKit/network/IOEthernetController.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOInterruptEventSource.h>

class RTL8723BE_MacDriver : public IOEthernetController {
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
