# 🎯 SYSTEM OVERVIEW - Magang Project

## 📦 Daftar Program

Workspace ini berisi **4 program utama**:

### 1. 🤖 **Autonomous ArUco Navigation** (NEW! ⭐)
**File:** `autonomous_aruco`  
**Status:** ✅ Production Ready

**Deskripsi:**  
Sistem navigasi semi-autonomous yang memandu operator untuk mencari dan men-scan ArUco markers secara otomatis.

**Fitur:**
- Auto-scanning mode
- Smart navigation (kiri/kanan/maju/mundur)
- Dual window display
- IP Camera support
- Real-time guidance

**Jalankan:**
```bash
./run_autonomous.sh
# atau
cd build && ./autonomous_aruco
```

**Dokumentasi:**
- 📖 `README_AUTONOMOUS.md` (lengkap)
- ⚡ `AUTONOMOUS_QUICK_START.md` (cepat)

---

### 2. 🎯 **ArUco Detection** (Basic)
**File:** `aruco_detection`  
**Status:** ✅ Ready

**Deskripsi:**  
Deteksi ArUco markers basic dengan logging dan notification.

**Fitur:**
- Real-time ArUco detection
- Modal notification
- Logging system
- Screenshot capture
- Generate markers

**Jalankan:**
```bash
./run_aruco.sh
# atau
cd build && ./aruco_detection
```

**Dokumentasi:**
- 📖 `README_ARUCO.md`
- ⚡ `ARUCO_QUICK_START.md`

---

### 3. 😊 **Face Blur Detection**
**File:** `face_blur_detection`  
**Status:** ✅ Ready

**Deskripsi:**  
Deteksi wajah dengan efek blur otomatis untuk privacy.

**Fitur:**
- Face detection dengan Haar Cascade
- Auto blur detected faces
- Black object detection
- Real-time processing

**Jalankan:**
```bash
cd build && ./face_blur_detection
```

**Dokumentasi:**
- 📖 `README_FACE_BLUR.md`

---

### 4. ⚫ **Black Object Detection**
**File:** `cobaAja_kj`  
**Status:** ✅ Ready

**Deskripsi:**  
Deteksi objek berwarna hitam dengan contour analysis.

**Fitur:**
- HSV color filtering
- Contour detection
- Bounding box visualization

**Jalankan:**
```bash
cd build && ./cobaAja_kj
```

---

## 🗂️ Struktur Folder

```
/home/icad/Downloads/magang/
│
├── 📁 src/                          # Source code
│   ├── autonomous_aruco.cpp         ⭐ NEW! Autonomous navigation
│   ├── aruco_detection.cpp          ← ArUco detection
│   ├── face_blur_detection.cpp      ← Face blur
│   └── cobaa.cpp                    ← Black object detection
│
├── 📁 build/                        # Compiled binaries
│   ├── autonomous_aruco             ⭐ NEW!
│   ├── aruco_detection
│   ├── face_blur_detection
│   ├── cobaAja_kj
│   └── *.log                        ← Log files
│
├── 📁 aruco_markers/                # Generated ArUco markers
│   └── aruco_marker_*.png
│
├── 🚀 run_autonomous.sh             ⭐ NEW! Launcher
├── 🚀 run_aruco.sh                  ← Launcher for basic ArUco
├── 🚀 aruco_menu.sh                 ← Menu system
├── 🚀 config_ip_camera.sh           ⭐ NEW! IP config tool
│
├── 🐍 generate_aruco.py             # Python script to generate markers
├── 🐍 view_log.py                   # Python log viewer
│
├── 📖 README_AUTONOMOUS.md          ⭐ NEW! Full docs
├── ⚡ AUTONOMOUS_QUICK_START.md     ⭐ NEW! Quick guide
├── 📖 README_ARUCO.md
├── ⚡ ARUCO_QUICK_START.md
├── 📖 README_FACE_BLUR.md
├── 📖 ARUCO_COMPLETE_GUIDE.md
├── 📖 LOGGING_FEATURE.md
├── 🎯 SYSTEM_OVERVIEW.md            ← File ini
│
└── ⚙️ CMakeLists.txt                # Build configuration

```

---

## 🚀 Quick Commands

### Compile Semua Program
```bash
cd build
cmake ..
make
```

### Compile Program Tertentu
```bash
cd build
make autonomous_aruco      # ⭐ Autonomous navigation
make aruco_detection        # Basic ArUco
make face_blur_detection    # Face blur
make cobaAja_kj            # Black object
```

