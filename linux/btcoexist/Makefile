CC ?= clang
SDKPATH = $(shell xcrun --show-sdk-path)

# Removido o -w perigoso e adicionado o ?= para aceitar a configuração do mestre
CFLAGS ?= -mkernel -arch x86_64 -nostdinc -target x86_64-apple-macos12 \
          -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
          -I$(SDKPATH)/usr/include \
          -I. -I../ -DKERNEL -DKERNEL_PRIVATE -fno-builtin

OBJS = halbtc8723b1ant.o halbtc8723b2ant.o halbtcoutsrc.o rtl_btc.o

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
