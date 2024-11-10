# FAQ

## Apakah KernelSU mendukung perangkat saya?

Pertama, perangkatmu harus bisa dibuka bootloadernya. Jika tiddak bisa, berarti tidak memungkinkan untuk bekerja.

Lalu instal aplikasi KernelSU manager di dalam perangkatmu dan buka, jika terlihat `Unsupported` berarti perangkatmu tidak didukung dan tidak akan didukung di kemudian hari.

## Apakah KernelSU membutuhkan buka-bootloader?

Ya, seharusnya.

## Apakah KernelSU mendukung modul?

Ya, Tetapi masih dalam versi awal, bisa jadi ngebug. Mohon tunggu sampai semuanya stabil :)

## Apakah KernelSU mendukung Xposed?

Ya, [Dreamland](https://github.com/canyie/Dreamland) dan [TaiChi](https://taichi.cool) sekarang bekerja sebagian, Dan kita sedang mencoba untuk membuat Xposed Framework lainnya bekerja.

## Apakah KernelSU kompatibel dengan Magisk?

Sistem modul KernelSU bertentangan dengan magic mount Magisk, jika ada modul yang diaktifkan di KernelSU, maka seluruh Magisk tidak akan bekerja.

Tetapi jika Anda hanya menggunakan `su` dari KernelSU, maka KernelSU akan bekerja dengan baik dengan Magisk: KernelSU memodifikasi `kernel` dan Magisk memodifikasi `ramdisk`, keduanya dapat bekerja bersama.

## Akankah KernelSU menggantikan Magisk?

Kami rasa tidak dan itu bukan tujuan kami. Magisk sudah cukup baik untuk solusi root userspace dan akan bertahan lama. Tujuan KernelSU adalah untuk menyediakan antarmuka kernel kepada pengguna, bukan untuk menggantikan Magisk.

## Dapatkah KernelSU mendukung perangkat non GKI?

Hal ini dimungkinkan. Tetapi Anda harus mengunduh sumber kernel dan mengintegrasikan KernelSU ke source tree dan mengkompilasi kernel sendiri.

## Dapatkah KernelSU mendukung perangkat di bawah Android 12?

Kernel perangkatlah yang mempengaruhi kompatibilitas KernelSU dan tidak ada hubungannya dengan versi Android, satu-satunya batasan adalah bahwa perangkat yang diluncurkan dengan Android 12 harus menggunakan kernel 5.10+ (perangkat GKI). Jadi:

1. Perangkat yang diluncurkan dengan Android 12 harus didukung.
2. Perangkat dengan kernel lama (Beberapa perangkat Android 12 juga memiliki kernel lama) dapat dikompilasi (Anda harus membuat kernel sendiri)

## Dapatkah KernelSU mendukung kernel lama?

Ada kemungkinan, KernelSU sudah di-backport ke kernel 4.14 sekarang, untuk kernel yang lebih lama, Anda harus melakukan backport secara manual dan menyambut baik PR darimu!

## Cara mengintegrasikan KernelSU untuk kernel lama?

Silakan merujuk ke [guide](how-to-integrate-for-non-gki)

## Mengapa ada file 512G yang besar?

`modules.img` berukuran 512G adalah file gambar disk, **jangan khawatir tentang ukurannya**, ini adalah jenis file khusus yang dikenal sebagai [file jarang](https://en.wikipedia.org/ wiki/Sparse_file), ukuran sebenarnya hanya sebesar modul yang Anda gunakan, dan akan menyusut secara dinamis setelah Anda menghapus modul; itu sebenarnya tidak menempati ruang disk 512G (sebenarnya ponsel Anda mungkin tidak memiliki banyak ruang).

Jika Anda benar-benar tidak puas dengan ukuran file ini, Anda dapat menggunakan perintah `resize2fs -M` untuk menjadikannya ukuran sebenarnya; namun modul tersebut mungkin tidak berfungsi dengan baik saat ini, dan kami tidak akan memberikan dukungan apa pun untuk hal ini.
