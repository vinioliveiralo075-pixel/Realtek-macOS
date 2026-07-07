# Detecta dinamicamente a pasta do sistema (SDK) do Mac no GitHub
SDKPATH := $(shell xcrun --show-sdk-path)

CC = clang
CXX = clang++

# Flags de compilação com o caminho corrigido para os arquivos do Mac (-I...)
CFLAGS = -mkernel -arch x86_64 -fno-builtin -g -O2 -DKERNEL -D_KERNEL -DKERNEL_PRIVATE -DAPPLE -std=gnu99 \
         -I. -I.. -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers

CXXFLAGS = -mkernel -arch x86_64 -fno-builtin -fno-rtti -fno-exceptions -g -O2 -DKERNEL -D_KERNEL -DKERNEL_PRIVATE -DAPPLE -std=c++11 \
           -I. -I.. -I$(SDKPATH)/System/Library/Frameworks/Kernel.framework/Headers

LDFLAGS = -mkernel -arch x86_64 -Xlinker -kext -nostdlib -lkmodc++ -lkmod -lcc_kext

# Nome da nossa Kext do Mac
KEXT_NAME = RTL8723BE_MacDriver
KEXT_BUNDLE = $(KEXT_NAME).kext
KEXT_BINARY = $(KEXT_BUNDLE)/Contents/MacOS/$(KEXT_NAME)

# Mapeia todos os arquivos .c e .cpp da pasta
C_SRCS = $(wildcard *.c)
CPP_SRCS = $(wildcard *.cpp)
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

clean:
	rm -rf *.o $(KEXT_BUNDLE)
