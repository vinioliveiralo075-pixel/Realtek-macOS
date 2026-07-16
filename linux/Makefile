SDKPATH = $(shell xcrun --show-sdk-path)
CC = clang

CFLAGS = -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos12 \
         -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
         -I$(SDKPATH)/usr/include \
         -I. -I.. -Irtl8723be -Irtl8723com -Ibtcoexist -I../rtl8723be -I../rtl8723com -I../btcoexist \
         -DKERNEL -DKERNEL_PRIVATE -fno-builtin \
         -DCONFIG_RTLWIFI_DEBUG -DBT_SUPPORT=1 -DCOEX_SUPPORT=1 \
         -include apple_linux_emulation.h

# Corrigido: Removidas as flags -I que não pertencem ao Linker (LDFLAGS)
LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib \
          -Xlinker -rpath -Xlinker /System/Library/Extensions \
          -lkmod -lcc_kext -target x86_64-apple-macos12

ROOT_OBJS = base.o cam.o core.o debug.o efuse.o ps.o rc.o regd.o stats.o pci.o usb.o

# Alvos construídos nos subdiretórios
BE_DIR  = rtl8723be
BTC_DIR = btcoexist
COM_DIR = rtl8723com

TARGET = RTL8723BE_MacDriver.kext/Contents/MacOS/RTL8723BE_MacDriver

all: patch_paths subdirs $(TARGET)

patch_paths:
	@rm -f rtl8723ae
	@ln -s rtl8723be rtl8723ae
	@mkdir -p linux
	@touch linux/module.h linux/ip.h linux/init.h linux/kernel.h linux/types.h linux/pci.h linux/netdevice.h linux/etherdevice.h linux/vmalloc.h

subdirs:
	$(MAKE) -C $(COM_DIR) CFLAGS="$(CFLAGS)" CC="$(CC)"
	$(MAKE) -C $(BTC_DIR) CFLAGS="$(CFLAGS)" CC="$(CC)"
	$(MAKE) -C $(BE_DIR) CFLAGS="$(CFLAGS)" CC="$(CC)"

# Corrigido: Garante que os subdirs rodem antes de tentar linkar tudo
$(TARGET): $(ROOT_OBJS) subdirs
	@mkdir -p RTL8723BE_MacDriver.kext/Contents/MacOS
	$(CC) $(LDFLAGS) $(ROOT_OBJS) $(BE_DIR)/*.o $(COM_DIR)/*.o $(BTC_DIR)/*.o -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions -c $< -o $@

clean:
	rm -f *.o $(TARGET) rtl8723ae
	rm -rf linux/
	$(MAKE) -C $(COM_DIR) clean
	$(MAKE) -C $(BTC_DIR) clean
	$(MAKE) -C $(BE_DIR) clean
