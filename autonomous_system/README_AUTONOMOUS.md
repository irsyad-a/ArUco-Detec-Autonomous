# 🤖 SEMI-AUTONOMOUS ROBOT NAVIGATION SYSTEM

## 📋 Deskripsi

Sistem navigasi semi-autonomous yang menggunakan **ArUco markers** untuk memandu robot/operator secara cerdas. Sistem ini mensimulasikan robot autonomous dengan memberikan instruksi navigasi real-time kepada operator (Anda) berdasarkan deteksi ArUco markers.

### ✨ Fitur Utama

1. **🔍 Auto-Scanning Mode** - Sistem secara otomatis mencari ArUco markers di lingkungan
2. **🧭 Smart Navigation** - Memberikan instruksi arah yang jelas (kiri, kanan, maju, mundur)
3. **📱 IP Camera Support** - Gunakan HP sebagai kamera robot
4. **🎯 Target Tracking** - Mengunci target marker dan memandu untuk mendekati
5. **📊 Dual Display** - Window terpisah untuk camera view dan navigation panel
6. **📝 Logging** - Mencatat semua aktivitas navigasi dan scanning
7. **🎨 Visual Feedback** - Indikator visual yang jelas untuk setiap status

---

## 🚀 Cara Menggunakan

### 1️⃣ Quick Start

**Metode Termudah - Menggunakan Script Launcher:**

```bash
cd /home/icad/Downloads/magang
./run_autonomous.sh
```

Script akan memberikan menu interaktif untuk memilih sumber kamera.

### 2️⃣ Manual Start

**Dengan IP Camera (Default):**

```bash
cd /home/icad/Downloads/magang/build
./autonomous_aruco
```

**Dengan Webcam:**

```bash
./autonomous_aruco 0
```

**Dengan Custom IP Camera:**

```bash
./autonomous_aruco http://192.168.1.100:8080/video
```

---

## 🎮 Cara Kerja Sistem

### State Machine

Sistem bekerja dengan 5 state berbeda:

| State | Warna | Deskripsi | Instruksi |
|-------|-------|-----------|-----------|
| **SCANNING** | 🟠 Orange | Mencari marker di lingkungan | "PUTAR KIRI - Scan lingkungan" |
| **TRACKING** | 🟡 Yellow | Mengikuti marker yang terdeteksi | "PUTAR KIRI 15°" / "PUTAR KANAN 20°" |
| **APPROACHING** | 🔵 Cyan | Mendekati marker | "MAJU 15 cm" |
| **LOCKED** | 🟢 Green | Target locked, siap scan | "LOCKED! - Tekan 'SCAN' (S)" |
| **COMPLETE** | 🟣 Magenta | Semua marker selesai | "SCAN COMPLETE" |

### Alur Kerja

```
START
  ↓
[SCANNING] ← Putar untuk mencari marker
  ↓ (marker terdeteksi)
[TRACKING] ← Adjust posisi (kiri/kanan)
  ↓ (centered)
[APPROACHING] ← Maju untuk mendekati
  ↓ (jarak optimal)
[LOCKED] ← Tekan 'S' untuk scan
  ↓ (scan selesai)
[SCANNING] ← Cari marker berikutnya
```

---

## 🖥️ Interface

### Window 1: Camera View

Menampilkan:
- ✅ Feed kamera real-time
- 🎯 Crosshair center (sebagai target)
- 📍 Marker detection overlay
- 🔗 Line connector ke target marker
- 📊 Status bar (jumlah marker, state)

### Window 2: Navigation Panel

Menampilkan:
- 🚦 Status robot (SCANNING/TRACKING/dll)
- 📢 **Instruksi navigasi besar** (mudah dibaca)
- 📈 Statistik:
  - Target ID saat ini
  - Total markers ditemukan
  - Total markers di-scan
- 🎮 Kontrol keyboard

---

## ⌨️ Kontrol Keyboard

| Tombol | Fungsi |
|--------|--------|
| `S` | **Scan marker** (saat LOCKED state) |
| `R` | **Reset target** (pilih target baru) |
| `Q` | **Keluar** dari program |
| `ESC` | **Keluar** dari program |

---

## 📱 Setup IP Camera HP

### Android: IP Webcam

1. **Download & Install:**
   - Buka Play Store
   - Cari "IP Webcam" oleh Pavel Khlebovich
   - Install aplikasi

2. **Konfigurasi:**
   - Buka aplikasi IP Webcam
   - Scroll ke bawah
   - Tekan tombol **"Start server"**
   - Catat URL yang muncul di bagian bawah layar
   - Contoh: `http://10.237.86.79:8080/video`

