SDKPATH ?= $(shell xcrun --show-sdk-path)
CC ?= clang

# Permitir que CFLAGS vindo do ambiente/GitHub Actions seja estendido
override CFLAGS += -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos12 \
                   -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
                   -I$(SDKPATH)/usr/include \
                   -I. -I.. -Irtl8723be -Irtl8723com -Ibtcoexist -I../rtl8723be -I../rtl8723com -I../btcoexist \
                   -DKERNEL -DKERNEL_PRIVATE -fno-builtin \
                   -DCONFIG_RTLWIFI_DEBUG -DBT_SUPPORT=1 -DCOEX_SUPPORT=1 \
                   -include apple_linux_emulation.h

LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib \
          -Xlinker -rpath -Xlinker /System/Library/Extensions \
          -lkmod -lcc_kext -target x86_64-apple-macos12

ROOT_OBJS = base.o cam.o core.o debug.o efuse.o ps.o rc.o regd.o stats.o pci.o usb.o

# Subdiretórios do driver
BE_DIR  = rtl8723be
BTC_DIR = btcoexist
COM_DIR = rtl8723com

# Novo nome do arquivo final da Kext
KEXT_BUNDLE = RealtekPCI-E_Version.kext
TARGET      = $(KEXT_BUNDLE)/Contents/MacOS/RealtekPCI-E_Version

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

$(TARGET): $(ROOT_OBJS) subdirs
	@mkdir -p $(KEXT_BUNDLE)/Contents/MacOS
	$(CC) $(LDFLAGS) $(ROOT_OBJS) $(BE_DIR)/*.o $(COM_DIR)/*.o $(BTC_DIR)/*.o -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions -c $< -o $@

clean:
	rm -f *.o rtl8723ae
	rm -rf linux/ $(KEXT_BUNDLE)
	$(MAKE) -C $(COM_DIR) clean
	$(MAKE) -C $(BTC_DIR) clean
	$(MAKE) -C $(BE_DIR) clean
