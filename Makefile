GCC = gcc
TARGET = decrypt_md5
LINKS = -fopenmp
OPENSSL = -I /usr/local/opt/openssl/

compile: $(TARGET).c
	$(GCC) $(LINKS) $< -o $(TARGET).o $(OPENSSL)

compileV2: $(TARGET)_v2.c
	$(GCC) $(LINKS) $< -o $(TARGET).o $(OPENSSL)

run:
	@./$(TARGET).o

clean:
	rm -f $(TARGET) $(TARGET).o $(TARGET).out
