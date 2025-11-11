#!/bin/bash

# ==============================================
# MAIN MENU - Magang Project
# Unified launcher untuk semua program
# ==============================================

# Warna
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
BOLD='\033[1m'
NC='\033[0m' # No Color

clear

# Banner
echo -e "${CYAN}${BOLD}"
echo "╔════════════════════════════════════════════════════════════╗"
echo "║                                                            ║"
echo "║         MAGANG PROJECT - COMPUTER VISION SYSTEM            ║"
echo "║                                                            ║"
echo "║                    🤖 🎯 😊 ⚫                            ║"
echo "║                                                            ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo -e "${NC}"

echo ""
echo -e "${BOLD}Pilih Program yang ingin dijalankan:${NC}"
echo ""
echo -e "${MAGENTA}${BOLD}═══════════════════════════════════════════════════════════${NC}"
echo ""

# Main Programs
echo -e "${GREEN}${BOLD}PROGRAM UTAMA:${NC}"
echo ""
echo -e "  ${YELLOW}1)${NC} 🤖 ${BOLD}Autonomous ArUco Navigation${NC} ${CYAN}⭐ RECOMMENDED${NC}"
echo "     └─ Semi-autonomous robot dengan panduan navigasi cerdas"
echo ""
echo -e "  ${YELLOW}2)${NC} 🎯 ${BOLD}Basic ArUco Detection${NC}"
echo "     └─ Deteksi ArUco marker dengan logging & notification"
echo ""
echo -e "  ${YELLOW}3)${NC} 😊 ${BOLD}Face Blur Detection${NC}"
echo "     └─ Deteksi wajah dengan efek blur otomatis"
echo ""
echo -e "  ${YELLOW}4)${NC} ⚫ ${BOLD}Black Object Detection${NC}"
echo "     └─ Deteksi objek berwarna hitam"
echo ""

echo -e "${MAGENTA}═══════════════════════════════════════════════════════════${NC}"
echo ""

# Tools & Utilities
echo -e "${BLUE}${BOLD}TOOLS & UTILITIES:${NC}"
echo ""
echo -e "  ${YELLOW}5)${NC} 🔧 Configure IP Camera URL"
echo -e "  ${YELLOW}6)${NC} 🎨 Generate ArUco Markers"
echo -e "  ${YELLOW}7)${NC} 📊 View Detection Logs"
echo -e "  ${YELLOW}8)${NC} 🔨 Compile All Programs"
echo -e "  ${YELLOW}9)${NC} 📖 Show Documentation"
echo ""

echo -e "${MAGENTA}═══════════════════════════════════════════════════════════${NC}"
echo ""
echo -e "  ${YELLOW}0)${NC} ${RED}${BOLD}Exit${NC}"
echo ""
echo -e "${MAGENTA}═══════════════════════════════════════════════════════════${NC}"
echo ""

read -p "$(echo -e ${BOLD}Pilihan Anda [0-9]: ${NC})" choice

echo ""

