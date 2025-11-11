# 📝 ArUco Detection with Logging & Notification

## ✨ FITUR BARU YANG DITAMBAHKAN

### 1. 🗃️ **Automatic Logging ke File**

- Setiap ArUco marker yang terdeteksi otomatis dicatat ke file
- File log: `aruco_detection_log.txt`
- Informasi yang disimpan:
  - ⏰ Timestamp (tanggal & waktu)
  - 🏷️ Marker ID
  - 📍 Posisi center (x, y)
  - 📏 Ukuran (pixels)
  - ✅ Status deteksi

### 2. 🎯 **Modal Box Notification**

- Pop-up notification muncul saat ArUco terdeteksi
- Tampilan modal box:
  - Header "ARUCO DETECTED!"
  - Icon checkmark ✓
  - Marker ID
  - Position
  - Size
  - Timestamp
- Modal tampil selama ~3 detik
- Multiple markers = multiple modals

### 3. 🚫 **Smart Detection (Anti-Spam)**

- Cooldown 3 detik per marker
- Marker yang sama tidak di-log berulang kali dalam waktu singkat
- Menghindari spam di log file
- Tracking marker yang masih terlihat vs marker baru

### 4. 📊 **Log Viewer Tool**

- Script Python untuk analisis log
- Statistik per marker ID
- Timeline deteksi
- Search by ID

---

## 🚀 CARA MENGGUNAKAN

### Jalankan Program

```bash
cd /home/icad/Downloads/magang/build
./aruco_detection
```

### Kontrol Keyboard

| Tombol       | Fungsi                        |
| ------------ | ----------------------------- |
| `q` atau `0` | Keluar program                |
| `s`          | Screenshot dengan timestamp   |
| `g`          | Generate 10 ArUco markers     |
| `l`          | Lihat summary log di terminal |

### Lihat Log File

**Opsi 1: Baca file langsung**

```bash
cat aruco_detection_log.txt
```

**Opsi 2: Menggunakan Log Viewer (Recommended)**

```bash
# Summary statistik
python3 view_log.py

# Detail log (20 terakhir)
python3 view_log.py --detail

# Semua log detail
python3 view_log.py --all

# Cari berdasarkan Marker ID
python3 view_log.py --id 5

# Help
python3 view_log.py --help
```

---

## 📄 FORMAT LOG FILE

```
================================================
Timestamp      : 2025-11-06 14:30:45
Marker ID      : 5
Center Position: (320, 240)
Size (pixels)  : 150 px
Status         : DETECTED
================================================
```

### Log Session

Setiap kali program dijalankan, session baru dicatat:

```
####################################################
# NEW SESSION STARTED
# Date/Time: 2025-11-06 14:30:00
# Camera: IP Camera: http://10.237.86.79:8080/video
####################################################
```

---

## 🎨 MODAL NOTIFICATION PREVIEW

```
╔══════════════════════════════════════════╗
║         ✓  ARUCO DETECTED!               ║
╠══════════════════════════════════════════╣
║                                          ║
║  Marker ID: 5                            ║
║  Position: (320, 240)                    ║
║  Size: 150 pixels                        ║
║  Time: 2025-11-06 14:30:45               ║
║                                          ║
╚══════════════════════════════════════════╝
```

**Karakteristik Modal:**

- ✅ Background putih semi-transparan
- ✅ Border hijau tebal
- ✅ Header hijau dengan checkmark
- ✅ Informasi lengkap marker
- ✅ Auto-hide setelah 3 detik
- ✅ Multiple modals untuk multiple markers

---

## 📊 CONTOH OUTPUT LOG VIEWER

### Summary Mode

```bash
$ python3 view_log.py

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

🏷️  Marker ID: 5
   Jumlah deteksi: 15
   Pertama kali: 2025-11-06 14:35:00
   Terakhir kali: 2025-11-06 14:45:30
   Rata-rata ukuran: 160.2 px
```

### Detail Mode

```bash
$ python3 view_log.py --detail

======================================================================
                           DETAILED LOG
======================================================================

Menampilkan 20 dari 45 deteksi terakhir:

#1
  Timestamp   : 2025-11-06 14:30:45
  Marker ID   : 5
  Position    : (320, 240)
  Size        : 150 px
  Status      : DETECTED
```

### Search by ID

