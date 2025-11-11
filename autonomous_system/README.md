# 🤖 Autonomous ArUco Navigation System

**Folder ini berisi semua file untuk Sistem Navigasi Semi-Autonomous yang baru dibuat!**

---

## 📁 Isi Folder

### 📖 **Dokumentasi**

| File | Deskripsi | Ukuran |
|------|-----------|--------|
| **GETTING_STARTED.txt** | ⭐ **START HERE!** Panduan lengkap untuk memulai | 16 KB |
| **README_AUTONOMOUS.md** | Full documentation dengan semua detail teknis | 15 KB |
| **AUTONOMOUS_QUICK_START.md** | Quick start guide (3 langkah mudah) | 2 KB |
| **SYSTEM_OVERVIEW.md** | Overview lengkap sistem (semua program) | 11 KB |

### 🚀 **Launcher Scripts**

| File | Deskripsi |
|------|-----------|
| **run_autonomous.sh** | Launcher untuk autonomous navigation |
| **main_menu.sh** | Main menu untuk semua program |
| **config_ip_camera.sh** | Tool untuk config IP camera |

---

## 🚀 Quick Start

### Cara Tercepat Memulai:

```bash
# 1. Generate markers
cd /home/icad/Downloads/magang
python3 generate_aruco.py

# 2. Launch program
cd autonomous_system
./run_autonomous.sh
```

### Atau Gunakan Main Menu:

```bash
cd /home/icad/Downloads/magang/autonomous_system
./main_menu.sh
```

---

## 📖 Urutan Membaca Dokumentasi

**Untuk pengguna baru:**

1. **GETTING_STARTED.txt** ← Mulai di sini!
   ```bash
   cat GETTING_STARTED.txt
   ```
   
2. **AUTONOMOUS_QUICK_START.md** ← 3 langkah mudah
   ```bash
   less AUTONOMOUS_QUICK_START.md
   ```

3. **README_AUTONOMOUS.md** ← Full guide (jika butuh detail)
   ```bash
   less README_AUTONOMOUS.md
   ```

4. **SYSTEM_OVERVIEW.md** ← Overview semua program
   ```bash
   less SYSTEM_OVERVIEW.md
   ```

---

## 🎯 Apa itu Autonomous Navigation System?

Sistem navigasi **semi-autonomous** yang menggunakan **ArUco markers** untuk memandu operator secara cerdas.

### Cara Kerja:
1. 🟠 **SCANNING** - Sistem mencari marker
2. 🟡 **TRACKING** - Mengikuti marker yang terdeteksi  
3. 🔵 **APPROACHING** - Mendekati marker
4. 🟢 **LOCKED** - Siap untuk scan! (Tekan 'S')

### Fitur Utama:
- ✅ Auto-scanning mode
- ✅ Smart navigation (kiri/kanan/maju/mundur)
- ✅ IP Camera support (gunakan HP)
- ✅ Dual display (Camera view + Navigation panel)
- ✅ Real-time guidance
- ✅ Logging system

---

## ⚡ Command Reference

### Launch Program:
```bash
./run_autonomous.sh              # Interactive launcher
cd ../build && ./autonomous_aruco  # Direct run dengan IP camera default
cd ../build && ./autonomous_aruco 0  # Direct run dengan webcam
```

### Tools:
```bash
./config_ip_camera.sh            # Configure IP camera URL
./main_menu.sh                   # Main menu (recommended!)
```

### Compile (jika ada perubahan):
```bash
cd ../build
cmake ..
make autonomous_aruco
```

---

## 🎮 Cara Menggunakan

1. **Jalankan program:**
   ```bash
   ./run_autonomous.sh
   ```

2. **Dua window akan muncul:**
   - **Camera View** - Feed kamera dengan detection
   - **Navigation Panel** - Instruksi navigasi BESAR

3. **Ikuti instruksi di Navigation Panel:**
   - "PUTAR KIRI 20°" → Putar kamera ke kiri
   - "MAJU 15 cm" → Maju beberapa langkah
   - "LOCKED!" → Tekan 'S' untuk scan

4. **Keyboard controls:**
   - `S` → Scan marker (saat LOCKED)
   - `R` → Reset target
   - `Q` → Keluar

---

## 📦 Struktur File

```
autonomous_system/
│
├── 📖 README.md                    ← File ini
├── 📄 GETTING_STARTED.txt          ← ⭐ Start here!
├── 📄 README_AUTONOMOUS.md         ← Full guide
├── 📄 AUTONOMOUS_QUICK_START.md    ← Quick start
├── 📄 SYSTEM_OVERVIEW.md           ← System overview
│
├── 🚀 run_autonomous.sh            ← Launcher
├── 🚀 main_menu.sh                 ← Main menu
└── 🚀 config_ip_camera.sh          ← Config tool
```

---

## 🔗 File Program Utama

Program executable dan source code ada di folder parent:

- **Source code:** `../src/autonomous_aruco.cpp`
- **Executable:** `../build/autonomous_aruco`
- **Log file:** `../build/autonomous_navigation_log.txt`

---

## 💡 Tips Cepat

✅ **Baca GETTING_STARTED.txt terlebih dahulu!**  
✅ Generate markers dengan: `cd .. && python3 generate_aruco.py`  
✅ Setup IP camera dengan: `./config_ip_camera.sh`  
✅ Gunakan main menu untuk akses semua: `./main_menu.sh`  
✅ Pencahayaan ruangan harus cukup terang  
✅ Print marker minimal 5x5 cm  

---

## 🐛 Troubleshooting

**IP Camera tidak connect?**
```bash
./config_ip_camera.sh  # Update URL
```

**Program belum dikompilasi?**
```bash
cd ../build
cmake .. && make autonomous_aruco
```

**Marker tidak terdeteksi?**
- Tambah cahaya
- Print ulang marker dengan quality bagus
- Jarak 20-200 cm

**Butuh bantuan?**
- Baca: `GETTING_STARTED.txt`
- Baca: `README_AUTONOMOUS.md`

---

## 🎯 Next Steps

1. **Baca dokumentasi:**
   ```bash
   cat GETTING_STARTED.txt
   ```

2. **Generate markers:**
   ```bash
   cd .. && python3 generate_aruco.py
   ```

3. **Launch:**
   ```bash
   ./run_autonomous.sh
   ```

**Selamat mencoba! 🚀**

---

## 📞 Info Lebih Lanjut

- **Developer:** ICAD Team
- **Date:** November 2025
- **Version:** 2.0.0
- **Program:** `autonomous_aruco`

**Made with ❤️ for autonomous robotics research and education**

---

**🎉 Selamat menggunakan Autonomous Navigation System!**

