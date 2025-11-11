#!/bin/bash

# Quick Start Guide untuk ArUco Detection System
# ================================================

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║         ArUco Marker Detection System - Quick Start           ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""

# Cek apakah sudah di-compile
if [ ! -f "build/aruco_detection" ]; then
    echo "⚠️  Program belum di-compile!"
    echo "   Compiling sekarang..."
    cd build && cmake .. && make
    cd ..
    echo "✓ Compile selesai!"
    echo ""
fi

# Menu pilihan
echo "Pilih mode kamera:"
echo "1. IP Camera dari HP (default)"
echo "2. Webcam laptop (index 0)"
echo "3. Custom URL/path"
echo "4. Generate ArUco markers dulu"
echo "5. Keluar"
echo ""
read -p "Pilihan [1-5]: " choice

case $choice in
    1)
        echo ""
        echo "✓ Menjalankan dengan IP Camera..."
        echo "  Pastikan HP dan komputer terhubung ke WiFi yang sama!"
        echo ""
        cd build && ./aruco_detection
        ;;
    2)
        echo ""
        echo "✓ Menjalankan dengan Webcam laptop..."
        cd build && ./aruco_detection 0
        ;;
    3)
        echo ""
        read -p "Masukkan URL atau path kamera: " custom_path
        echo "✓ Menjalankan dengan: $custom_path"
        cd build && ./aruco_detection "$custom_path"
        ;;
    4)
        echo ""
        echo "✓ Generating ArUco markers..."
        python3 generate_aruco.py
        echo ""
        echo "✓ Markers tersimpan di folder 'aruco_markers/'"
        echo "  Buka folder tersebut dan print markers untuk testing"
        echo ""
        read -p "Jalankan program detection sekarang? (y/n): " run_now
        if [ "$run_now" = "y" ] || [ "$run_now" = "Y" ]; then
            cd build && ./aruco_detection
        fi
        ;;
    5)
        echo "Keluar..."
        exit 0
        ;;
    *)
        echo "Pilihan tidak valid!"
        exit 1
        ;;
esac
