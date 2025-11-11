# ArUco Marker Detection System

## 📋 Deskripsi

Program ini dapat mendeteksi **ArUco markers** secara real-time menggunakan kamera (IP Camera dari HP atau webcam). ArUco markers adalah marker persegi hitam-putih yang mirip QR code, sangat berguna untuk:

- Augmented Reality (AR)
- Pose estimation
- Tracking objek
- Kalibrasi kamera
- Robot navigation

## 🚀 Cara Menggunakan

### 1. Jalankan Program

**Dengan IP Camera (Default):**

```bash
cd /home/icad/Downloads/magang/build
./aruco_detection
```

**Dengan Webcam Laptop:**

```bash
./aruco_detection 0
```

**Dengan Custom IP Camera:**

```bash
./aruco_detection http://192.168.1.100:8080/video
```

### 2. Kontrol Keyboard

| Tombol       | Fungsi                             |
| ------------ | ---------------------------------- |
| `q` atau `0` | Keluar dari program                |
| `s`          | Screenshot frame saat ini          |
| `g`          | Generate 10 ArUco markers (ID 0-9) |

## 🎯 Generate ArUco Markers

### Cara 1: Dalam Program

1. Jalankan program `./aruco_detection`
2. Tekan tombol `g`
3. Program akan membuat 10 file: `aruco_marker_0.png` sampai `aruco_marker_9.png`
4. Print file-file tersebut

### Cara 2: Generate Manual dengan Python

Jika ingin generate lebih banyak markers:

```python
import cv2
import cv2.aruco as aruco

# Pilih dictionary
dictionary = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)

# Generate markers ID 0-49
for i in range(50):
    marker_image = aruco.drawMarker(dictionary, i, 200)
    cv2.imwrite(f'aruco_marker_{i}.png', marker_image)
    print(f'Generated marker {i}')
```

### Cara 3: Generate Online

Website untuk generate ArUco markers:

- https://chev.me/arucogen/
- Pilih Dictionary: **4x4 (50 markers)**
- Marker ID: 0-49
- Marker Size: 200mm atau lebih besar
- Download dan print

## 📱 Setup IP Camera dari HP

1. **Download Aplikasi:**

   - Android: "IP Webcam" dari Play Store
   - iOS: "IP Camera Lite" atau "EpocCam"

2. **Aktifkan Server:**

   - Buka aplikasi di HP
   - Tekan "Start Server"
   - Catat URL yang muncul (contoh: `http://10.237.86.79:8080/video`)

3. **Update URL di Program:**

   - Edit file `src/aruco_detection.cpp`
   - Cari baris: `std::string ipCameraURL = "http://10.237.86.79:8080/video";`
   - Ganti dengan URL dari HP Anda
   - Compile ulang: `cd build && make`

4. **Jalankan:**
   ```bash
   ./aruco_detection
   ```

## 🔧 Troubleshooting

### Program tidak bisa detect ArUco

- ✅ Pastikan marker dicetak dengan jelas (hitam pekat, putih bersih)
- ✅ Marker tidak terlipat atau rusak
- ✅ Pencahayaan cukup terang
- ✅ Kamera fokus (tidak blur)
- ✅ Marker tidak terlalu kecil di frame

### IP Camera tidak terhubung

- ✅ HP dan komputer di WiFi yang sama
- ✅ URL sudah benar (cek di aplikasi HP)
- ✅ Firewall tidak memblokir koneksi
- ✅ Coba URL alternatif:
  - `/video` → `/videofeed`
  - Port `8080` → `4747` (untuk DroidCam)

### Compile error

```bash
# Install OpenCV contrib jika belum ada
sudo apt-get install libopencv-dev libopencv-contrib-dev

# Recompile
cd build
cmake ..
make
```

## 📊 Informasi Deteksi

Program akan menampilkan:

- **ID Marker**: Angka identifikasi (0-49)
- **Size**: Ukuran marker dalam pixel
- **Center**: Koordinat pusat marker (x, y)
- **Corners**: 4 sudut marker (visualisasi)

### Contoh Output:

```
✓ Marker ID 5 terdeteksi | Center: (320, 240) | Size: 150px
✓ Marker ID 12 terdeteksi | Center: (450, 300) | Size: 120px
```

## 🎨 ArUco Dictionary yang Digunakan

Program menggunakan **DICT_4X4_50**:

- Grid: 4×4 bit
- Total markers: 50 (ID: 0-49)
- Cocok untuk: Aplikasi sederhana dengan markers terbatas

### Dictionary Lainnya (jika perlu diganti):

| Dictionary   | Grid | Total Markers |
| ------------ | ---- | ------------- |
| DICT_4X4_50  | 4×4  | 50            |
| DICT_4X4_100 | 4×4  | 100           |
| DICT_5X5_50  | 5×5  | 50            |
| DICT_6X6_250 | 6×6  | 250           |

## 💡 Tips & Tricks

1. **Untuk Deteksi Terbaik:**

   - Print marker minimal ukuran 5×5 cm
   - Gunakan kertas putih bersih
   - Tinta hitam pekat
   - Laminating untuk durability

2. **Multiple Markers:**

   - Program bisa detect beberapa marker sekaligus
   - Setiap marker akan diberi label berbeda
   - Berguna untuk tracking multiple objects

3. **Augmented Reality:**
   - Gunakan marker corners untuk pose estimation
   - Bisa tambahkan rendering 3D di atas marker
   - Combine dengan OpenGL untuk AR effects

## 📝 Edit Konfigurasi

Edit file `src/aruco_detection.cpp` untuk:

```cpp
// Ganti IP Camera
std::string ipCameraURL = "http://YOUR_IP:8080/video";

// Ganti Dictionary
cv::Ptr<cv::aruco::Dictionary> dictionary =
    cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

// Ganti jumlah markers untuk generate
int numMarkers = 20;  // Generate 20 markers
```

Setelah edit, compile ulang:

```bash
cd build && make
```

## 🎓 Referensi

- OpenCV ArUco Documentation: https://docs.opencv.org/4.x/d5/dae/tutorial_aruco_detection.html
- ArUco Original Paper: https://www.uco.es/investiga/grupos/ava/node/26
- Online Generator: https://chev.me/arucogen/

## ⚙️ Requirements

- OpenCV 4.x dengan module contrib (aruco)
- C++ compiler (g++)
- CMake 3.0+
- Camera (IP Camera atau Webcam)

---

**Dibuat oleh:** ICAD Team  
**Tanggal:** November 2025  
**Program:** `aruco_detection`
