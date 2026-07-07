#ifndef RTL8723BE_MACDRIVER_HPP
#define RTL8723BE_MACDRIVER_HPP

#include <IOKit/network/IOEthernetController.h>
#include <IOKit/pci/IOPCIDevice.h>

// Engana o macOS fazendo-o tratar a placa de Wi-Fi como uma placa de rede controlável
class RTL8723BE_MacDriver : public IOEthernetController {
    OSDeclareDefaultStructors(RTL8723BE_MacDriver)

private:
    IOPCIDevice* pciDevice; // Guarda a referência física do slot PCI do seu notebook

public:
    // Ciclo de vida que o macOS vai executar:
    virtual bool init(OSDictionary* dictionary = nullptr) override;
    virtual void free() override;
    virtual IOService* probe(IOService* provider, SInt32* score) override;
    virtual bool start(IOService* provider) override;
    virtual void stop(IOService* provider) override;
};

#endif // RTL8723BE_MACDRIVER_HPP