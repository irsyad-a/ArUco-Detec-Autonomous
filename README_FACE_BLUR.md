# Face Blur + Black Object Detection

Program deteksi objek hitam dengan fitur **Face Blur menggunakan AI/ML (Haar Cascade)** dan **Panel Informasi Lengkap**.

## 📋 Fitur Utama

### 1. **Face Blur dengan AI/ML (Haar Cascade)**

- Menggunakan Haar Cascade Classifier (Machine Learning model)
- Deteksi wajah secara real-time
- Blur otomatis pada area wajah dengan Gaussian Blur
- Toggle ON/OFF untuk blur wajah
- Padding area untuk coverage lebih baik

### 2. **Deteksi Objek Hitam**

- Deteksi objek berdasarkan warna hitam (HSV color space)
- Filter multi-kriteria: area, perimeter, circularity, aspect ratio
- Bounding box hijau dengan label dan informasi area
- Counting objek yang terdeteksi

### 3. **Panel Informasi Lengkap (Pojok Kiri Atas)**

- **FPS (Frame Per Second)**: Performa real-time
- **Resolusi**: Ukuran frame video
- **Frame Count**: Total frame yang diproses
- **Wajah Terdeteksi**: Jumlah wajah yang terdeteksi
- **Status Blur**: ON/OFF untuk blur wajah
- **Objek Hitam**: Jumlah objek hitam yang terdeteksi
- **Total Area**: Total area objek hitam dalam pixel
- Background semi-transparan untuk readability

### 4. **Statistik Session**

- Total frame diproses
- Durasi session
- Rata-rata FPS

## 🎮 Kontrol Keyboard

| Tombol         | Fungsi                    |
| -------------- | ------------------------- |
| `q` atau `ESC` | Keluar dari program       |
| `s`            | Screenshot (simpan frame) |
| `b`            | Toggle blur wajah ON/OFF  |

## 🚀 Cara Menjalankan

### 1. **Default (Camera Index 0)**

```bash
./build/face_blur_detection
```

### 2. **Camera Index Tertentu**

```bash
./build/face_blur_detection 0    # Kamera pertama
./build/face_blur_detection 1    # Kamera kedua
```

### 3. **Path Langsung ke Device**

```bash
./build/face_blur_detection /dev/video0
./build/face_blur_detection /dev/video1
```

## 🔧 Compile

```bash
cd /home/icad/Downloads/magang/build
cmake ..
make
```

## 📊 Parameter Deteksi Objek Hitam

### Range Warna HSV

- **Lower**: `(0, 0, 0)`
- **Upper**: `(180, 255, 40)` - Value 40 untuk hitam murni

### Filter Kriteria

- **Min Area**: 1200 pixel
- **Min Perimeter**: 100 pixel
- **Min Circularity**: 0.25
- **Aspect Ratio**: 0.3 - 3.0

### Morphological Operations

- **Kernel**: Ellipse 7x7, 9x9
- **Iterations**: 2x untuk pembersihan maksimal

## 🤖 AI/ML Model

### Haar Cascade Classifier

- **Model**: `haarcascade_frontalface_default.xml`
- **Path**: `/usr/share/opencv4/haarcascades/`
- **Algoritma**: Cascade of boosted classifiers (Viola-Jones)
- **Trained on**: Ribuan gambar wajah positif dan negatif
- **Detection**: Real-time face detection

### Install Model (jika belum ada)

```bash
sudo apt install opencv-data
```

## 📸 Output Windows

1. **"Face Blur + Black Object Detection"**: Window utama dengan:

   - Wajah yang di-blur (kotak biru)
   - Objek hitam dengan bounding box (kotak hijau)
   - Panel informasi di pojok kiri atas

2. **"Mask - Deteksi Hitam"**: Binary mask untuk deteksi warna hitam

## 🎯 Use Cases

- **Privacy Protection**: Blur wajah untuk privasi dalam recording
- **Object Tracking**: Deteksi dan tracking objek hitam
- **Surveillance**: Monitoring dengan face privacy
- **Industrial**: Quality control untuk objek gelap/hitam
- **Research**: Computer vision experiments

## 📝 File Struktur

```
magang/
├── CMakeLists.txt              # Build configuration
├── src/
│   ├── cobaa.cpp               # Program original (deteksi hitam saja)
│   └── face_blur_detection.cpp # Program baru (face blur + deteksi hitam)
└── build/
    ├── cobaAja_kj              # Executable 1
    └── face_blur_detection     # Executable 2 (NEW!)
```

## 🔍 Perbedaan dengan Program Sebelumnya

| Fitur               | cobaa.cpp | face_blur_detection.cpp |
| ------------------- | --------- | ----------------------- |
| Deteksi Objek Hitam | ✅        | ✅                      |
| Face Blur (AI/ML)   | ❌        | ✅                      |
| Panel Info Lengkap  | ❌        | ✅                      |
| FPS Counter         | ❌        | ✅                      |
| Frame Counter       | ❌        | ✅                      |
| Resolution Info     | ❌        | ✅                      |
| Total Area          | ❌        | ✅                      |
| Toggle Blur         | ❌        | ✅                      |
| Session Statistics  | ❌        | ✅                      |

## 💡 Tips

1. **Pencahayaan**: Face detection bekerja lebih baik dengan pencahayaan cukup
2. **Jarak**: Wajah harus dalam jarak yang wajar dari kamera
3. **Performance**: Blur wajah menambah beban CPU, normal jika FPS turun sedikit
4. **Multiple Faces**: Dapat mendeteksi dan blur multiple faces sekaligus

## 🐛 Troubleshooting

### Haar Cascade tidak ditemukan

```bash
# Install opencv-data
sudo apt install opencv-data

# Atau cari lokasi file
find /usr -name "haarcascade_frontalface_default.xml"
```

### Kamera tidak terbuka

```bash
# Cek kamera tersedia
ls /dev/video*

# Test dengan index berbeda
./build/face_blur_detection 1
```

## 📚 Teknologi yang Digunakan

- **C++**: Programming language
- **OpenCV 4**: Computer vision library
- **Haar Cascade**: ML model untuk face detection
- **HSV Color Space**: Untuk deteksi warna
- **Morphological Operations**: Noise reduction
- **Gaussian Blur**: Face privacy protection

## 🎓 Konsep AI/ML

**Haar Cascade** adalah salah satu metode Machine Learning klasik untuk object detection:

- **Training**: Model dilatih dengan ribuan gambar
- **Features**: Menggunakan Haar-like features
- **Cascade**: Bertingkat untuk efisiensi
- **Real-time**: Cukup cepat untuk aplikasi real-time

---

**Author**: GitHub Copilot
**Date**: October 20, 2025
**Version**: 1.0
