# 🎯 ArUco Detection System - SIAP DIGUNAKAN!

## ✅ Program Sudah Berhasil Dibuat!

**File Program:**

- 📁 Source: `src/aruco_detection.cpp`
- 🔨 Executable: `build/aruco_detection`
- 📜 Script Helper: `run_aruco.sh`
- 🐍 Generator: `generate_aruco.py`
- 🖼️ Markers: `aruco_markers/` (20 markers)

---

## 🚀 CARA CEPAT MENGGUNAKAN

### Opsi 1: Script Otomatis (PALING MUDAH)

```bash
./run_aruco.sh
```

Pilih menu yang muncul!

### Opsi 2: Manual

```bash
cd build

# Dengan IP Camera (default)
./aruco_detection

# Dengan webcam laptop
./aruco_detection 0

# Dengan custom URL
./aruco_detection http://192.168.1.100:8080/video
```

---

## 📱 Setup IP Camera HP

1. **Download aplikasi "IP Webcam"** dari Play Store
2. Buka aplikasi → tekan **"Start Server"**
3. Lihat URL di layar HP (contoh: `http://10.237.86.79:8080/video`)
4. **Edit URL di program:**
   - Buka file `src/aruco_detection.cpp`
   - Line 43: ganti URL dengan URL dari HP Anda
   - Compile ulang: `cd build && make`

---

## 🎨 ArUco Markers Sudah Siap!

✅ **20 markers sudah di-generate** di folder `aruco_markers/`

**Cara pakai:**

1. Buka folder `aruco_markers/`
2. Pilih marker (contoh: `aruco_marker_00.png` sampai `aruco_marker_19.png`)
3. Print marker tersebut
4. Tunjukkan ke kamera
5. Program akan detect otomatis!

**Generate lebih banyak markers:**

```bash
python3 generate_aruco.py
```

---

## ⌨️ Kontrol Saat Program Berjalan

| Tombol       | Fungsi                   |
| ------------ | ------------------------ |
| `q` atau `0` | Keluar program           |
| `s`          | Screenshot               |
| `g`          | Generate 10 markers baru |

---

## 🔍 Apa yang Akan Terdeteksi?

Program akan menampilkan:

- ✅ **Kotak hijau** mengelilingi marker
- ✅ **ID marker** (angka 0-49)
- ✅ **Size** (ukuran dalam pixel)
- ✅ **Titik center** (koordinat pusat)
- ✅ **Counter** jumlah marker terdeteksi

---

## 💡 Tips Agar Deteksi Maksimal

1. **Print marker dengan kualitas baik:**

   - Kertas putih bersih
   - Tinta hitam pekat (bukan abu-abu)
   - Minimal ukuran 5cm × 5cm

2. **Pencahayaan:**

   - Ruangan cukup terang
   - Tidak ada bayangan di marker
   - Hindari silau/glare

3. **Posisi marker:**

   - Jangan terlalu jauh dari kamera
   - Jangan terlipat atau rusak
   - Tempelkan di permukaan rata

4. **Kamera:**
   - Fokus (tidak blur)
   - Stabil (tidak goyang)
   - Arahkan tegak lurus ke marker

---

## 🆘 Troubleshooting

### "Error: Tidak dapat membuka kamera"

```bash
# Cek kamera tersedia
ls /dev/video*

# Test dengan webcam
./aruco_detection 0
```

### "Tidak ada ArUco terdeteksi"

- ✅ Cek marker sudah dicetak dengan benar
- ✅ Marker tidak blur/rusak
- ✅ Pencahayaan cukup
- ✅ Marker cukup besar di frame

### IP Camera tidak connect

- ✅ HP dan PC di WiFi yang sama
- ✅ URL sudah benar
- ✅ Server di HP sudah aktif

---

## 📊 Informasi Teknis

**Dictionary:** DICT_4X4_50

- Grid: 4×4 bit
- Total markers: 50 (ID: 0-49)
- Cocok untuk tracking dan AR

**OpenCV Version:** 4.x
**Module:** opencv_aruco
**Language:** C++11

---

## 📖 Dokumentasi Lengkap

Baca file: **README_ARUCO.md** untuk info lebih detail

---

## 🎬 Quick Start Example

```bash
# 1. Generate markers
python3 generate_aruco.py

# 2. Print marker dari folder aruco_markers/

# 3. Jalankan program
./run_aruco.sh

# 4. Pilih opsi 1 (IP Camera) atau 2 (Webcam)

# 5. Tunjukkan marker ke kamera

# 6. Lihat magic! 🎉
```

---

## ✨ Selamat Mencoba!

**Program sudah 100% siap digunakan!**

Jika ada masalah, cek:

1. README_ARUCO.md (dokumentasi lengkap)
2. Output di terminal (pesan error)
3. Test dengan webcam dulu (./aruco_detection 0)

**Enjoy! 🚀**
