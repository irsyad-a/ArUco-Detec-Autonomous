## CALIBRATION GUIDE (Open-loop)

Tujuan: Menentukan `ms_per_cm` (jalan lurus) dan `ms_per_deg` (putar di tempat) untuk rover Anda.

Persiapan:
- Area datar, meteran, marker arah pada lantai.
- Firmware sudah terpasang.

1) Kalibrasi Jarak (ms_per_cm)
- Set nilai awal di `config/rover_config.yaml` (misal 80).
- Jalankan perintah:
  ```
  MOVE FWD D=50 V=150
  ```
- Ukur jarak nyata yang ditempuh (cm). Misal hasil 40 cm (target 50).
- Hitung skala:
  ```
  ms_per_cm_new = ms_per_cm_old * (target_cm / result_cm)
  ms_per_cm_new = 80 * (50/40) = 100
  ```
- Update `calibration.ms_per_cm`.

2) Kalibrasi Putaran (ms_per_deg)
- Tandai heading awal. Jalankan:
  ```
  TURN LEFT A=90 V=130
  ```
- Ukur sudut nyata. Misal baru 70°.
- Hitung:
  ```
  ms_per_deg_new = ms_per_deg_old * (target_deg / result_deg)
  ms_per_deg_new = 12 * (90/70) ≈ 15
  ```
- Update `calibration.ms_per_deg`.

3) Uji Ulang dan Simpan
- Ulangi hingga akurat.
- Nilai kalibrasi juga bisa dikirim runtime:
  ```
  CAL SET ms_per_cm=100
  CAL SET ms_per_deg=15
  ```

Catatan:
- Kecepatan (V) mempengaruhi hasil; kalibrasi pada kecepatan yang akan dipakai.
- Untuk akurasi tinggi, pertimbangkan encoder (closed-loop) di masa depan.