3. **Update Program:**
   ```cpp
   // Edit file src/autonomous_aruco.cpp line ~528
   std::string ipCameraURL = "http://10.237.86.79:8080/video";
   ```
   
   Ganti dengan IP yang didapat dari HP Anda, lalu compile ulang:
   ```bash
   cd build && make autonomous_aruco
   ```

4. **Alternatif: Tanpa Edit Kode**
   ```bash
   ./autonomous_aruco "http://IP_ANDA:8080/video"
   ```

### iOS: IP Camera Lite

1. Download "IP Camera Lite" dari App Store
2. Buka app dan tekan "Start"
3. Catat URL (biasanya format: `http://IP:8080/live`)
4. Gunakan URL tersebut saat menjalankan program

### Tips Koneksi:

- ✅ HP dan laptop harus di **WiFi yang sama**
- ✅ Matikan **Mobile Data** di HP (gunakan WiFi saja)
- ✅ Cek firewall tidak memblokir koneksi
- ✅ Pastikan tidak ada VPN aktif
- ✅ Test URL di browser dulu: buka `http://IP:8080/video` di laptop

---

## 🎯 Logika Navigasi

### 1. Target Selection

Sistem memilih marker berdasarkan:
- **Prioritas 1:** Marker yang belum di-scan
- **Prioritas 2:** Marker paling dekat ke center frame
- **Prioritas 3:** Marker dengan ukuran terbesar (paling dekat)

### 2. Centering Logic

```
Offset X > 80px → "PUTAR KANAN"
Offset X < -80px → "PUTAR KIRI"
```

Sistem menghitung jarak marker dari center frame dan memberikan instruksi.

### 3. Distance Estimation

```
Size > 120px → Cukup dekat untuk scan
Size < 120px → Perlu maju
```

Ukuran marker di frame digunakan untuk estimasi jarak.

### 4. Instruction Format

```
"PUTAR KIRI 15°"   → Sudut disesuaikan dengan offset
"MAJU 10 cm"       → Jarak disesuaikan dengan size marker
"LOCKED!"          → Siap untuk scan
```

---

## 📊 Parameter Tuning

Anda bisa adjust parameter di `src/autonomous_aruco.cpp`:

```cpp
// Threshold untuk "centered" (pixels)
float centerThreshold = 80.0f;

// Ukuran minimum marker untuk scan (pixels)
float minMarkerSize = 120.0f;

// Jarak optimal untuk scanning
float approachDistance = 180.0f;
```

**Rekomendasi:**
- **centerThreshold:** 60-100px (lebih kecil = lebih presisi)
- **minMarkerSize:** 100-150px (tergantung ukuran marker fisik)
- **approachDistance:** 150-200px (tergantung kualitas kamera)

Setelah edit, compile ulang:
```bash
cd build && make autonomous_aruco
```

---

## 📝 Logging System

Sistem akan membuat file log: `build/autonomous_navigation_log.txt`

### Format Log:

```
=== NEW SESSION ===
Start Time: 2025-11-06 19:30:00
Camera: http://10.237.86.79:8080/video

========================================
SCAN EVENT
Timestamp: 2025-11-06 19:31:15
Marker ID: 5
Position: (320, 240)
Size: 145 px
Status: SCANNED
========================================

=== SESSION END ===
End Time: 2025-11-06 19:35:00
Total Markers Found: 8
Total Markers Scanned: 5
```

### Lihat Log:

```bash
# Lihat seluruh log
cat build/autonomous_navigation_log.txt

# Lihat 50 baris terakhir
tail -n 50 build/autonomous_navigation_log.txt

# Atau gunakan Python viewer
python3 view_log.py
```

---

## 🔧 Troubleshooting

### ❌ Program tidak bisa connect ke IP Camera

**Solusi:**

1. **Cek koneksi:**
   ```bash
   ping IP_HP_ANDA
   ```
   Pastikan HP bisa di-ping.

2. **Test URL di browser:**
   Buka `http://IP_HP:8080/video` di browser laptop.
   Harusnya muncul video stream.

3. **Coba URL alternatif:**
   - `/video` → `/videofeed`
   - `/video` → `/live`
   - Port `8080` → `4747` (untuk DroidCam)

4. **Restart aplikasi di HP**

### ❌ Marker tidak terdeteksi

**Solusi:**

