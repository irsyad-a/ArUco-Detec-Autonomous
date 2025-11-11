#!/bin/bash

# ArUco Detection System - Interactive Menu
# Dengan Logging & Notification

clear

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║     ArUco Detection System - Logging & Notification           ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""
echo "✨ Fitur:"
echo "  • Auto-logging deteksi ke file"
echo "  • Modal notification saat ArUco terdeteksi"
echo "  • Smart detection (anti-spam)"
echo "  • Log viewer & analytics"
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

# Menu utama
echo "═══════════════════════════════════════════════════════════════"
echo "MAIN MENU"
echo "═══════════════════════════════════════════════════════════════"
echo ""
echo "1. 🎥 Run ArUco Detection (IP Camera)"
echo "2. 🎥 Run ArUco Detection (Webcam)"
echo "3. 🎥 Run ArUco Detection (Custom URL)"
echo "4. 📊 View Detection Log (Summary)"
echo "5. 📋 View Detection Log (Detailed)"
echo "6. 🔍 Search Log by Marker ID"
echo "7. 🎨 Generate ArUco Markers"
echo "8. 🗑️  Clear Log File"
echo "9. ❌ Exit"
echo ""
read -p "Pilih menu [1-9]: " choice

case $choice in
    1)
        echo ""
        echo "═══════════════════════════════════════════════════════════════"
        echo "🎥 Menjalankan dengan IP Camera..."
        echo "═══════════════════════════════════════════════════════════════"
        echo ""
        echo "Tips:"
        echo "  • Pastikan HP dan PC terhubung ke WiFi yang sama"
        echo "  • Aplikasi IP Camera di HP sudah running"
        echo "  • URL sudah dikonfigurasi di program"
        echo ""
        echo "Kontrol:"
        echo "  q/0 = Keluar | s = Screenshot | l = Log summary | g = Generate"
        echo ""
        read -p "Tekan Enter untuk mulai..."
        cd build && ./aruco_detection
        ;;
    2)
        echo ""
        echo "═══════════════════════════════════════════════════════════════"
        echo "🎥 Menjalankan dengan Webcam..."
        echo "═══════════════════════════════════════════════════════════════"
        echo ""
        cd build && ./aruco_detection 0
        ;;
    3)
        echo ""
        read -p "Masukkan URL atau path kamera: " custom_path
        echo ""
        echo "✓ Menjalankan dengan: $custom_path"
        cd build && ./aruco_detection "$custom_path"
        ;;
    4)
        echo ""
        if [ ! -f "aruco_detection_log.txt" ]; then
            echo "❌ File log belum ada!"
            echo "   Jalankan program dan deteksi beberapa marker dulu."
        else
            echo "📊 Loading log summary..."
            python3 view_log.py
        fi
        echo ""
        read -p "Tekan Enter untuk kembali..."
        ;;
    5)
        echo ""
        if [ ! -f "aruco_detection_log.txt" ]; then
            echo "❌ File log belum ada!"
        else
            echo "📋 Loading detailed log..."
            python3 view_log.py --detail
        fi
        echo ""
        read -p "Tekan Enter untuk kembali..."
        ;;
    6)
        echo ""
        if [ ! -f "aruco_detection_log.txt" ]; then
            echo "❌ File log belum ada!"
        else
            read -p "Masukkan Marker ID yang dicari: " marker_id
            echo ""
            python3 view_log.py --id "$marker_id"
        fi
        echo ""
        read -p "Tekan Enter untuk kembali..."
        ;;
    7)
        echo ""
        echo "═══════════════════════════════════════════════════════════════"
        echo "🎨 Generating ArUco Markers..."
        echo "═══════════════════════════════════════════════════════════════"
        python3 generate_aruco.py
        echo ""
        echo "✓ Markers tersimpan di folder 'aruco_markers/'"
        echo ""
        read -p "Jalankan detection sekarang? (y/n): " run_now
        if [ "$run_now" = "y" ] || [ "$run_now" = "Y" ]; then
            cd build && ./aruco_detection
        fi
        ;;
    8)
        echo ""
        if [ ! -f "aruco_detection_log.txt" ]; then
            echo "❌ File log tidak ditemukan!"
        else
            echo "⚠️  Anda akan menghapus semua log deteksi!"
            read -p "Lanjutkan? (y/n): " confirm
            if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
                # Backup dulu
                backup_name="aruco_log_backup_$(date +%Y%m%d_%H%M%S).txt"
                cp aruco_detection_log.txt "$backup_name"
                echo "✓ Backup disimpan: $backup_name"
                
                # Clear log file
                > aruco_detection_log.txt
                echo "✓ Log file telah dikosongkan"
            else
                echo "Batal menghapus log"
            fi
        fi
        echo ""
        read -p "Tekan Enter untuk kembali..."
        ;;
    9)
        echo ""
        echo "👋 Terima kasih!"
        exit 0
        ;;
    *)
        echo ""
        echo "❌ Pilihan tidak valid!"
        exit 1
        ;;
esac
