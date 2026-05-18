# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall

# Arquivos fonte
SRCS = main.c time.c bdtimes.c partida.c bdpartidas.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# Nome do executável
TARGET = sistema

# Alvos que não representam arquivos
.PHONY: all compile run clean

# Alvo padrão (compilar e executar)
all: compile run

# Alvo explícito de compilação (gera o executável)
compile: $(TARGET)

# Executa o programa
run: $(TARGET)
	./$(TARGET)

# Limpeza dos arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)

# Compila cada arquivo fonte em seu respectivo arquivo objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
# Linka os arquivos objeto para gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)