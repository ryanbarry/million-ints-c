## example run

run on my not-too-old laptop

```sh
ryan@comp ~/c/million-ints-c (master)> make
cc main.c -o mmints
ryan@comp ~/c/million-ints-c (master)> ./mmints create somanyintsinthisfile
started: 2020-05-28 07:43:00.016968
created: 2020-05-28 07:43:00.029017 delta:  12049µs
written: 2020-05-28 07:43:00.056835 delta:  27818µs
ryan@comp ~/c/million-ints-c (master)> ./mmints sort somanyintsinthisfile
started: 2020-05-28 07:43:10.062150
   read: 2020-05-28 07:43:10.068305 delta:   6155µs
 sorted: 2020-05-28 07:43:10.282200 delta: 213895µs
written: 2020-05-28 07:43:10.303273 delta:  21073µs
ryan@comp ~/c/million-ints-c (master)> ls -l
total 31320
-rw-r--r--  1 ryan  staff       73 May 27 23:41 Makefile
-rw-r--r--  1 ryan  staff      577 May 28 00:44 README.md
-rw-r--r--  1 ryan  staff     3710 May 28 00:37 main.c
-rwxr-xr-x  1 ryan  staff    14068 May 28 00:42 mmints*
-rw-r--r--  1 ryan  staff  8000000 May 28 00:43 somanyintsinthisfile
-rw-r--r--  1 ryan  staff  8000000 May 28 00:43 somanyintsinthisfile_sorted
```
