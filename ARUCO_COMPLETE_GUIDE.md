# 🎯 ArUco Detection System - PANDUAN LENGKAP

## ✅ FITUR YANG SUDAH DIBUAT

### 1. 🎥 **ArUco Detection Real-time**

- Deteksi ArUco markers dari kamera
- Support IP Camera (dari HP)
- Support Webcam laptop
- Dictionary: DICT_4X4_50 (ID: 0-49)

### 2. 📝 **Automatic Logging System**

- ✅ Auto-save setiap deteksi ke file `aruco_detection_log.txt`
- ✅ Informasi lengkap: Timestamp, ID, Position, Size
- ✅ Smart detection dengan cooldown 3 detik (anti-spam)
- ✅ Session tracking

### 3. 🎨 **Modal Box Notification**

- ✅ Pop-up notification saat ArUco terdeteksi
- ✅ Tampilan modal yang jelas dan menarik
- ✅ Informasi: ID, Position, Size, Timestamp
- ✅ Auto-hide setelah 3 detik
- ✅ Support multiple markers

### 4. 📊 **Log Viewer & Analytics**

- ✅ Python script untuk analisis log
- ✅ Summary statistik per marker
- ✅ Detailed log viewer
- ✅ Search by Marker ID

### 5. 🛠️ **Helper Tools**

- ✅ Interactive menu script
- ✅ ArUco marker generator
- ✅ Log backup & clear

---

## 🚀 QUICK START

### Cara Tercepat (Menggunakan Menu Interaktif)

```bash
cd /home/icad/Downloads/magang
./aruco_menu.sh
```

**Menu yang tersedia:**

1. Run dengan IP Camera
2. Run dengan Webcam
3. Run dengan Custom URL
4. View Log Summary
5. View Log Detail
6. Search by Marker ID
7. Generate Markers
8. Clear Log
9. Exit

---

## 📱 SETUP IP CAMERA

### Langkah-langkah:

1. **Install Aplikasi di HP:**

   - Android: "IP Webcam" (Play Store)
   - iOS: "IP Camera Lite"

2. **Aktifkan Server:**

   - Buka aplikasi → Start Server
   - Catat URL (contoh: `http://10.237.86.79:8080/video`)

3. **Update URL di Program:**

   ```bash
   # Edit file source
   nano src/aruco_detection.cpp

   # Cari line 16, ganti URL:
   std::string ipCameraURL = "http://YOUR_IP:8080/video";

   # Compile ulang
   cd build && make
   ```

4. **Jalankan:**
   ```bash
   ./aruco_detection
   ```

---

## ⌨️ KONTROL KEYBOARD

Saat program berjalan:

| Tombol           | Fungsi                         |
| ---------------- | ------------------------------ |
| **q** atau **0** | Keluar dari program            |
| **s**            | Screenshot dengan timestamp    |
| **l**            | Lihat log summary di terminal  |
| **g**            | Generate 10 ArUco markers baru |

---

## 📝 SISTEM LOGGING

### Format Log File

Setiap deteksi dicatat dengan format:

```
================================================
Timestamp      : 2025-11-06 14:30:45
Marker ID      : 5
Center Position: (320, 240)
Size (pixels)  : 150 px
Status         : DETECTED
================================================
```

### Lokasi File Log

- **File:** `aruco_detection_log.txt`
- **Location:** `/home/icad/Downloads/magang/`

### Smart Detection

- **Cooldown:** 3 detik per marker
- **Benefit:** Menghindari spam log untuk marker yang sama
- **Tracking:** Marker yang terlihat terus vs marker baru

---

## 🎨 MODAL NOTIFICATION

### Kapan Muncul?

- ✅ Pertama kali marker terdeteksi
- ✅ Setelah cooldown 3 detik (jika marker hilang lalu muncul lagi)

### Apa yang Ditampilkan?

- ✓ Icon checkmark
- Marker ID
- Position (x, y)
- Size (pixels)
- Timestamp

### Durasi

- Modal tampil selama **~3 detik** (90 frames @ 30fps)
- Lalu auto-hide

### Multiple Markers

- Jika 3 marker terdeteksi bersamaan → 3 modal muncul
- Masing-masing punya timer sendiri

---

## 📊 LOG VIEWER

### Command Line Tool

```bash
# 1. Summary (statistik per marker)
python3 view_log.py

# 2. Detail (20 deteksi terakhir)
python3 view_log.py --detail

# 3. Semua deteksi
python3 view_log.py --all

# 4. Cari by Marker ID
python3 view_log.py --id 5

# 5. Help
python3 view_log.py --help
```

### Contoh Output Summary