```bash
$ python3 view_log.py --id 5

======================================================================
DETEKSI UNTUK MARKER ID 5
======================================================================

Total deteksi: 15

#1 - 2025-11-06 14:35:00
     Position: (320, 240) | Size: 150 px

#2 - 2025-11-06 14:35:10
     Position: (315, 238) | Size: 148 px
```

---

## 🔍 MONITORING REAL-TIME

### Console Output saat Deteksi Baru

```
🎯 NEW DETECTION!
   Marker ID: 5
   Position: (320, 240)
   Size: 150 px
   Time: 2025-11-06 14:30:45
   ✓ Logged to file: aruco_detection_log.txt
```

### Check Summary dalam Program

Tekan `l` saat program running:

```
==================================
DETECTION LOG SUMMARY
==================================
Log file: aruco_detection_log.txt
Total markers tracked: 5

Markers detected in this session:
  - Marker ID 0 (last seen: Wed Nov  6 14:32:15 2025)
  - Marker ID 5 (last seen: Wed Nov  6 14:45:30 2025)
  - Marker ID 12 (last seen: Wed Nov  6 14:40:20 2025)

Buka file 'aruco_detection_log.txt' untuk detail lengkap
==================================
```

---

## 📈 USE CASES

### 1. **Tracking Kehadiran**

- Setiap orang punya ArUco marker
- Scan marker saat masuk/keluar
- Log mencatat waktu deteksi
- Analisis dengan log viewer

### 2. **Inventory Management**

- Setiap barang punya ArUco marker
- Scan untuk tracking lokasi
- Log history pergerakan barang

### 3. **Event Check-in**

- Peserta scan marker saat registrasi
- Auto-log dengan timestamp
- Real-time notification

### 4. **Robot Navigation**

- ArUco sebagai waypoint
- Log posisi robot
- Tracking path yang dilalui

---

## ⚙️ KUSTOMISASI

### Ubah Cooldown Detection

Edit `aruco_detection.cpp` line ~218:

```cpp
int detectionCooldown = 3; // Ganti dengan detik yang diinginkan
```

### Ubah Durasi Modal

Edit `aruco_detection.cpp` line ~222:

```cpp
int modalDuration = 90; // Ganti dengan jumlah frame (90 = 3 detik @ 30fps)
```

### Ubah Nama Log File

Edit `aruco_detection.cpp` line ~209:

```cpp
std::string logFilename = "my_custom_log.txt";
```

Setelah edit, compile ulang:

```bash
cd build && make
```

---

## 📦 FILE YANG DIHASILKAN

| File                             | Deskripsi                     |
| -------------------------------- | ----------------------------- |
| `aruco_detection_log.txt`        | Log deteksi utama             |
| `aruco_screenshot_TIMESTAMP.jpg` | Screenshot (tekan 's')        |
| `aruco_marker_X.png`             | Generated markers (tekan 'g') |

---

## 🆘 TROUBLESHOOTING

### Log file tidak terbuat

- ✅ Cek permission folder
- ✅ Pastikan program berjalan dengan benar
- ✅ Deteksi minimal 1 marker

### Modal tidak muncul

- ✅ Pastikan compile berhasil
- ✅ Cek apakah marker benar-benar terdeteksi (ada outline hijau)
- ✅ Modal mungkin sudah timeout (3 detik)

### Log viewer error

- ✅ Install Python 3
- ✅ Pastikan file log ada
- ✅ Cek format command: `python3 view_log.py`

---

## 🎉 DEMO FLOW

1. **Jalankan program:**

   ```bash
   ./aruco_detection
   ```

2. **Tunjukkan ArUco marker ke kamera**

3. **Lihat notifikasi:**

   - ✅ Modal box muncul di layar
   - ✅ Console print "NEW DETECTION!"
   - ✅ Auto-save ke log file

4. **Check log (dalam program):**

   - Tekan `l` untuk summary

5. **Analisis log (setelah selesai):**

   ```bash
   python3 view_log.py
   ```

6. **Export atau share log:**
   ```bash
   cp aruco_detection_log.txt backup_log_$(date +%Y%m%d).txt
   ```

---

## 📚 NEXT LEVEL FEATURES (Coming Soon?)

- 📧 Email notification saat deteksi
- 🔊 Sound alert
- 📱 Push notification ke HP
- 💾 Database integration (SQLite)
- 📊 Web dashboard
- 🎥 Auto-record video saat deteksi
- 🤖 ML untuk prediksi marker berikutnya

---

**Program sudah siap digunakan dengan fitur logging dan notification lengkap!** 🚀
