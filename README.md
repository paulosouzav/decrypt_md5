# Quebrar MD5
Projeto feito para a disciplina de Arquiteturas Paralelas e Distribuídas (APD) e consiste em um programa escrito em C utilizado para quebrar uma hash md5 por força bruta.

## Rodando o código

Para compilar basta rodar o comando:
```
$ gcc -fopenmp decrypt_md5.c -o decrypt_md5.o -I /usr/local/opt/openssl/
```

Ou basta rodar os comandos do arquivo `Makefile`.
```
// Para compilar:
$ make compile

// Para rodar:
$ make run

// Para limpar o projeto:
$ make clean
```

## Testando o Código

A hash MD5 de teste é:
```
24b91372fa71abbdf7f69b88834cfaa7
```

O resultado dessa é o texto a seguir:
```
@T$1
```

### Outras hashes para teste

```
@T$1   =  24b91372fa71abbdf7f69b88834cfaa7
!      =  9033e0e305f247c0c3c80d0c7848c8b3
@      =  518ed29525738cebdac49c49e60ea9d3
@1     =  68986ab776eb5d6b5a809a1c005a7300
aaaa   =  74b87337454200d4d33f80c4663dc5e5
!!!!   =  98abe3a28383501f4bfd2d9077820f11
!!!!!  =  952bccf9afe8e4c04306f70f7bed6610
!@!!!  =  9eed591efa53e93c1c7b172733786ff7
!!!@!  =  c5800ffa9e056207ae1f26a993c4e06e
FeRi@s =  369349d4f440d4e139b3204121588d39
```