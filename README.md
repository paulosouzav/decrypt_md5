# Quebrar MD5
Projeto feito para a disciplina de Arquiteturas Paralelas e Distribuídas (APD) e consiste em um programa escrito em C utilizado para quebrar uma hash md5 por força bruta.

## Rodando o código

Para compilar basta rodar o comando:
```
$ gcc -fopenmp decrypt_md5.c -o decrypt_md5.o -I /usr/local/opt/openssl/
```

Ou basta rodar os comandos do arquivo `Makefile`.
```
// Para compilar o código paralelo
$ make compile_parallel

// Para compilar o código sequencial:
$ make compile

// Para rodar:
$ make run

// Para limpar o projeto:
$ make clean
```

## Testando o Código

Como no trabalho dado, fomos informados do tamanho da palavra, você apenas precisa definir o `número de threads`, o `tamanho da palavra` nas linhas `187` e `188` do arquivo `decrypt_md5_parallel.c`.
```
#define NUMERO_DE_THREADS    4
#define TAMANHO_PALAVRA      4
```


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
@T$1    =  24b91372fa71abbdf7f69b88834cfaa7
!       =  9033e0e305f247c0c3c80d0c7848c8b3
@       =  518ed29525738cebdac49c49e60ea9d3
@1      =  68986ab776eb5d6b5a809a1c005a7300
zz      =  25ed1bcb423b0b7200f485fc5ff71c8e
zzz     =  f3abb86bd34cf4d52698f14c0da1dc60
aaaa    =  74b87337454200d4d33f80c4663dc5e5
!!!!    =  98abe3a28383501f4bfd2d9077820f11
!!!!!   =  952bccf9afe8e4c04306f70f7bed6610
!@!!!   =  9eed591efa53e93c1c7b172733786ff7
!!!@!   =  c5800ffa9e056207ae1f26a993c4e06e
1W0rd   =  f2dbb73795afcbcb56eaca0db8d765b2
FeRi@s  =  369349d4f440d4e139b3204121588d39
1W0rd!@ =  6ca812d547a65f36c7f9b52e85cabd3d
!!!!!!! =  85833abc49f38e7bb1042b14d64a22cf

// Para a entrega:
T$i     =  6b4a1e0b6612fd53a2b85922c5d5bd12
#R%f    =  26757bfdf2c738b0aeb3d634e677a482
=@pD?   =  d63f657df132357091c5b51031bba8bf
```