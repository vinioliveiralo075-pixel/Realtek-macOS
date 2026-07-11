SDKPATH = $(shell xcrun --show-sdk-path)

CC = clang
# Trocado -D__KERNEL__ por -DKERNEL -DKERNEL_PRIVATE -fno-builtin para o ambiente macOS Kext correto
CFLAGS = -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos12 -w \
         -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
         -I$(SDKPATH)/usr/include \
         -I. -DKERNEL -DKERNEL_PRIVATE -fno-builtin

LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib -L$(SDKPATH)/usr/lib -w \
          -lkmodc++ -lc++_kext -lkmod -lcc_kext -target x86_64-apple-macos12

OBJS = dm.o fw.o hw.o led.o phy.o pwrseq.o pwrseqcmd.o rf.o sw.o table.o trx.o RTL8723BE_MacDriver.o
TARGET = RTL8723BE_MacDriver.kext/Contents/MacOS/RTL8723BE_MacDriver

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p RTL8723BE_MacDriver.kext/Contents/MacOS
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions -c $< -o $@

clean:
	rm -f *.o $(TARGET)
