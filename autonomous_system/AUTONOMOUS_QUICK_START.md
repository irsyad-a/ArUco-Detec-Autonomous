# 🚀 QUICK START - Autonomous Navigation System

## ⚡ 3 Langkah Mudah

### 1️⃣ Setup IP Camera HP

**Android:**
1. Download "IP Webcam" dari Play Store
2. Buka app → Scroll down → Tekan **"Start server"**
3. Catat URL (contoh: `http://10.237.86.79:8080/video`)

**iOS:**
1. Download "IP Camera Lite" dari App Store  
2. Start → Catat URL

💡 **Penting:** HP dan laptop harus di WiFi yang sama!

---

### 2️⃣ Generate ArUco Markers

```bash
cd /home/icad/Downloads/magang
python3 generate_aruco.py
```

Print marker dari folder `aruco_markers/` (minimal 5 marker).

---

### 3️⃣ Jalankan Program

```bash
./run_autonomous.sh
```

Pilih opsi 1 (IP Camera HP) atau 2 (Webcam).

---

## 🎮 Cara Main

### Anda adalah ROBOT! 🤖

1. **Lihat Navigation Panel** (window kanan) untuk instruksi
2. **Ikuti instruksi** yang ditampilkan:
   - `PUTAR KIRI` → Putar badan/kamera ke kiri
   - `PUTAR KANAN` → Putar ke kanan
   - `MAJU 15 cm` → Maju beberapa langkah
   - `LOCKED!` → Tekan tombol **'S'** untuk scan

3. **Repeat** sampai semua marker terdeteksi!

---

## 📊 Status Robot

| Warna | Status | Artinya |
|-------|--------|---------|
| 🟠 Orange | SCANNING | Mencari marker |
| 🟡 Yellow | TRACKING | Mengikuti marker |
| 🔵 Cyan | APPROACHING | Mendekati marker |
| 🟢 Green | LOCKED | Siap scan! |

---

## ⌨️ Kontrol

- `S` → Scan marker (saat LOCKED)
- `R` → Reset target (cari target baru)
- `Q` → Keluar

---

## 💡 Tips

✅ **Gerakan smooth dan perlahan**  
✅ **Tunggu 1-2 detik antara gerakan**  
✅ **Jaga kamera stabil**  
✅ **Pencahayaan cukup terang**  
✅ **Print marker minimal 5x5 cm**

---

## ❌ Troubleshooting Cepat

**IP Camera tidak connect?**
- Cek WiFi (HP & laptop harus sama)
- Test URL di browser: `http://IP:8080/video`
- Restart aplikasi di HP

**Marker tidak terdeteksi?**
- Print ulang marker (hitam pekat, putih bersih)
- Tambah cahaya
- Jarak 20-200 cm dari marker

**Program error?**
```bash
cd /home/icad/Downloads/magang/build
rm -rf *
cmake ..
make autonomous_aruco
```

---

## 📖 Dokumentasi Lengkap

Baca `README_AUTONOMOUS.md` untuk detail lengkap!

---

**Selamat mencoba! 🎉**

