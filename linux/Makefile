SDKPATH ?= $(shell xcrun --show-sdk-path)
CC ?= clang

override CFLAGS += -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos11 \
                   -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
                   -I$(SDKPATH)/usr/include \
                   -I. -Irtl8723be -Irtl8723com -Ibtcoexist \
                   -DKERNEL -DKERNEL_PRIVATE -fno-builtin \
                   -DCONFIG_RTLWIFI_DEBUG -DBT_SUPPORT=1 -DCOEX_SUPPORT=1

LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib \
          -Xlinker -rpath -Xlinker /System/Library/Extensions \
          -lkmod -lcc_kext -target x86_64-apple-macos11

ROOT_OBJS = base.o cam.o core.o debug.o efuse.o ps.o rc.o regd.o stats.o pci.o

BE_DIR  = rtl8723be
BTC_DIR = btcoexist
COM_DIR = rtl8723com

KEXT_BUNDLE = RealtekPCI-E_Version.kext
TARGET      = $(KEXT_BUNDLE)/Contents/MacOS/RealtekPCI-E_Version

all: prep_headers subdirs $(TARGET)

prep_headers:
	@mkdir -p linux
	@touch linux/module.h linux/ip.h linux/init.h linux/kernel.h linux/types.h linux/pci.h linux/netdevice.h linux/etherdevice.h linux/vmalloc.h linux/export.h

subdirs:
	$(MAKE) -C $(COM_DIR) CFLAGS="$(CFLAGS)" CC="$(CC)"
	$(MAKE) -C $(BTC_DIR) CFLAGS="$(CFLAGS)" CC="$(CC)"
	$(MAKE) -C $(BE_DIR) CFLAGS="$(CFLAGS)" CC="$(CC)"

$(TARGET): $(ROOT_OBJS) subdirs
	@mkdir -p $(KEXT_BUNDLE)/Contents/MacOS
	$(CC) $(LDFLAGS) $(ROOT_OBJS) $(BE_DIR)/*.o $(COM_DIR)/*.o $(BTC_DIR)/*.o -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -rf linux/ $(KEXT_BUNDLE)
	$(MAKE) -C $(COM_DIR) clean || true
	$(MAKE) -C $(BTC_DIR) clean || true
	$(MAKE) -C $(BE_DIR) clean || true