1. **Print marker dengan benar:**
   - Minimal ukuran 5x5 cm
   - Kertas putih bersih
   - Tinta hitam pekat
   - Tidak terlipat/rusak

2. **Generate marker:**
   ```bash
   cd /home/icad/Downloads/magang
   python3 generate_aruco.py
   ```
   Atau tekan `G` saat program running.

3. **Pencahayaan:**
   - Pastikan ruangan cukup terang
   - Hindari bayangan pada marker
   - Hindari backlight

4. **Fokus kamera:**
   - Pastikan kamera fokus (tidak blur)
   - Jarak 20-200 cm dari marker
   - Marker menghadap kamera

### ❌ Instruksi tidak akurat

**Solusi:**

1. **Kalibrasi parameter:**
   Edit threshold di `src/autonomous_aruco.cpp`:
   ```cpp
   float centerThreshold = 60.0f;  // Lebih presisi
   float minMarkerSize = 130.0f;   // Lebih dekat
   ```

2. **Stabilkan kamera:**
   - Gunakan tripod/holder untuk HP
   - Hindari gerakan tiba-tiba
   - Gerakan smooth saat mengikuti instruksi

### ❌ Compile error

**Solusi:**

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y libopencv-dev libopencv-contrib-dev cmake g++

# Clean build
cd /home/icad/Downloads/magang/build
rm -rf *
cmake ..
make autonomous_aruco
```

---

## 🎓 Tips Penggunaan

### Untuk Testing Optimal:

1. **Setup Lingkungan:**
   - Letakkan 5-10 ArUco markers di sekitar ruangan
   - Berikan jarak 1-2 meter antar marker
   - Tempelkan di dinding/papan setinggi mata

2. **Posisi Awal:**
   - Berdiri di tengah ruangan
   - Pegang HP/kamera horizontal
   - Pastikan minimal 1 marker terlihat

3. **Ikuti Instruksi:**
   - Lihat **Navigation Panel** untuk instruksi
   - Eksekusi gerakan secara **smooth**
   - Tunggu sistem update (1-2 detik)
   - Ikuti instruksi berikutnya

4. **Saat LOCKED:**
   - Marker sudah centered dan cukup dekat
   - Tekan `S` untuk scan
   - Sistem akan cari marker berikutnya

### Simulasi Robot:

**Anda bertindak sebagai robot!**

```
[INSTRUKSI: PUTAR KIRI 20°]
  → Putar badan/kamera ke kiri perlahan

[INSTRUKSI: MAJU 15 cm]
  → Maju beberapa langkah

[INSTRUKSI: LOCKED!]
  → Tekan 'S' untuk scan
```

### Untuk Hasil Terbaik:

- 🎯 Gerakan **smooth** dan **perlahan**
- 👀 Selalu lihat **Navigation Panel**
- ⏱️ Tunggu sistem **update** sebelum gerakan baru
- 📷 Jaga kamera **stabil**
- 💡 Pastikan **pencahayaan** cukup

---

## 🔬 Cara Kerja Teknis

### Algoritma Detection

```cpp
1. Capture frame dari kamera
2. Detect ArUco markers dengan OpenCV
3. Hitung posisi center setiap marker
4. Hitung ukuran marker (estimasi jarak)
5. Pilih target marker (prioritas)
6. Hitung offset dari frame center
7. Generate instruksi navigasi
8. Update UI (camera + panel)
9. Repeat
```

### State Transition

```cpp
if (no markers detected)
    → SCANNING

else if (marker detected but not centered)
    → TRACKING

else if (centered but far)
    → APPROACHING

else if (centered and close)
    → LOCKED

else if (all markers scanned)
    → COMPLETE
```

### Navigation Decision Tree

```
Marker Detected?
  ├─ No → "PUTAR KIRI - Scan lingkungan"
  └─ Yes
      ├─ Offset X > threshold?
      │   ├─ Positive → "PUTAR KANAN"
      │   └─ Negative → "PUTAR KIRI"
      └─ Centered?
          ├─ Size < min?
          │   └─ "MAJU X cm"
          └─ Size > min?
              └─ "LOCKED!"
