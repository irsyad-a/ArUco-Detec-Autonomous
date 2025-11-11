# 📁 File Organization - Magang Project

**Dokumentasi ini menjelaskan organisasi file dalam project.**

---

## 🎯 Struktur Folder

```
/home/icad/Downloads/magang/
│
├── 📁 autonomous_system/          ← ⭐ FOLDER BARU untuk autonomous navigation
│   ├── README.md                  ← Index folder ini
│   ├── README_AUTONOMOUS.md       ← Full documentation
│   ├── AUTONOMOUS_QUICK_START.md  ← Quick start guide
│   ├── SYSTEM_OVERVIEW.md         ← System overview
│   ├── GETTING_STARTED.txt        ← Getting started panduan
│   ├── main_menu.sh               ← Main menu launcher
│   ├── run_autonomous.sh          ← Autonomous launcher
│   └── config_ip_camera.sh        ← IP camera config tool
│
├── 📁 src/                        ← Source code
│   ├── autonomous_aruco.cpp       ← ⭐ Autonomous navigation (NEW!)
│   ├── aruco_detection.cpp        ← Basic ArUco detection
│   ├── face_blur_detection.cpp    ← Face blur
│   └── cobaa.cpp                  ← Black object detection
│
├── 📁 build/                      ← Compiled binaries
│   ├── autonomous_aruco           ← ⭐ Main program (NEW!)
│   ├── aruco_detection
│   ├── face_blur_detection
│   └── cobaAja_kj
│
├── 📁 aruco_markers/              ← Generated ArUco markers
│   └── aruco_marker_*.png
│
├── 📖 README.md                   ← ⭐ MAIN ENTRY POINT (updated)
├── 📖 README_ARUCO.md             ← ArUco documentation
├── 📖 ARUCO_QUICK_START.md        ← ArUco quick start
├── 📖 ARUCO_COMPLETE_GUIDE.md     ← Complete ArUco guide
├── 📖 README_FACE_BLUR.md         ← Face blur documentation
├── 📖 LOGGING_FEATURE.md          ← Logging system docs
├── 📖 FILE_ORGANIZATION.md        ← This file
│
├── 🚀 start_autonomous.sh         ← ⭐ Quick launcher (NEW!)
├── 🚀 run_aruco.sh                ← ArUco launcher
├── 🚀 aruco_menu.sh               ← ArUco menu
│
├── 🐍 generate_aruco.py           ← Generate markers
├── 🐍 view_log.py                 ← Log viewer
│
└── ⚙️  CMakeLists.txt             ← Build configuration (updated)
```

---

## 📁 Folder: `autonomous_system/`

**Folder khusus untuk Autonomous Navigation System yang baru dibuat.**

### Berisi:

#### 📖 Dokumentasi:
- `README.md` - Index folder & navigation
- `README_AUTONOMOUS.md` - Full documentation (15 KB)
- `AUTONOMOUS_QUICK_START.md` - Quick start (3 langkah)
- `SYSTEM_OVERVIEW.md` - System overview
- `GETTING_STARTED.txt` - Getting started guide

#### 🚀 Scripts:
- `run_autonomous.sh` - Launcher autonomous navigation
- `main_menu.sh` - Main menu untuk semua program
- `config_ip_camera.sh` - Config IP camera tool

### Akses:
```bash
cd /home/icad/Downloads/magang/autonomous_system
ls -lh
```

---

## 📖 Dokumentasi di Root Folder

**Dokumentasi untuk fitur yang sudah ada sebelumnya:**

- `README_ARUCO.md` - Basic ArUco detection
- `ARUCO_QUICK_START.md` - ArUco quick start
- `ARUCO_COMPLETE_GUIDE.md` - Complete ArUco guide
- `README_FACE_BLUR.md` - Face blur detection
- `LOGGING_FEATURE.md` - Logging system
- `FILE_ORGANIZATION.md` - File organization (this file)

---

## 🚀 Quick Launchers

### Di Root Folder:
- `start_autonomous.sh` ⭐ - Quick launcher untuk autonomous (redirect ke folder)
- `run_aruco.sh` - Launcher untuk basic ArUco
- `aruco_menu.sh` - Menu untuk ArUco features

### Di `autonomous_system/`:
- `run_autonomous.sh` - Main autonomous launcher
- `main_menu.sh` - Menu untuk semua program
- `config_ip_camera.sh` - Config tool

---

## 🎯 Cara Menggunakan

### 1. Autonomous Navigation (NEW!) ⭐

**Dari root folder:**
```bash
./start_autonomous.sh
```

**Dari folder autonomous_system:**
```bash
cd autonomous_system
./run_autonomous.sh
```

**Main menu:**
```bash
cd autonomous_system
./main_menu.sh
# Pilih opsi 1
```

### 2. Basic ArUco Detection

```bash
./run_aruco.sh
```

### 3. Other Programs

