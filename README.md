# ASLR-gen

Благодаря технологии ASLR операционная система при каждом запуске выделяет программе новые адреса в стеке. Программа берет эти адреса, смешивает их битовыми операциями и использует как начальное число для генерации символов.

## Как собрать и запустить

В Linux (GCC):

```bash
gcc src/main.c -o pass_gen
./pass_gen
```

В macOS (Clang):

```bash
clang src/main.c -o pass_gen
./pass_gen
```

В Windows (MinGW):

```bash
gcc src/main.c -o pass_gen.exe
pass_gen.exe
```