```

---

## 📦 Struktur File

```
/home/icad/Downloads/magang/
├── src/
│   ├── autonomous_aruco.cpp     ← Program utama (NEW!)
│   ├── aruco_detection.cpp      ← Detection biasa
│   └── ...
├── build/
│   ├── autonomous_aruco         ← Executable (NEW!)
│   ├── autonomous_navigation_log.txt  ← Log file
│   └── ...
├── aruco_markers/               ← Marker images
│   ├── aruco_marker_00.png
│   ├── aruco_marker_01.png
│   └── ...
├── run_autonomous.sh            ← Script launcher (NEW!)
├── generate_aruco.py            ← Generate markers
├── CMakeLists.txt               ← Build config (updated)
├── README_AUTONOMOUS.md         ← Dokumentasi ini (NEW!)
└── ...
```

---

## 🎨 Customization

### Ubah Warna State

Edit di `src/autonomous_aruco.cpp`:

```cpp
cv::Scalar getStateColor() const
{
    switch (currentState)
    {
        case SCANNING: return cv::Scalar(0, 165, 255);    // Orange
        case TRACKING: return cv::Scalar(0, 255, 255);    // Yellow
        case LOCKED: return cv::Scalar(0, 255, 0);        // Green
        // ...
    }
}
```

### Ubah Ukuran Window

```cpp
cv::resizeWindow("Camera View", 1024, 768);     // Lebih besar
cv::resizeWindow("Navigation Panel", 600, 800);  // Lebih tinggi
```

### Ubah Dictionary ArUco

Jika ingin support lebih banyak marker:

```cpp
// Ganti DICT_4X4_50 dengan:
cv::Ptr<cv::aruco::Dictionary> dictionary = 
    cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
```

**Available dictionaries:**
- `DICT_4X4_50` → 50 markers (current)
- `DICT_4X4_100` → 100 markers
- `DICT_5X5_50` → 50 markers
- `DICT_6X6_250` → 250 markers

---

## 🚀 Next Steps & Ideas

### Upgrade Ideas:

1. **Voice Guidance:**
   - Tambahkan text-to-speech untuk instruksi suara
   - Gunakan `espeak` atau `festival`

2. **Obstacle Detection:**
   - Deteksi halangan dengan depth camera
   - Warning saat terlalu dekat dinding

3. **Path Planning:**
   - Simpan posisi marker yang sudah di-scan
   - Buat path optimal untuk scan semua marker

4. **Multi-Robot:**
   - Koordinasi beberapa robot
   - Distributed scanning

5. **3D Mapping:**
   - Gunakan pose estimation
   - Buat 3D map lingkungan

6. **Real Robot Integration:**
   - Connect ke motor controller
   - Autonomous movement (tidak manual lagi)
   - PID control untuk smooth movement

---

## 📚 Referensi

- **OpenCV ArUco:** https://docs.opencv.org/4.x/d5/dae/tutorial_aruco_detection.html
- **ArUco Original Paper:** https://www.uco.es/investiga/grupos/ava/node/26
- **Pose Estimation:** https://docs.opencv.org/4.x/d5/d1f/calib3d_solvePnP.html

---

## 👨‍💻 Developer

**Created by:** ICAD Team  
**Date:** November 2025  
**Version:** 1.0.0  
**Program:** `autonomous_aruco`  

---

## 📄 License

Free to use for educational and research purposes.

---

## 🙏 Acknowledgments

Terima kasih kepada:
- OpenCV team untuk library ArUco detection
- IP Webcam developers
- ArUco markers creators

---

## 💬 Feedback & Support

Jika ada pertanyaan atau masalah:

1. Check **Troubleshooting** section
2. Lihat log file untuk debug: `build/autonomous_navigation_log.txt`
3. Test dengan webcam dulu sebelum IP camera
4. Pastikan marker sudah ter-print dengan benar

---

## 🎯 Quick Reference Card

```
┌─────────────────────────────────────────┐
│  AUTONOMOUS NAVIGATION QUICK REF        │
├─────────────────────────────────────────┤
│  STATE    │ COLOR  │ ACTION             │
├───────────┼────────┼────────────────────┤
│ SCANNING  │ 🟠     │ Cari marker        │
│ TRACKING  │ 🟡     │ Adjust posisi      │
│ APPROACH  │ 🔵     │ Maju               │
│ LOCKED    │ 🟢     │ Tekan 'S'          │
├─────────────────────────────────────────┤
│  KEY      │ FUNCTION                    │
├───────────┼─────────────────────────────┤
│  S        │ Scan marker                 │
│  R        │ Reset target                │
│  Q/ESC    │ Quit                        │
└─────────────────────────────────────────┘
```

**REMEMBER:**
- 👀 Watch the Navigation Panel!
- 🐢 Move slowly and smoothly
- ⏳ Wait for system to update
- 🎯 Follow instructions precisely

**Selamat mencoba sistem autonomous navigation! 🚀**

