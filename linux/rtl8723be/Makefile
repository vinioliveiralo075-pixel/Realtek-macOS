# Nome da nossa Kext final
MODULE_NAME = RTL8723BE_MacDriver
KEXT_NAME   = $(MODULE_NAME).kext

# Compiladores do macOS para Kernel
CC       = clang
CXX      = clang++

# === MAPA DA MINA: Lista exata de objetos que você mandou do Linux ===
REALTEK_OBJS = dm.o fw.o hw.o led.o phy.o pwrseq.o rf.o sw.o table.o trx.o

# Junta os arquivos da Realtek com a nossa ponte em C++ do Mac
OBJS     = $(REALTEK_OBJS) RTL8723BE_MacDriver.o

# Flags de Compilação do Kernel do Mac (Garante que rode em Hackintosh/Mac Intel)
KERNEL_FLAGS = -mkernel -arch x86_64 -nostdinc -fno-builtin -g -O2 \
               -DKERNEL -D_KERNEL -DKERNEL_PRIVATE -DAPPLE -DNeXT

CFLAGS   = $(KERNEL_FLAGS) -std=gnu99
CXXFLAGS = $(KERNEL_FLAGS) -std=c++14 -fno-rtti -fno-exceptions -fno-threadsafe-statics

# Flags do Linker (O que junta tudo num arquivo executável de Kernel)
LDFLAGS  = -Xlinker -kext -nostdlib -lkmodc++ -lkmod -ccversion 4.2.1

# Diretorios da estrutura da Kext
KEXT_CONTENTS = $(KEXT_NAME)/Contents
KEXT_MACOS    = $(KEXT_CONTENTS)/MacOS

# Regra Principal: Compilar e montar a estrutura da Kext
all: $(KEXT_MACOS)/$(MODULE_NAME) $(KEXT_CONTENTS)/Info.plist
	@echo "========================================================="
	@echo " 🎉 Sucesso! Sua Kext '$(KEXT_NAME)' foi gerada com êxito!"
	@echo "========================================================="

# Linkagem final do binário do driver
$(KEXT_MACOS)/$(MODULE_NAME): $(OBJS)
	@mkdir -p $(KEXT_MACOS)
	$(CC) $(KERNEL_FLAGS) $(LDFLAGS) $(OBJS) -o $@

# Regra para compilar os arquivos em C (Realtek)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar o arquivo em C++ (Mac Driver)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Copia o Info.plist para dentro da Kext na estrutura correta
$(KEXT_CONTENTS)/Info.plist: Info.plist
	@mkdir -p $(KEXT_CONTENTS)
	cp Info.plist $(KEXT_CONTENTS)/Info.plist

# Limpeza dos arquivos temporários de compilação
clean:
	rm -rf *.o $(KEXT_NAME)
	@echo "🧹 Pasta limpa!"# Nome da nossa Kext final
MODULE_NAME = RTL8723BE_MacDriver
KEXT_NAME   = $(MODULE_NAME).kext

# Compiladores do macOS para Kernel
CC       = clang
CXX      = clang++

# === MAPA DA MINA: Lista exata de objetos que você mandou do Linux ===
REALTEK_OBJS = dm.o fw.o hw.o led.o phy.o pwrseq.o rf.o sw.o table.o trx.o

# Junta os arquivos da Realtek com a nossa ponte em C++ do Mac
OBJS     = $(REALTEK_OBJS) RTL8723BE_MacDriver.o

# Flags de Compilação do Kernel do Mac (Garante que rode em Hackintosh/Mac Intel)
KERNEL_FLAGS = -mkernel -arch x86_64 -nostdinc -fno-builtin -g -O2 \
               -DKERNEL -D_KERNEL -DKERNEL_PRIVATE -DAPPLE -DNeXT

CFLAGS   = $(KERNEL_FLAGS) -std=gnu99
CXXFLAGS = $(KERNEL_FLAGS) -std=c++14 -fno-rtti -fno-exceptions -fno-threadsafe-statics

# Flags do Linker (O que junta tudo num arquivo executável de Kernel)
LDFLAGS  = -Xlinker -kext -nostdlib -lkmodc++ -lkmod -ccversion 4.2.1

# Diretorios da estrutura da Kext
KEXT_CONTENTS = $(KEXT_NAME)/Contents
KEXT_MACOS    = $(KEXT_CONTENTS)/MacOS

# Regra Principal: Compilar e montar a estrutura da Kext
all: $(KEXT_MACOS)/$(MODULE_NAME) $(KEXT_CONTENTS)/Info.plist
	@echo "========================================================="
	@echo " 🎉 Sucesso! Sua Kext '$(KEXT_NAME)' foi gerada com êxito!"
	@echo "========================================================="

# Linkagem final do binário do driver
$(KEXT_MACOS)/$(MODULE_NAME): $(OBJS)
	@mkdir -p $(KEXT_MACOS)
	$(CC) $(KERNEL_FLAGS) $(LDFLAGS) $(OBJS) -o $@

# Regra para compilar os arquivos em C (Realtek)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar o arquivo em C++ (Mac Driver)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Copia o Info.plist para dentro da Kext na estrutura correta
$(KEXT_CONTENTS)/Info.plist: Info.plist
	@mkdir -p $(KEXT_CONTENTS)
	cp Info.plist $(KEXT_CONTENTS)/Info.plist

# Limpeza dos arquivos temporários de compilação
clean:
	rm -rf *.o $(KEXT_NAME)
	@echo "🧹 Pasta limpa!"