### Clean & Rebuild
```bash
cd build
rm -rf *
cmake ..
make
```

---

## 📱 IP Camera Setup

### Quick Setup:
1. Download "IP Webcam" (Android) atau "IP Camera Lite" (iOS)
2. Start server di app
3. Catat URL (contoh: `http://10.237.86.79:8080/video`)
4. Jalankan:
   ```bash
   ./config_ip_camera.sh
   ```
5. Masukkan URL baru
6. Done! ✅

### Manual Setup:
Edit `src/autonomous_aruco.cpp` line ~528:
```cpp
std::string ipCameraURL = "http://YOUR_IP:8080/video";
```
Lalu compile ulang.

---

## 🎯 Recommended Workflow

### Untuk Pertama Kali:

1. **Generate ArUco Markers**
   ```bash
   python3 generate_aruco.py
   ```
   Print marker dari folder `aruco_markers/` (minimal 5 marker)

2. **Setup IP Camera**
   ```bash
   ./config_ip_camera.sh
   ```
   Atau gunakan webcam laptop (pilih opsi 2 di launcher)

3. **Test Basic Detection**
   ```bash
   ./run_aruco.sh
   ```
   Pastikan marker terdeteksi dengan baik

4. **Try Autonomous Mode** ⭐
   ```bash
   ./run_autonomous.sh
   ```
   Ikuti instruksi di Navigation Panel!

---

## 📊 Comparison Table

| Fitur | Basic ArUco | Autonomous ⭐ |
|-------|-------------|---------------|
| ArUco Detection | ✅ | ✅ |
| IP Camera | ✅ | ✅ |
| Logging | ✅ | ✅ |
| Modal Notification | ✅ | ❌ |
| **Auto-Scanning** | ❌ | ✅ |
| **Navigation Guide** | ❌ | ✅ |
| **Target Tracking** | ❌ | ✅ |
| **Dual Display** | ❌ | ✅ |
| **State Machine** | ❌ | ✅ |

💡 **Rekomendasi:** Gunakan **Autonomous** untuk simulasi robot, gunakan **Basic** untuk testing sederhana.

---

## 🔧 Tools & Utilities

### 1. Configuration Tool
```bash
./config_ip_camera.sh
```
Update IP camera URL tanpa edit kode.

### 2. Generate Markers
```bash
python3 generate_aruco.py
```
Generate 20 ArUco markers (ID 0-19).

### 3. View Logs
```bash
python3 view_log.py
```
View detection logs dengan format rapi.

### 4. Menu System
```bash
./aruco_menu.sh
```
Interactive menu untuk semua fitur ArUco.

---

## 📖 Documentation Index

### Autonomous Navigation ⭐
- **Full Guide:** `README_AUTONOMOUS.md`
- **Quick Start:** `AUTONOMOUS_QUICK_START.md`

### Basic ArUco Detection
- **Full Guide:** `README_ARUCO.md`
- **Quick Start:** `ARUCO_QUICK_START.md`
- **Complete Guide:** `ARUCO_COMPLETE_GUIDE.md`

### Other
- **Logging Feature:** `LOGGING_FEATURE.md`
- **Face Blur:** `README_FACE_BLUR.md`
- **System Overview:** `SYSTEM_OVERVIEW.md` (this file)

---

## 💡 Tips & Best Practices

### Untuk Development:

1. **Always compile in build directory:**
   ```bash
   cd build && cmake .. && make
   ```

2. **Check logs for debugging:**
   ```bash
   tail -f build/*_log.txt
   ```

3. **Backup before major changes:**
   ```bash
   cp src/autonomous_aruco.cpp src/autonomous_aruco.cpp.backup
   ```

### Untuk Testing:

1. **Start simple:**
   - Test dengan webcam dulu
   - Test dengan 2-3 marker dulu
   - Baru tambah complexity

2. **Environment:**
   - Pencahayaan cukup (>300 lux)
   - Background kontras (putih lebih baik)
   - Jarak marker 20-200 cm

3. **Markers:**
   - Print minimal 5x5 cm
   - Kertas putih tebal
   - Tinta hitam pekat
   - Laminating untuk durability

---

## 🐛 Common Issues

### Issue: IP Camera tidak connect
**Solution:**
- Cek WiFi (HP & laptop sama)
- Test di browser: `http://IP:8080/video`
- Coba port lain (4747, 8081)
- Restart aplikasi di HP