```
======================================================================
                    ARUCO DETECTION LOG SUMMARY
======================================================================

📊 Total Deteksi: 45
🎯 Total Marker Unik: 5
⏰ Deteksi Pertama: 2025-11-06 14:30:00
⏰ Deteksi Terakhir: 2025-11-06 14:45:30

----------------------------------------------------------------------
DETAIL PER MARKER ID
----------------------------------------------------------------------

🏷️  Marker ID: 0
   Jumlah deteksi: 10
   Pertama kali: 2025-11-06 14:30:00
   Terakhir kali: 2025-11-06 14:32:15
   Rata-rata ukuran: 145.5 px
```

---

## 🎨 GENERATE ARUCO MARKERS

### Metode 1: Dalam Program

1. Jalankan `./aruco_detection`
2. Tekan tombol **`g`**
3. 10 markers (ID 0-9) akan di-generate
4. File: `aruco_marker_0.png` sampai `aruco_marker_9.png`

### Metode 2: Python Script

```bash
python3 generate_aruco.py
```

- Generate 20 markers (ID 0-19)
- Dengan border dan label
- Folder: `aruco_markers/`

### Print Markers

- **Ukuran minimal:** 5cm × 5cm
- **Kertas:** Putih bersih
- **Tinta:** Hitam pekat
- **Tips:** Laminating untuk durability

---

## 🎬 DEMO WORKFLOW

### Skenario: Tracking Kehadiran

1. **Persiapan:**

   ```bash
   # Generate markers untuk setiap peserta
   python3 generate_aruco.py

   # Print marker ID 0-19
   # Berikan 1 marker per peserta
   ```

2. **Jalankan System:**

   ```bash
   ./aruco_menu.sh
   # Pilih opsi 1 (IP Camera)
   ```

3. **Check-in Process:**

   - Peserta tunjukkan marker ke kamera
   - 🎯 Modal notification muncul: "ARUCO DETECTED!"
   - 📝 Auto-log ke file dengan timestamp
   - Console print: "NEW DETECTION!"

4. **Monitoring:**

   ```bash
   # Tekan 'l' dalam program untuk quick summary
   # Atau
   python3 view_log.py
   ```

5. **Export Report:**

   ```bash
   # Backup log
   cp aruco_detection_log.txt kehadiran_$(date +%Y%m%d).txt

   # Lihat statistik
   python3 view_log.py > report.txt
   ```

---

## 🔧 KUSTOMISASI

### 1. Ubah Cooldown Detection

Edit `src/aruco_detection.cpp` sekitar line 190:

```cpp
int detectionCooldown = 5; // Ganti 3 jadi 5 detik
```

### 2. Ubah Durasi Modal

Edit `src/aruco_detection.cpp` sekitar line 194:

```cpp
int modalDuration = 150; // 150 frames = 5 detik @ 30fps
```

### 3. Ubah Dictionary ArUco

Edit `src/aruco_detection.cpp` sekitar line 172:

```cpp
// Ganti DICT_4X4_50 dengan yang lain:
// DICT_4X4_100, DICT_5X5_50, DICT_6X6_250
cv::Ptr<cv::aruco::Dictionary> dictionary =
    cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
```

### 4. Ubah Nama Log File

Edit `src/aruco_detection.cpp` sekitar line 182:

```cpp
std::string logFilename = "my_custom_log.txt";
```

**Setelah edit, compile ulang:**

```bash
cd build && make
```

---

## 📂 STRUKTUR FILE

```
/home/icad/Downloads/magang/
├── src/
│   ├── aruco_detection.cpp          # Program utama ✨
│   ├── cobaa.cpp                    # Program deteksi hitam
│   └── face_blur_detection.cpp      # Program face blur
├── build/
│   ├── aruco_detection              # Executable ⭐
│   ├── cobaAja_kj
│   └── face_blur_detection
├── aruco_markers/                   # Generated markers
│   ├── aruco_marker_00.png
│   ├── aruco_marker_01.png
│   └── ...
├── aruco_detection_log.txt          # Log file 📝
├── aruco_menu.sh                    # Menu interaktif 🎯
├── generate_aruco.py                # Generator markers
├── view_log.py                      # Log viewer
├── run_aruco.sh                     # Quick launcher
├── CMakeLists.txt
├── README_ARUCO.md
├── LOGGING_FEATURE.md
└── ARUCO_QUICK_START.md
```

---

## 🆘 TROUBLESHOOTING

### ❌ Program tidak bisa compile

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install libopencv-dev libopencv-contrib-dev

