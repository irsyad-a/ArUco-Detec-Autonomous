#!/bin/bash

# Script untuk mengkonfigurasi IP Camera URL
# tanpa perlu edit kode manual

clear
echo "=========================================="
echo "  IP CAMERA CONFIGURATION TOOL"
echo "=========================================="
echo ""

# File yang akan diedit
SOURCE_FILE="../src/autonomous_aruco.cpp"

if [ ! -f "$SOURCE_FILE" ]; then
    echo "ERROR: File $SOURCE_FILE tidak ditemukan!"
    exit 1
fi

# Baca URL saat ini
CURRENT_URL=$(grep 'std::string ipCameraURL' "$SOURCE_FILE" | sed 's/.*"\(.*\)".*/\1/')

echo "URL saat ini: $CURRENT_URL"
echo ""
echo "Masukkan URL IP Camera baru:"
echo "(Contoh: http://192.168.1.100:8080/video)"
echo ""

read -p "URL Baru: " NEW_URL

if [ -z "$NEW_URL" ]; then
    echo ""
    echo "ERROR: URL tidak boleh kosong!"
    exit 1
fi

# Backup file
cp "$SOURCE_FILE" "${SOURCE_FILE}.backup"
echo ""
echo "✓ Backup dibuat: ${SOURCE_FILE}.backup"

# Update URL
sed -i "s|std::string ipCameraURL = \".*\";|std::string ipCameraURL = \"$NEW_URL\";|" "$SOURCE_FILE"

echo "✓ URL berhasil diupdate ke: $NEW_URL"
echo ""
echo "Recompiling program..."
echo ""

cd ../build
cmake .. > /dev/null 2>&1
make autonomous_aruco

if [ $? -eq 0 ]; then
    echo ""
    echo "=========================================="
    echo "  ✓ KONFIGURASI SELESAI!"
    echo "=========================================="
    echo ""
    echo "URL baru: $NEW_URL"
    echo ""
    echo "Jalankan program dengan:"
    echo "  cd build && ./autonomous_aruco"
    echo ""
    echo "Atau gunakan launcher:"
    echo "  ./run_autonomous.sh"
    echo ""
else
    echo ""
    echo "ERROR: Compile gagal!"
    echo ""
    echo "Mengembalikan file asli..."
    mv "${SOURCE_FILE}.backup" "$SOURCE_FILE"
    echo "File dikembalikan ke kondisi awal."
    exit 1
fi

