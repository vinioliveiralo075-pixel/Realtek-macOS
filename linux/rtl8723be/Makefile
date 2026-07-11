# Caminho automático para o SDK do macOS (Garante que o Clang ache as bibliotecas)
SDKPATH = $(shell xcrun --show-sdk-path)

CC = clang
CFLAGS = -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos12 \
         -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
         -I$(SDKPATH)/usr/include \
         -I. -D__KERNEL__ -Wno-implicit-function-declaration -Wno-incompatible-pointer-types

# LDFLAGS corrigido com -lc++_kext e o caminho do SDK (-L)
LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib -L$(SDKPATH)/usr/lib \
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
