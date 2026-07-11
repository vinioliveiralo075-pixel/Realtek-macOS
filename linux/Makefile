# ====================================================================
# Makefile Unificado para macOS (Kext RTL8723BE) - Vini Tech
# ====================================================================

SDKPATH = $(shell xcrun --show-sdk-path)
CC = clang

# Parâmetros de compilação do Kernel do macOS (X86_64)
# Inclui os caminhos corretos de todas as subpastas que guardam os códigos da Realtek
CFLAGS = -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos12 -w \
         -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
         -I$(SDKPATH)/usr/include \
         -I. -Irtl8723be -Irtl8723com -Ibtcoexist \
         -DKERNEL -DKERNEL_PRIVATE -fno-builtin -DCONFIG_RTLWIFI_DEBUG

# Parâmetros de Linkagem da Kext
LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib \
          -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
          -Xlinker -rpath -Xlinker /System/Library/Extensions \
          -lkmod -lcc_kext -target x86_64-apple-macos12

# 1. ARQUIVOS DA PASTA RAIZ (Arquivos base do rtlwifi do Linux que migraram)
ROOT_OBJS = base.o cam.o core.o debug.o efuse.o ps.o rc.o regd.o stats.o pci.o usb.o

# 2. ARQUIVOS DA SUBPASTA rtl8723be (Onde está o wrapper e os arquivos específicos do chip)
# Nota: Adicionei o seu RTL8723BE_MacDriver.o aqui!
BE_OBJS = rtl8723be/dm.o rtl8723be/fw.o rtl8723be/hw.o rtl8723be/led.o \
          rtl8723be/phy.o rtl8723be/pwrseq.o rtl8723be/pwrseqcmd.o \
          rtl8723be/rf.o rtl8723be/sw.o rtl8723be/table.o rtl8723be/trx.o \
          rtl8723be/RTL8723BE_MacDriver.o

# 3. ARQUIVOS DA SUBPASTA btcoexist (Controle de interferência Bluetooth/Wi-Fi)
BTC_OBJS = btcoexist/halbtc8723b1ant.o btcoexist/halbtc8723b2ant.o \
           btcoexist/halbtcoutsrc.o btcoexist/rtl_btc.o

# 4. ARQUIVOS DA SUBPASTA rtl8723com (Funções comuns compartilhadas)
COM_OBJS = rtl8723com/dm_common.o rtl8723com/fw_common.o rtl8723com/main.o \
           rtl8723com/phy_common.o

# Junta todas as listas de objetos que precisam ser criadas
OBJS = $(ROOT_OBJS) $(BE_OBJS) $(BTC_OBJS) $(COM_OBJS)

# Destino final do binário dentro da estrutura do Mac
TARGET = RTL8723BE_MacDriver.kext/Contents/MacOS/RTL8723BE_MacDriver

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "============================================="
	@echo "Linkando e gerando o binário final da Kext..."
	@echo "============================================="
	@mkdir -p RTL8723BE_MacDriver.kext/Contents/MacOS
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)
	@echo "Kext compilada com sucesso!"

# Regra para compilar qualquer arquivo C do projeto (Raiz ou Subpastas)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar o arquivo C++ do driver do macOS
%.o: %.cpp
	$(CC) $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions -c $< -o $@

clean:
	@echo "Limpando arquivos antigos..."
	rm -f *.o rtl8723be/*.o btcoexist/*.o rtl8723com/*.o $(TARGET)
