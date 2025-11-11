# 🤖 Magang Project - Computer Vision System

[![OpenCV](https://img.shields.io/badge/OpenCV-4.x-blue.svg)](https://opencv.org/)
[![C++](https://img.shields.io/badge/C++-17-green.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.0+-red.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-Educational-yellow.svg)]()

Kumpulan program Computer Vision untuk deteksi objek, ArUco markers, dan navigasi semi-autonomous menggunakan OpenCV.

---

## 🚀 Quick Start

### 1️⃣ Jalankan Main Menu (Recommended)

```bash
cd /home/icad/Downloads/magang/autonomous_system
./main_menu.sh
```

Menu interaktif akan muncul dengan pilihan semua program dan tools.

### 2️⃣ Atau Jalankan Program Langsung

**Autonomous Navigation (⭐ Recommended):**
```bash
# Dari root folder
./start_autonomous.sh

# Atau dari folder autonomous_system
cd autonomous_system
./run_autonomous.sh
```

**Basic ArUco Detection:**
```bash
./run_aruco.sh
```

---

## 📦 Program yang Tersedia

| # | Program | Deskripsi | Status |
|---|---------|-----------|--------|
| 1 | 🤖 **Autonomous ArUco Navigation** | Sistem navigasi semi-autonomous dengan panduan cerdas | ⭐ NEW! |
| 2 | 🎯 **ArUco Detection** | Deteksi ArUco markers dengan logging & notification | ✅ Ready |
| 3 | 😊 **Face Blur Detection** | Deteksi wajah dengan efek blur otomatis | ✅ Ready |
| 4 | ⚫ **Black Object Detection** | Deteksi objek berwarna hitam | ✅ Ready |

---

## 🌟 Featured: Autonomous Navigation System

**Sistem navigasi semi-autonomous** yang menggunakan ArUco markers untuk memandu operator secara cerdas.

### ✨ Fitur Utama:
- 🔍 **Auto-Scanning Mode** - Mencari markers otomatis
- 🧭 **Smart Navigation** - Instruksi arah yang jelas (kiri/kanan/maju/mundur)
- 📱 **IP Camera Support** - Gunakan HP sebagai kamera
- 🎯 **Target Tracking** - Mengunci dan mengikuti marker
- 📊 **Dual Display** - Camera view + Navigation panel terpisah
- 🎨 **Visual Feedback** - Indikator status yang jelas

### 🎮 Cara Kerja:
1. **SCANNING** 🟠 - Sistem mencari marker di lingkungan
2. **TRACKING** 🟡 - Mengikuti marker yang terdeteksi
3. **APPROACHING** 🔵 - Mendekati marker
4. **LOCKED** 🟢 - Siap untuk scan!

### 📖 Dokumentasi:
- 📘 [README_AUTONOMOUS.md](README_AUTONOMOUS.md) - Full documentation
- ⚡ [AUTONOMOUS_QUICK_START.md](AUTONOMOUS_QUICK_START.md) - Quick start guide

---

## 📖 Dokumentasi Lengkap

### 🎯 System Overview
- **[SYSTEM_OVERVIEW.md](autonomous_system/SYSTEM_OVERVIEW.md)** ← Start here! 🌟
  - Ringkasan semua program
  - Struktur folder
  - Comparison table
  - Best practices

### 🤖 Autonomous Navigation
- **[📁 autonomous_system/](autonomous_system/)** - **Folder khusus untuk sistem autonomous** ⭐
- **[README_AUTONOMOUS.md](autonomous_system/README_AUTONOMOUS.md)** - Full guide dengan semua detail
- **[AUTONOMOUS_QUICK_START.md](autonomous_system/AUTONOMOUS_QUICK_START.md)** - 3 langkah mudah untuk mulai
- **[GETTING_STARTED.txt](autonomous_system/GETTING_STARTED.txt)** - Panduan lengkap memulai

### 🎯 ArUco Detection
- **[README_ARUCO.md](README_ARUCO.md)** - Panduan lengkap
- **[ARUCO_QUICK_START.md](ARUCO_QUICK_START.md)** - Quick start
- **[ARUCO_COMPLETE_GUIDE.md](ARUCO_COMPLETE_GUIDE.md)** - Complete guide
- **[LOGGING_FEATURE.md](LOGGING_FEATURE.md)** - Logging system

### 😊 Other Features
- **[README_FACE_BLUR.md](README_FACE_BLUR.md)** - Face blur detection

---

## 🛠️ Installation & Setup

### Prerequisites

```bash
sudo apt-get update
sudo apt-get install -y \
    libopencv-dev \
    libopencv-contrib-dev \
    cmake \
    g++ \
    build-essential
```

### Compile Programs

```bash
cd /home/icad/Downloads/magang/build
cmake ..
make
```

Atau gunakan main menu:
```bash
./main_menu.sh
# Pilih opsi 8 (Compile All Programs)
```

### Verify Installation

```bash
cd build
ls -lh autonomous_aruco aruco_detection face_blur_detection cobaAja_kj
```

Semua file executable harus ada.

---

## 📱 IP Camera Setup

### Quick Setup (Android):

1. **Download** "IP Webcam" dari Play Store
2. **Buka app** → Scroll down → **"Start server"**
3. **Catat URL** (contoh: `http://10.237.86.79:8080/video`)
4. **Configure:**
   ```bash
   ./config_ip_camera.sh
   ```
5. **Masukkan URL** yang didapat dari HP
6. **Done!** ✅

### Manual Configuration:

Edit file `src/autonomous_aruco.cpp` atau `src/aruco_detection.cpp`:

```cpp
std::string ipCameraURL = "http://YOUR_IP:8080/video";
```

Lalu compile ulang:
```bash
cd build && cmake .. && make
```

---

## 🎮 Usage Examples

### Example 1: Autonomous Navigation

```bash
# Launch dengan quick launcher
./start_autonomous.sh

# Atau dari folder autonomous_system
cd autonomous_system
./run_autonomous.sh

# Atau direct run dengan options
cd build
./autonomous_aruco 0  # Webcam
./autonomous_aruco "http://192.168.1.100:8080/video"  # Custom IP
```

**Output:**
- Window 1: Camera feed dengan marker detection
- Window 2: Navigation panel dengan instruksi besar

**Instruksi yang muncul:**
- "PUTAR KIRI 20°" → Putar badan ke kiri
- "MAJU 15 cm" → Maju beberapa langkah
- "LOCKED! - Tekan 'SCAN' (S)" → Tekan S untuk scan

### Example 2: Basic ArUco Detection

```bash
./run_aruco.sh
# Pilih sumber kamera
# Program akan mendeteksi dan log semua marker
```

**Keyboard Controls:**
- `s` → Screenshot
- `l` → Lihat log summary
- `g` → Generate markers
- `q` → Keluar

### Example 3: Generate ArUco Markers

```bash
python3 generate_aruco.py
# Markers tersimpan di folder aruco_markers/
# Print dan gunakan untuk testing
```

---

## 🔧 Tools & Utilities

### 1. Main Menu (`main_menu.sh`)
Menu interaktif untuk semua program dan tools.

```bash
cd autonomous_system
./main_menu.sh
```

### 2. Config IP Camera (`config_ip_camera.sh`)
Update IP camera URL tanpa edit kode.

```bash
cd autonomous_system
./config_ip_camera.sh
```

### 3. Generate Markers (`generate_aruco.py`)
Generate ArUco markers untuk testing.

```bash
python3 generate_aruco.py
```

### 4. View Logs (`view_log.py`)
View detection logs dengan format rapi.

```bash
python3 view_log.py
```

---

## 📊 Project Structure

```
/home/icad/Downloads/magang/
│
├── 📁 src/                          # Source code
│   ├── autonomous_aruco.cpp         # ⭐ Autonomous navigation
│   ├── aruco_detection.cpp          # Basic ArUco detection
│   ├── face_blur_detection.cpp      # Face blur
│   └── cobaa.cpp                    # Black object detection
│
├── 📁 build/                        # Compiled binaries
│   ├── autonomous_aruco             # ⭐ Main program
│   ├── aruco_detection
│   ├── face_blur_detection
│   └── cobaAja_kj
│
├── 📁 aruco_markers/                # Generated markers
│   └── aruco_marker_*.png
│
├── 📁 autonomous_system/            # ⭐ NEW! Autonomous system files
│   ├── 📖 README.md                 # Index untuk folder ini
│   ├── 📖 README_AUTONOMOUS.md      # Full documentation
│   ├── 📖 AUTONOMOUS_QUICK_START.md # Quick start guide
│   ├── 📖 SYSTEM_OVERVIEW.md        # System overview
│   ├── 📄 GETTING_STARTED.txt       # Getting started
│   ├── 🚀 main_menu.sh              # Main launcher
│   ├── 🚀 run_autonomous.sh         # Autonomous launcher
│   └── 🚀 config_ip_camera.sh       # Config tool
│
├── 🚀 start_autonomous.sh           # ⭐ Quick launcher (root)
├── 🚀 run_aruco.sh                  # ArUco launcher
├── 🚀 aruco_menu.sh                 # ArUco menu
│
├── 🐍 generate_aruco.py             # Generate markers
├── 🐍 view_log.py                   # Log viewer
│
├── 📖 README.md                     # This file (main entry)
├── 📖 README_ARUCO.md               # ArUco docs
├── 📖 ARUCO_COMPLETE_GUIDE.md       # Complete ArUco guide
└── ...                              # Other docs
```

---

## 🎯 Recommended Workflow

### For First-Time Users:

1. **Read System Overview**
   ```bash
   less autonomous_system/SYSTEM_OVERVIEW.md
   ```

2. **Generate Markers**
   ```bash
   python3 generate_aruco.py
   ```
   Print 5-10 markers (minimal 5x5 cm)

3. **Setup IP Camera** (atau gunakan webcam)
   ```bash
   cd autonomous_system
   ./config_ip_camera.sh
   ```

4. **Test Basic Detection**
   ```bash
   ./run_aruco.sh
   ```
   Pastikan marker terdeteksi dengan baik

5. **Try Autonomous Mode** ⭐
   ```bash
   ./start_autonomous.sh
   # atau: cd autonomous_system && ./run_autonomous.sh
   ```
   Ikuti instruksi di Navigation Panel!

### For Developers:

1. **Read source code:**
   - `src/autonomous_aruco.cpp` - Main logic
   - Study the `AutonomousNavigator` class
   - Understand state machine

2. **Modify parameters:**
   ```cpp
   float centerThreshold = 80.0f;
   float minMarkerSize = 120.0f;
   ```

3. **Compile & test:**
   ```bash
   cd build
   cmake .. && make
   ./autonomous_aruco
   ```

4. **Contribute improvements!**

---

## 🐛 Troubleshooting

### Issue: IP Camera tidak connect

**Solution:**
```bash
# 1. Cek koneksi
ping IP_HP_ANDA

# 2. Test di browser
firefox http://IP_HP:8080/video

# 3. Coba URL alternatif
./autonomous_aruco "http://IP:8080/videofeed"
```

### Issue: Marker tidak terdeteksi

**Solution:**
- Print marker dengan quality bagus (hitam pekat, putih bersih)
- Tambah cahaya di ruangan
- Jarak 20-200 cm dari marker
- Pastikan marker tidak terlipat/rusak

### Issue: Compile error

**Solution:**
```bash
# Install dependencies
sudo apt-get install -y libopencv-dev libopencv-contrib-dev

# Clean rebuild
cd build
rm -rf *
cmake ..
make
```

### More Help:

Check dokumentasi lengkap di masing-masing README file, atau:

```bash
./main_menu.sh
# Pilih opsi 9 (Show Documentation)
```

---

## 💡 Tips & Best Practices

### Untuk Testing:
- ✅ Mulai dengan webcam sebelum IP camera
- ✅ Test dengan 2-3 marker dulu
- ✅ Pencahayaan 300+ lux (cukup terang)
- ✅ Background putih/kontras
- ✅ Print marker minimal 5x5 cm

### Untuk Autonomous Mode:
- 🎯 Gerakan **smooth** dan **perlahan**
- 👀 Selalu lihat **Navigation Panel**
- ⏱️ Tunggu sistem **update** sebelum gerakan baru
- 📷 Jaga kamera **stabil**

### Untuk Development:
- 🔧 Gunakan `main_menu.sh` untuk compile & test
- 📝 Check logs untuk debugging
- 💾 Backup sebelum edit besar
- 🧪 Test incremental changes

---

## 🎓 Learning Resources

### Internal Docs:
- [autonomous_system/](autonomous_system/) - **Folder khusus autonomous navigation** ⭐
- [SYSTEM_OVERVIEW.md](autonomous_system/SYSTEM_OVERVIEW.md) - Complete system overview
- [README_AUTONOMOUS.md](autonomous_system/README_AUTONOMOUS.md) - Autonomous navigation details
- [ARUCO_COMPLETE_GUIDE.md](ARUCO_COMPLETE_GUIDE.md) - Complete ArUco guide

### External Resources:
- [OpenCV ArUco Tutorial](https://docs.opencv.org/4.x/d5/dae/tutorial_aruco_detection.html)
- [ArUco Original Paper](https://www.uco.es/investiga/grupos/ava/node/26)
- [Online Marker Generator](https://chev.me/arucogen/)

---

## 🚀 Future Roadmap

### Planned Features:
- [ ] Voice guidance (text-to-speech)
- [ ] Path planning algorithm
- [ ] 3D pose estimation & mapping
- [ ] Multi-robot coordination
- [ ] Real robot hardware integration
- [ ] Web dashboard
- [ ] Mobile app controller
- [ ] Obstacle detection & avoidance

### Current Status:
- [x] Basic ArUco detection ✅
- [x] IP Camera integration ✅
- [x] Logging system ✅
- [x] Autonomous navigation ✅
- [x] Smart guidance system ✅
- [x] Dual display interface ✅

---

## 👨‍💻 Development Info

**Project:** Magang Computer Vision System  
**Version:** 2.0.0  
**Last Updated:** November 2025  
**Developed by:** ICAD Team  

**Technologies:**
- OpenCV 4.x (ArUco, Detection, Processing)
- C++17 (Main programs)
- Python 3 (Tools & utilities)
- CMake (Build system)
- Bash (Launchers & scripts)

**Platform:**
- Linux (Ubuntu/Debian)
- Tested on: Ubuntu 22.04 LTS

---

## 🙏 Credits & Acknowledgments

**Special Thanks:**
- OpenCV community untuk library yang powerful
- ArUco developers untuk marker system
- IP Webcam app developers
- All contributors & testers

---

## 📄 License

Educational use only. Free to use for learning and research purposes.

---

## 📞 Support & Contact

**Need Help?**

1. ✅ Check [SYSTEM_OVERVIEW.md](SYSTEM_OVERVIEW.md)
2. ✅ Read program-specific README
3. ✅ Look at Troubleshooting section
4. ✅ Check log files for errors

**Found a Bug?**
- Check logs: `build/*_log.txt`
- Try clean rebuild: `cd build && rm -rf * && cmake .. && make`

---

## 🎊 Get Started Now!

Ready to try the **Autonomous Navigation System**?

```bash
# Quick start (3 steps):
python3 generate_aruco.py          # 1. Generate markers
cd autonomous_system               # 2. Masuk ke folder autonomous
./config_ip_camera.sh              # 3. Setup camera (atau skip, gunakan webcam)
./run_autonomous.sh                # 4. Launch!
```

**Or use quick launcher from root:**

```bash
./start_autonomous.sh
```

**Or use the main menu:**

```bash
cd autonomous_system
./main_menu.sh
```

---

## 🌟 Highlights

### ⭐ NEW: Autonomous Navigation System

**Paling recommended untuk dicoba!**

```bash
./run_autonomous.sh
```

- Semi-autonomous robot simulation
- Smart navigation guidance
- Real-time target tracking
- Intuitive dual-window interface

**Perfect untuk:**
- 🎓 Learning autonomous systems
- 🤖 Robot navigation research
- 🎯 ArUco marker applications
- 📱 Computer vision projects

---

## 🎯 Quick Command Reference

```bash
# Quick Launch (from root)
./start_autonomous.sh                    # ⭐ Autonomous navigation

# Main Menu
cd autonomous_system && ./main_menu.sh   # Menu dengan semua pilihan

# Launch Programs
./run_aruco.sh                           # Basic ArUco detection
cd autonomous_system && ./run_autonomous.sh  # Autonomous navigation

# Tools
cd autonomous_system && ./config_ip_camera.sh  # Configure IP camera
python3 generate_aruco.py                      # Generate markers
python3 view_log.py                            # View logs

# Build
cd build
cmake .. && make                         # Compile all
make autonomous_aruco                    # Compile specific

# Direct Run
cd build
./autonomous_aruco                       # IP camera (default)
./autonomous_aruco 0                     # Webcam
./autonomous_aruco "URL"                 # Custom camera
```

---

## 📚 Documentation Index

| Document | Description | Location |
|----------|-------------|----------|
| `README.md` | **This file** - Main entry point | Root |
| **`autonomous_system/`** | **📁 Folder khusus autonomous** | **Root** ⭐ |
| `SYSTEM_OVERVIEW.md` | Complete system overview & comparison | autonomous_system/ |
| `README_AUTONOMOUS.md` | Autonomous navigation full guide | autonomous_system/ |
| `AUTONOMOUS_QUICK_START.md` | Autonomous quick start (3 steps) | autonomous_system/ |
| `GETTING_STARTED.txt` | Getting started guide | autonomous_system/ |
| `README_ARUCO.md` | Basic ArUco detection guide | Root |
| `ARUCO_QUICK_START.md` | ArUco quick start | Root |
| `ARUCO_COMPLETE_GUIDE.md` | Complete ArUco reference | Root |
| `README_FACE_BLUR.md` | Face blur detection guide | Root |
| `LOGGING_FEATURE.md` | Logging system documentation | Root |

---

<div align="center">

## 🎉 Selamat Mencoba!

**Made with ❤️ by ICAD Team**

*Computer Vision • Autonomous Systems • ArUco Detection*

---

⭐ **Recommended:** Try the Autonomous Navigation System first!

```bash
./run_autonomous.sh
```

---

*Last Updated: November 2025 | Version 2.0.0*

</div>