# Clean build
cd build
rm -rf *
cmake ..
make
```

### ❌ IP Camera tidak connect

- ✅ HP dan PC di WiFi yang sama?
- ✅ Firewall tidak block?
- ✅ URL sudah benar?
- ✅ Server di HP sudah aktif?

**Test:**

```bash
# Test dengan webcam dulu
./aruco_detection 0
```

### ❌ ArUco tidak terdeteksi

- ✅ Marker dicetak dengan jelas?
- ✅ Pencahayaan cukup?
- ✅ Kamera fokus (tidak blur)?
- ✅ Marker cukup besar di frame?
- ✅ Marker tidak rusak/terlipat?

### ❌ Modal tidak muncul

- ✅ Compile berhasil tanpa error?
- ✅ Marker benar-benar terdeteksi? (ada outline hijau)
- ✅ Sudah pertama kali deteksi? (cooldown 3 detik)

### ❌ Log file tidak terbuat

- ✅ Permission folder OK?
- ✅ Minimal 1 marker sudah terdeteksi?
- ✅ Program berjalan tanpa error?

### ❌ Log viewer error

```bash
# Cek Python 3 installed
python3 --version

# Cek log file ada
ls -la aruco_detection_log.txt

# Test viewer
python3 view_log.py --help
```

---

## 💡 TIPS & BEST PRACTICES

### Untuk Deteksi Optimal:

1. **Pencahayaan:**

   - Cahaya merata, tidak terlalu terang/gelap
   - Hindari bayangan di marker
   - Tidak ada silau/glare

2. **Print Quality:**

   - Printer laser lebih baik dari inkjet
   - Kertas putih 80gsm atau lebih
   - Tinta hitam pekat (bukan abu-abu)
   - Laminating untuk outdoor use

3. **Positioning:**

   - Jarak ideal: 30cm - 100cm dari kamera
   - Marker tegak lurus ke kamera
   - Tidak miring >45 derajat
   - Tidak tertutup objek lain

4. **Multiple Markers:**
   - Jarak antar marker >10cm
   - Tidak overlap
   - Jangan lebih dari 5 marker sekaligus

### Untuk Logging:

1. **Backup Regular:**

   ```bash
   # Daily backup
   cp aruco_detection_log.txt backup_$(date +%Y%m%d).txt
   ```

2. **Clear Old Logs:**

   ```bash
   # Setiap awal bulan
   ./aruco_menu.sh
   # Pilih opsi 8 (Clear Log)
   ```

3. **Export ke Excel:**
   ```bash
   # Convert log ke CSV
   python3 view_log.py --all > data.csv
   # Import ke Excel/Google Sheets
   ```

---

## 🎓 USE CASES

### 1. **Event Management**

- Check-in peserta dengan ArUco
- Tracking waktu kehadiran
- Generate report otomatis

### 2. **Warehouse/Inventory**

- Tag barang dengan ArUco
- Scan untuk update lokasi
- History tracking

### 3. **Access Control**

- ID card dengan ArUco
- Log masuk/keluar
- Real-time monitoring

### 4. **Education**

- Absensi mahasiswa
- Lab equipment tracking
- Library book check-in/out

### 5. **Augmented Reality**

- AR marker positioning
- Virtual object overlay
- Interactive exhibits

---

## 📈 STATISTIK & ANALISIS

### Metrics yang Bisa Ditracking:

1. **Frequency:** Berapa kali marker terdeteksi
2. **Duration:** Berapa lama marker terlihat
3. **Position:** Di mana marker biasa muncul
4. **Size:** Jarak marker dari kamera
5. **Timing:** Kapan marker paling sering muncul

### Custom Analysis dengan Python:

```python
# Baca log file
with open('aruco_detection_log.txt', 'r') as f:
    data = f.read()

# Parse data
# Lakukan analisis custom
# Export ke chart/graph
```

---

## 🔮 FUTURE ENHANCEMENTS

Ide untuk pengembangan selanjutnya:

- [ ] 📧 Email notification
- [ ] 🔊 Sound alert
- [ ] 💾 SQLite database
- [ ] 🌐 Web dashboard
- [ ] 📱 Mobile app integration
- [ ] 🎥 Auto-record video
- [ ] 🤖 ML prediction
- [ ] 📊 Real-time analytics dashboard
- [ ] 🔐 Authentication system
- [ ] ☁️ Cloud sync

---

## 📞 SUPPORT

Jika ada masalah:

1. Cek dokumentasi ini
2. Lihat LOGGING_FEATURE.md untuk detail
3. Cek README_ARUCO.md untuk basics
4. Test dengan webcam dulu sebelum IP cam
5. Pastikan marker dicetak dengan benar

---

## ✨ KESIMPULAN

**Program ArUco Detection System sudah 100% siap digunakan dengan fitur:**

✅ Real-time detection  
✅ Automatic logging  
✅ Modal notifications  
✅ Smart anti-spam  
✅ Log analytics  
✅ Multiple tools  
✅ Full documentation

**Selamat menggunakan! 🚀**

---

**Created by:** ICAD Team  
**Date:** November 6, 2025  
**Version:** 2.0 (with Logging & Notification)
