#!/bin/bash

# ==============================================
# SEMI-AUTONOMOUS ROBOT NAVIGATION SYSTEM
# Script Launcher
# ==============================================

clear

echo "=========================================="
echo "  AUTONOMOUS ARUCO NAVIGATION SYSTEM"
echo "=========================================="
echo ""
echo "Robot Semi-Autonomous dengan Panduan Navigasi"
echo ""

# Warna
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Cek apakah executable ada
if [ ! -f "../build/autonomous_aruco" ]; then
    echo -e "${RED}ERROR: Program belum dikompilasi!${NC}"
    echo ""
    echo "Jalankan perintah berikut untuk compile:"
    echo "  cd ../build"
    echo "  cmake .."
    echo "  make autonomous_aruco"
    echo ""
    exit 1
fi

echo -e "${GREEN}✓ Program ditemukan!${NC}"
echo ""

# Menu pilihan
echo "Pilih sumber kamera:"
echo ""
echo "1) IP Camera HP (default)"
echo "2) Webcam Laptop (index 0)"
echo "3) Custom URL/Path"
echo "4) Keluar"
echo ""

read -p "Pilihan [1-4]: " choice

case $choice in
    1)
        echo ""
        echo -e "${YELLOW}Menggunakan IP Camera HP${NC}"
        echo ""
        echo "Pastikan:"
        echo "  - HP dan laptop terhubung ke WiFi yang sama"
        echo "  - Aplikasi IP Webcam sudah running di HP"
        echo "  - URL sudah dikonfigurasi di program"
        echo ""
        
        read -p "Tekan ENTER untuk melanjutkan atau Ctrl+C untuk batal..."
        
        cd ../build
        ./autonomous_aruco
        ;;
    
    2)
        echo ""
        echo -e "${YELLOW}Menggunakan Webcam Laptop${NC}"
        echo ""
        
        cd ../build
        ./autonomous_aruco 0
        ;;
    
    3)
        echo ""
        echo -e "${YELLOW}Custom Camera Source${NC}"
        echo ""
        echo "Contoh format:"
        echo "  - IP Camera: http://192.168.1.100:8080/video"
        echo "  - RTSP: rtsp://192.168.1.100:8554/stream"
        echo "  - Device: /dev/video0"
        echo "  - Index: 0, 1, 2, dst"
        echo ""
        
        read -p "Masukkan URL/Path: " custom_url
        
        if [ -z "$custom_url" ]; then
            echo -e "${RED}Error: URL tidak boleh kosong!${NC}"
            exit 1
        fi
        
        echo ""
        echo -e "${GREEN}Menghubungkan ke: $custom_url${NC}"
        echo ""
        
        cd ../build
        ./autonomous_aruco "$custom_url"
        ;;
    
    4)
        echo ""
        echo "Keluar dari program."
        echo ""
        exit 0
        ;;
    
    *)
        echo ""
        echo -e "${RED}Pilihan tidak valid!${NC}"
        echo ""
        exit 1
        ;;
esac

# Cleanup
echo ""
echo "=========================================="
echo "Program selesai!"
echo "=========================================="
echo ""

# Tampilkan info log
if [ -f "../build/autonomous_navigation_log.txt" ]; then
    echo -e "${GREEN}Log file tersedia:${NC} ../build/autonomous_navigation_log.txt"
    echo ""
    
    read -p "Lihat log sekarang? [y/n]: " show_log
    
    if [ "$show_log" = "y" ] || [ "$show_log" = "Y" ]; then
        echo ""
        echo "=========================================="
        echo "ISI LOG FILE"
        echo "=========================================="
        tail -n 50 ../build/autonomous_navigation_log.txt
        echo ""
    fi
fi

echo ""
echo "Terima kasih telah menggunakan Autonomous Navigation System!"
echo ""