case $choice in
    1)
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${GREEN}${BOLD}  🤖 AUTONOMOUS ARUCO NAVIGATION SYSTEM${NC}"
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        
        if [ ! -f "../build/autonomous_aruco" ]; then
            echo -e "${RED}ERROR: Program belum dikompilasi!${NC}"
            echo ""
            read -p "Compile sekarang? [y/n]: " compile_choice
            if [ "$compile_choice" = "y" ] || [ "$compile_choice" = "Y" ]; then
                cd ../build
                cmake .. && make autonomous_aruco
                cd ../autonomous_system
                echo ""
            else
                exit 1
            fi
        fi
        
        echo -e "${CYAN}Launching autonomous navigation...${NC}"
        echo ""
        sleep 1
        ./run_autonomous.sh
        ;;
    
    2)
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${GREEN}${BOLD}  🎯 BASIC ARUCO DETECTION${NC}"
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        
        if [ ! -f "../build/aruco_detection" ]; then
            echo -e "${RED}ERROR: Program belum dikompilasi!${NC}"
            echo ""
            read -p "Compile sekarang? [y/n]: " compile_choice
            if [ "$compile_choice" = "y" ] || [ "$compile_choice" = "Y" ]; then
                cd ../build
                cmake .. && make aruco_detection
                cd ../autonomous_system
                echo ""
            else
                exit 1
            fi
        fi
        
        echo -e "${CYAN}Launching ArUco detection...${NC}"
        echo ""
        sleep 1
        ../run_aruco.sh
        ;;
    
    3)
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${GREEN}${BOLD}  😊 FACE BLUR DETECTION${NC}"
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        
        if [ ! -f "../build/face_blur_detection" ]; then
            echo -e "${RED}ERROR: Program belum dikompilasi!${NC}"
            echo ""
            read -p "Compile sekarang? [y/n]: " compile_choice
            if [ "$compile_choice" = "y" ] || [ "$compile_choice" = "Y" ]; then
                cd ../build
                cmake .. && make face_blur_detection
                cd ../autonomous_system
                echo ""
            else
                exit 1
            fi
        fi
        
        echo -e "${CYAN}Launching face blur detection...${NC}"
        echo ""
        echo "Pilih camera source:"
        echo "1) Webcam (0)"
        echo "2) IP Camera"
        echo ""
        read -p "Pilihan [1-2]: " cam_choice
        
        if [ "$cam_choice" = "1" ]; then
            cd ../build && ./face_blur_detection 0
        elif [ "$cam_choice" = "2" ]; then
            read -p "Masukkan URL IP Camera: " ip_url
            cd ../build && ./face_blur_detection "$ip_url"
        fi
        ;;
    
    4)
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${GREEN}${BOLD}  ⚫ BLACK OBJECT DETECTION${NC}"
        echo -e "${GREEN}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        
        if [ ! -f "../build/cobaAja_kj" ]; then
            echo -e "${RED}ERROR: Program belum dikompilasi!${NC}"
            echo ""
            read -p "Compile sekarang? [y/n]: " compile_choice
            if [ "$compile_choice" = "y" ] || [ "$compile_choice" = "Y" ]; then
                cd ../build
                cmake .. && make cobaAja_kj
                cd ../autonomous_system
                echo ""
            else
                exit 1
            fi
        fi
        
        echo -e "${CYAN}Launching black object detection...${NC}"
        echo ""
        sleep 1
        cd ../build && ./cobaAja_kj
        ;;
    
    5)
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${BLUE}${BOLD}  🔧 CONFIGURE IP CAMERA${NC}"
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        ./config_ip_camera.sh
        ;;
    
    6)
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${BLUE}${BOLD}  🎨 GENERATE ARUCO MARKERS${NC}"
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        
        if [ ! -f "../generate_aruco.py" ]; then
            echo -e "${RED}ERROR: generate_aruco.py tidak ditemukan!${NC}"
            exit 1
        fi
        
        echo -e "${CYAN}Generating ArUco markers...${NC}"
        echo ""
        cd .. && python3 generate_aruco.py
        echo ""
        echo -e "${GREEN}✓ Selesai!${NC}"
        echo ""
        echo "Markers tersimpan di folder: aruco_markers/"
        echo ""
        read -p "Tekan ENTER untuk kembali..."
        ;;
    
    7)
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${BLUE}${BOLD}  📊 VIEW DETECTION LOGS${NC}"
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        
        echo "Pilih log file:"
        echo ""
        echo "1) Autonomous Navigation Log"
        echo "2) Basic ArUco Detection Log"
        echo "3) View dengan Python viewer"
        echo ""
        read -p "Pilihan [1-3]: " log_choice
        
        case $log_choice in
            1)
                if [ -f "../build/autonomous_navigation_log.txt" ]; then
                    echo ""
                    echo -e "${CYAN}${BOLD}=== AUTONOMOUS NAVIGATION LOG ===${NC}"
                    echo ""
                    tail -n 100 ../build/autonomous_navigation_log.txt
                else
                    echo ""
                    echo -e "${RED}Log file tidak ditemukan!${NC}"
                fi
                ;;
            2)
                if [ -f "../build/aruco_detection_log.txt" ]; then
                    echo ""
                    echo -e "${CYAN}${BOLD}=== ARUCO DETECTION LOG ===${NC}"
                    echo ""
                    tail -n 100 ../build/aruco_detection_log.txt
                else
                    echo ""
                    echo -e "${RED}Log file tidak ditemukan!${NC}"
                fi
                ;;
            3)
                if [ -f "../view_log.py" ]; then
                    cd .. && python3 view_log.py
                else
                    echo ""
                    echo -e "${RED}view_log.py tidak ditemukan!${NC}"
                fi
                ;;
        esac
        
        echo ""
        read -p "Tekan ENTER untuk kembali..."
        ;;
    
    8)
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${BLUE}${BOLD}  🔨 COMPILE ALL PROGRAMS${NC}"
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        
        echo -e "${YELLOW}Compiling all programs...${NC}"
        echo ""
        
        cd ../build
        cmake ..
        echo ""
        make
        
        if [ $? -eq 0 ]; then
            echo ""
            echo -e "${GREEN}${BOLD}✓ Compile berhasil!${NC}"
            echo ""
            echo "Programs yang berhasil dikompilasi:"
            echo "  ✓ autonomous_aruco"
            echo "  ✓ aruco_detection"
            echo "  ✓ face_blur_detection"
            echo "  ✓ cobaAja_kj"
        else
            echo ""
            echo -e "${RED}${BOLD}✗ Compile gagal!${NC}"
            echo ""
            echo "Coba jalankan:"
            echo "  sudo apt-get install -y libopencv-dev libopencv-contrib-dev"
        fi
        
        cd ../autonomous_system
        echo ""
        read -p "Tekan ENTER untuk kembali..."
        ;;
    
    9)
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo -e "${BLUE}${BOLD}  📖 DOCUMENTATION${NC}"
        echo -e "${BLUE}${BOLD}═══════════════════════════════════════════════════════════${NC}"
        echo ""
        
        echo "Available documentation:"
        echo ""
        echo -e "${GREEN}Autonomous Navigation:${NC}"
        echo "  - README_AUTONOMOUS.md (full guide)"
        echo "  - AUTONOMOUS_QUICK_START.md (quick start)"
        echo ""
        echo -e "${GREEN}ArUco Detection:${NC}"
        echo "  - README_ARUCO.md (full guide)"
        echo "  - ARUCO_QUICK_START.md (quick start)"
        echo "  - ARUCO_COMPLETE_GUIDE.md (complete)"
        echo ""
        echo -e "${GREEN}Other:${NC}"
        echo "  - README_FACE_BLUR.md"
        echo "  - LOGGING_FEATURE.md"
        echo "  - SYSTEM_OVERVIEW.md (overview)"
        echo ""
        
        read -p "Buka file? (ketik nama file atau tekan ENTER untuk kembali): " doc_file
        
        if [ -n "$doc_file" ] && [ -f "$doc_file" ]; then
            less "$doc_file"
        fi
        ;;
    
    0)
        echo -e "${CYAN}Terima kasih! Sampai jumpa! 👋${NC}"
        echo ""
        exit 0
        ;;
    
    *)
        echo -e "${RED}Pilihan tidak valid!${NC}"
        echo ""
        sleep 2
        exec ./main_menu.sh
        ;;
esac

echo ""
echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
echo ""

read -p "Kembali ke main menu? [y/n]: " back_choice

if [ "$back_choice" = "y" ] || [ "$back_choice" = "Y" ]; then
    exec ./main_menu.sh
else
    echo ""
    echo -e "${CYAN}Terima kasih! 👋${NC}"
    echo ""
fi

