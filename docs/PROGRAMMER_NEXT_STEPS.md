## Programmer Next Steps (Laptop + STM32)

Tujuan: Langkah-langkah setelah firmware STM32 siap menerima perintah `stm32`.

### 1) Setup Konfigurasi
Edit `config/rover_config.yaml`:
```
camera.url: "http://IP_HP:8080/video"
robot.port: "/dev/rfcomm0"      # atau USB-CDC /dev/ttyACM0
robot.baud: 115200
protocol.mode: "stm32"
speed.forward: 170
speed.back: 160
speed.turn: 150
calibration.ms_per_cm: 80
calibration.ms_per_deg: 12
```

### 2) Build & Run
```
./scripts/run_rover.sh
```
Atau override kamera:
```
./scripts/run_rover.sh http://IP_HP:8080/video
```

### 3) Mode Uji
- Vision only: pastikan instruksi berubah sesuai marker/garis kuning.
- Pseudo serial (tanpa robot): gunakan `socat` + `screen` untuk melihat string `F/B/TL/TR/S` yang dikirim.

### 4) Tuning
- Garis kuning: sesuaikan HSV/rasio di `src/autonomous_rover.cpp` (fungsi `detectYellowBoundaryAhead`).
- Kecepatan: ubah di `rover_config.yaml`.
- Kalibrasi open‑loop: ikuti `docs/CALIBRATION_GUIDE.md`.

### 5) Operasional Lapangan
1. Nyalakan kamera HP → pastikan `camera.url` benar.
2. Pair HC‑05 → `./scripts/setup_bluetooth.sh <BT_MAC> 1` → cek `/dev/rfcomm0`.
3. Jalankan program; STM32 akan menerima perintah format `stm32`.
4. Monitor log perintah (opsional) via pseudo‑serial atau serial monitor STM32.

### 6) Jika STM32 Pakai Format Lain
- Hubungi tim firmware untuk spesifikasi string mereka (mis. `GO <cm> <spd>`).
- Ubah mapping di `src/robot_bridge.cpp` pada cabang `useStm32Protocol` agar menghasilkan format tersebut.
- Tidak perlu mengubah modul deteksi/navigasi.


