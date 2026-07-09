# Detecta o caminho do SDK automaticamente
SDKPATH := $(shell xcrun --sdk macosx --show-sdk-path)

CC = clang
CXX = clang++

# Flags essenciais para Kext: -mkernel, -nostdlib, -target (macOS 12.0)
# Adicionado -iframework para o compilador achar o IOKit como um Framework
KEXT_FLAGS = -mkernel -arch x86_64 -fno-builtin -g -O2 -DKERNEL -D_KERNEL -DKERNEL_PRIVATE -DAPPLE -std=gnu99 -w \
             -target x86_64-apple-macos12 \
             -I. -I.. -I../.. \
             -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers \
             -iframework $(SDKPATH)/System/Library/Frameworks

CFLAGS = $(KEXT_FLAGS)
CXXFLAGS = $(KEXT_FLAGS) -std=c++11 -fno-rtti -fno-exceptions

LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib -lkmodc++ -lkmod -lcc_kext -target x86_64-apple-macos12

KEXT_NAME = RTL8723BE_MacDriver
KEXT_BUNDLE = $(KEXT_NAME).kext
KEXT_BINARY = $(KEXT_BUNDLE)/Contents/MacOS/$(KEXT_NAME)

# ALTERAÇÃO AQUI: Agora busca os códigos na pasta atual e na pasta anterior (..)
C_SRCS = $(wildcard *.c) $(wildcard ../*.c)
CPP_SRCS = $(wildcard *.cpp) $(wildcard ../*.cpp)
OBJS = $(C_SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)

all: $(KEXT_BINARY)

$(KEXT_BINARY): $(OBJS)
	@mkdir -p $(KEXT_BUNDLE)/Contents/MacOS
	$(CC) $(LDFLAGS) $(OBJS) -o $(KEXT_BINARY)
	@cp -f Info.plist $(KEXT_BUNDLE)/Contents/Info.plist
	@echo "Kext criada com sucesso!"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ALTERAÇÃO AQUI: Limpa os arquivos .o de ambas as pastas para não dar conflito
clean:
	rm -rf *.o ../*.o $(KEXT_BUNDLE)
