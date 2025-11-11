## Fitur: Batas Garis Kuning

Modul deteksi objek hitam telah dihapus sesuai permintaan. Dokumen ini kini hanya menjelaskan fitur batas garis kuning.

### Batas Garis Kuning (Arena 3x3 m)
- Sistem memindai 20% area bawah frame untuk piksel kuning (HSV).
- Jika coverage > 2% → STOP; jika > 6% → MUNDUR 20 cm.
- Visual: kotak kuning di bawah frame + teks peringatan pada tampilan kamera.

### Uji Cepat
1. Jalankan:
   ```
   ./scripts/run_rover.sh
   ```
2. Gerakkan rover mendekati garis kuning sampai masuk area bawah frame; rover akan berhenti/mundur otomatis.


