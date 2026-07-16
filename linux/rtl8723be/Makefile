CC ?= clang
SDKPATH = $(shell xcrun --show-sdk-path)

# Removido o -w perigoso e adicionado o ?= para aceitar a configuração do mestre
CFLAGS ?= -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos12 \
          -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
          -I$(SDKPATH)/usr/include \
          -I. -I../ -I../rtl8723com -I../btcoexist -DKERNEL -DKERNEL_PRIVATE -fno-builtin

OBJS = dm.o fw.o hw.o led.o phy.o pwrseq.o pwrseqcmd.o rf.o sw.o table.o trx.o RTL8723BE_MacDriver.o

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions -c $< -o $@

clean:
	rm -f *.o
