# Tugas 4 - Struktur Data dan Algoritma

<div>
    <img src="https://img.shields.io/badge/-C-0373fc?logo=c&logoColor=white&style=for-the-badge">
</div>    

### Nama  : Muhammad Hizqil Alfi
### NPM   : 2308107010046

### Deskripsi

Repository ini memuat source code serta hasil analisis terhadap waktu eksekusi dan penggunaan memori dari berbagai metode pengurutan (sorting). Seluruh implementasi ditulis menggunakan bahasa pemrograman C.

### Tujuan

Mengukur dan membandingkan performa berbagai algoritma sorting (pengurutan), baik untuk data angka maupun kata acak, dalam hal waktu eksekusi dan penggunaan memori.

### File

Dalam repository ini terdapat 3 file utama yaitu : 
- sort_combined.h (file header)
- main_angka.c (file untuk melakukan generate angka)
- main_kata.c (file untuk melakukan generate kata)

*Folder docs berisi laporan dan data sheets eksperimen

### Cara Menjalankan

Untuk menjalankan file source code, 3 file utama harus dalam satu folder yang sama.

Compile (untuk angka) :
```c
gcc -o main_a main_angka.c -lpsapi
```

Compile (untuk kata) :
```c
gcc -o main_b main_kata.c -lpsapi
```

Note : Cara compile kemungkinan berbeda untuk setiap sistem operasi. dikarenakan flags ```-lpsapi``` kemungkinan tidak berjalan untuk sistem operasi Linux.

### Terimakasih