### Issue: Marker tidak terdeteksi
**Solution:**
- Print ulang dengan quality bagus
- Tambah cahaya
- Bersihkan lensa kamera
- Periksa fokus kamera

### Issue: Compile error
**Solution:**
```bash
sudo apt-get install -y libopencv-dev libopencv-contrib-dev
cd build
rm -rf *
cmake ..
make
```

### Issue: Navigation tidak akurat
**Solution:**
- Edit threshold di `src/autonomous_aruco.cpp`
- Gerakan lebih smooth
- Stabilkan kamera
- Tunggu sistem update

---

## 🎓 Learning Path

Recommended order untuk belajar:

1. **Basic ArUco Detection** (`aruco_detection`)
   - Pahami cara kerja ArUco
   - Lihat modal notification
   - Coba screenshot & logging

2. **Autonomous Navigation** (`autonomous_aruco`) ⭐
   - Pahami state machine
   - Ikuti instruksi navigasi
   - Lihat bagaimana target dipilih

3. **Customization**
   - Edit parameter (threshold, size)
   - Ubah warna state
   - Tambah fitur sendiri

4. **Integration**
   - Connect ke motor controller
   - Implementasi actual autonomous movement
   - Add obstacle detection

---

## 📈 Performance Tips

### Untuk FPS Lebih Tinggi:
```cpp
// Di main loop
int key = cv::waitKey(1);  // Lower wait time
```

### Untuk Detection Lebih Akurat:
```cpp
detectorParams->cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX;
detectorParams->cornerRefinementWinSize = 5;
```

### Untuk Stabilitas Lebih Baik:
```cpp
// Tambah smoothing
float centerThreshold = 100.0f;  // Lebih longgar
int maxHistorySize = 20;         // Lebih banyak history
```

---

## 🔗 External Resources

### OpenCV Documentation:
- ArUco Module: https://docs.opencv.org/4.x/d5/dae/tutorial_aruco_detection.html
- Pose Estimation: https://docs.opencv.org/4.x/d5/d1f/calib3d_solvePnP.html

### ArUco Resources:
- Online Generator: https://chev.me/arucogen/
- Original Paper: https://www.uco.es/investiga/grupos/ava/node/26

### OpenCV Installation:
- Official Guide: https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html

---

## 🎯 Project Goals

✅ **Completed:**
- [x] Basic ArUco detection
- [x] Logging system
- [x] IP Camera integration
- [x] Modal notifications
- [x] Autonomous navigation ⭐
- [x] Smart guidance system ⭐
- [x] Dual display ⭐

🚧 **Future Plans:**
- [ ] Voice guidance (text-to-speech)
- [ ] Path planning algorithm
- [ ] 3D pose estimation
- [ ] Multi-robot coordination
- [ ] Real robot hardware integration
- [ ] Web dashboard
- [ ] Mobile app controller

---

## 👨‍💻 Development Info

**Project:** Magang Computer Vision  
**Framework:** OpenCV 4.x  
**Language:** C++17  
**Build System:** CMake  
**Platform:** Linux (Ubuntu/Debian)  

**Features Implemented:**
- ArUco marker detection
- Semi-autonomous navigation
- State machine system
- IP camera streaming
- Real-time guidance
- Logging & monitoring
- Face detection & blur
- Object detection

---

## 🙏 Credits

**Developed by:** ICAD Team  
**Date:** November 2025  
**Version:** 2.0.0  

**Special Thanks:**
- OpenCV contributors
- ArUco developers
- IP Webcam app developers

---

## 📞 Support

Jika ada pertanyaan:

1. ✅ Baca dokumentasi terkait
2. ✅ Check troubleshooting section
3. ✅ Lihat log files
4. ✅ Test dengan webcam dulu

---

## 🎊 Conclusion

Anda sekarang memiliki **4 program** yang siap pakai:

1. 🤖 **Autonomous Navigation** ⭐ - Untuk simulasi robot semi-autonomous
2. 🎯 **Basic ArUco Detection** - Untuk testing & development
3. 😊 **Face Blur** - Untuk privacy protection
4. ⚫ **Black Object Detection** - Untuk object tracking

**Recommended untuk dicoba: AUTONOMOUS NAVIGATION! 🚀**

```bash
./run_autonomous.sh
```

**Selamat bereksperimen! 🎉**

---

*Last Updated: November 2025*  
*Document Version: 1.0.0*

