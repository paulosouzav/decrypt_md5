GCC = gcc-7
TARGET = decrypt_md5
LINKS = -fopenmp
OPENSSL = -I /usr/local/opt/openssl/

compile: $(TARGET)_sequential.c
	$(GCC) $(LINKS) $< -o $(TARGET).o $(OPENSSL)

compile_parallel: $(TARGET)_parallel.c
	$(GCC) $(LINKS) $< -o $(TARGET).o $(OPENSSL)

run:
	@./$(TARGET).o

clean:
	rm -f $(TARGET) $(TARGET).o $(TARGET).out
