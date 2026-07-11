SDKPATH = $(shell xcrun --show-sdk-path)
CC = clang

CFLAGS = -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos12 \
         -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
         -I$(SDKPATH)/usr/include \
         -I. -I.. -Irtl8723be -Irtl8723com -Ibtcoexist -I../rtl8723be -I../rtl8723com -I../btcoexist \
         -DKERNEL -DKERNEL_PRIVATE -fno-builtin \
         -DCONFIG_RTLWIFI_DEBUG -DBT_SUPPORT=1 -DCOEX_SUPPORT=1 \
         -include apple_linux_emulation.h

LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib \
          -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
          -Xlinker -rpath -Xlinker /System/Library/Extensions \
          -lkmod -lcc_kext -target x86_64-apple-macos12

ROOT_OBJS = base.o cam.o core.o debug.o efuse.o ps.o rc.o regd.o stats.o pci.o usb.o

BE_OBJS = rtl8723be/dm.o rtl8723be/fw.o rtl8723be/hw.o rtl8723be/led.o \
          rtl8723be/phy.o rtl8723be/pwrseq.o rtl8723be/pwrseqcmd.o \
          rtl8723be/rf.o rtl8723be/sw.o rtl8723be/table.o rtl8723be/trx.o \
          rtl8723be/RTL8723BE_MacDriver.o

BTC_OBJS = btcoexist/halbtc8723b1ant.o btcoexist/halbtc8723b2ant.o \
           btcoexist/halbtcoutsrc.o btcoexist/rtl_btc.o

COM_OBJS = rtl8723com/dm_common.o rtl8723com/fw_common.o rtl8723com/main.o \
           rtl8723com/phy_common.o

OBJS = $(ROOT_OBJS) $(BE_OBJS) $(BTC_OBJS) $(COM_OBJS)

TARGET = RTL8723BE_MacDriver.kext/Contents/MacOS/RTL8723BE_MacDriver

all: subdirs $(TARGET)

subdirs:
	$(MAKE) -C rtl8723com CFLAGS="$(CFLAGS)" CC="$(CC)"
	$(MAKE) -C btcoexist CFLAGS="$(CFLAGS)" CC="$(CC)"
	$(MAKE) -C rtl8723be CFLAGS="$(CFLAGS)" CC="$(CC)"

$(TARGET): $(ROOT_OBJS)
	@mkdir -p RTL8723BE_MacDriver.kext/Contents/MacOS
	$(CC) $(LDFLAGS) $(ROOT_OBJS) rtl8723be/*.o rtl8723com/*.o btcoexist/*.o -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions -c $< -o $@

clean:
	rm -f *.o $(TARGET)
	$(MAKE) -C rtl8723com clean
	$(MAKE) -C btcoexist clean
	$(MAKE) -C rtl8723be clean