```bash
cd autonomous_system
./main_menu.sh
# Pilih program yang diinginkan
```

---

## 📚 Dokumentasi Berdasarkan Topik

### 🤖 Autonomous Navigation

**Lokasi:** `autonomous_system/`

| File | Deskripsi |
|------|-----------|
| `GETTING_STARTED.txt` | ⭐ Start here! Panduan lengkap |
| `README_AUTONOMOUS.md` | Full technical documentation |
| `AUTONOMOUS_QUICK_START.md` | 3 langkah mudah |
| `SYSTEM_OVERVIEW.md` | Overview semua program |

**Read order:**
1. GETTING_STARTED.txt
2. AUTONOMOUS_QUICK_START.md
3. README_AUTONOMOUS.md
4. SYSTEM_OVERVIEW.md

### 🎯 ArUco Detection

**Lokasi:** Root folder

| File | Deskripsi |
|------|-----------|
| `README_ARUCO.md` | Main ArUco documentation |
| `ARUCO_QUICK_START.md` | Quick start |
| `ARUCO_COMPLETE_GUIDE.md` | Complete reference |
| `LOGGING_FEATURE.md` | Logging system |

### 😊 Face Blur

**Lokasi:** Root folder

| File | Deskripsi |
|------|-----------|
| `README_FACE_BLUR.md` | Face blur documentation |

---

## 🔍 Finding Files

### Cari dokumentasi autonomous:
```bash
cd autonomous_system
ls -lh *.md *.txt
```

### Cari dokumentasi ArUco:
```bash
ls -lh README_ARUCO.md ARUCO_*.md
```

### Cari semua launcher:
```bash
ls -lh *.sh
ls -lh autonomous_system/*.sh
```

### Cari semua dokumentasi:
```bash
find . -name "*.md" -o -name "*.txt" | grep -v build
```

---

## 💡 Tips Navigasi

### Baru mulai?
1. Baca `README.md` di root (main entry point)
2. Masuk ke `autonomous_system/` untuk fitur baru
3. Baca `GETTING_STARTED.txt` di autonomous_system

### Mau compile?
```bash
cd build
cmake ..
make
```

### Mau launch autonomous?
**Termudah:**
```bash
./start_autonomous.sh  # dari root
```

**Atau:**
```bash
cd autonomous_system
./run_autonomous.sh
```

### Mau lihat semua pilihan?
```bash
cd autonomous_system
./main_menu.sh
```

---

## ✅ Checklist

- ✅ File autonomous dikumpulkan di folder `autonomous_system/`
- ✅ File ArUco tetap di root (sudah ada sebelumnya)
- ✅ Quick launcher `start_autonomous.sh` di root
- ✅ README.md updated dengan path baru
- ✅ Dokumentasi lengkap di masing-masing lokasi

---

## 🎯 Entry Points

**Untuk pengguna baru:**
1. Baca `README.md` (root)
2. Buka folder `autonomous_system/`
3. Baca `GETTING_STARTED.txt`

**Untuk launch program:**
1. `./start_autonomous.sh` (root) ← Autonomous ⭐
2. `./run_aruco.sh` (root) ← Basic ArUco

**Untuk menu lengkap:**
1. `cd autonomous_system && ./main_menu.sh`

---

## 📊 File Summary

### Total Files:

**Autonomous System (folder):**
- 5 documentation files
- 3 launcher scripts
- 1 README index
= **9 files** in `autonomous_system/`

**Root Folder:**
- 6 documentation files (ArUco, Face, Logging, etc)
- 3 launcher scripts
- 2 Python scripts
- 1 main README
= **12 files** in root

**Source & Build:**
- 4 source files in `src/`
- 4 binaries in `build/`
- 20 markers in `aruco_markers/`

---

## 🚀 Quick Actions

```bash
# Launch autonomous (quick)
./start_autonomous.sh

# Main menu (all options)
cd autonomous_system && ./main_menu.sh

# Config IP camera
cd autonomous_system && ./config_ip_camera.sh

# Generate markers
python3 generate_aruco.py

# View autonomous docs
cd autonomous_system
cat GETTING_STARTED.txt
less README_AUTONOMOUS.md

# View ArUco docs
cat README_ARUCO.md
less ARUCO_COMPLETE_GUIDE.md

# Compile
cd build && cmake .. && make
```

---

## 📝 Notes

- **Folder `autonomous_system/`** berisi SEMUA file untuk fitur autonomous baru
- **Root folder** berisi file untuk fitur yang sudah ada (ArUco, Face Blur, dll)
- **Quick launcher** (`start_autonomous.sh`) di root untuk kemudahan akses
- **Main README** di root sebagai entry point utama

---

**Organized by:** ICAD Team  
**Date:** November 2025  
**Purpose:** Clean file organization untuk project yang lebih besar

---

*Untuk detail lengkap, baca README.md di masing-masing lokasi.*

