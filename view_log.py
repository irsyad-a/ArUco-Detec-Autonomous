#!/usr/bin/env python3
"""
ArUco Detection Log Viewer
Tool untuk membaca dan menganalisis log deteksi ArUco
"""

import sys
import os
from datetime import datetime
from collections import defaultdict

def parse_log_file(filename):
    """Parse log file dan ekstrak informasi deteksi"""
    
    if not os.path.exists(filename):
        print(f"❌ File '{filename}' tidak ditemukan!")
        return None
    
    detections = []
    current_detection = {}
    
    with open(filename, 'r') as f:
        lines = f.readlines()
        
        for line in lines:
            line = line.strip()
            
            if line.startswith("Timestamp"):
                current_detection['timestamp'] = line.split(":", 1)[1].strip()
            elif line.startswith("Marker ID"):
                current_detection['marker_id'] = int(line.split(":")[1].strip())
            elif line.startswith("Center Position"):
                pos = line.split(":")[1].strip()
                current_detection['position'] = pos
            elif line.startswith("Size (pixels)"):
                size = line.split(":")[1].strip().replace(" px", "")
                current_detection['size'] = int(size)
            elif line.startswith("Status"):
                current_detection['status'] = line.split(":")[1].strip()
                
                # Simpan deteksi yang sudah lengkap
                if current_detection:
                    detections.append(current_detection.copy())
                    current_detection = {}
    
    return detections

def analyze_detections(detections):
    """Analisis data deteksi"""
    
    if not detections:
        print("Tidak ada data deteksi!")
        return
    
    # Statistik per marker ID
    marker_stats = defaultdict(list)
    
    for det in detections:
        marker_id = det['marker_id']
        marker_stats[marker_id].append(det)
    
    return marker_stats

def display_summary(detections, marker_stats):
    """Tampilkan ringkasan deteksi"""
    
    print("\n" + "="*70)
    print(" "*20 + "ARUCO DETECTION LOG SUMMARY")
    print("="*70)
    print(f"\n📊 Total Deteksi: {len(detections)}")
    print(f"🎯 Total Marker Unik: {len(marker_stats)}")
    
    if detections:
        print(f"⏰ Deteksi Pertama: {detections[0]['timestamp']}")
        print(f"⏰ Deteksi Terakhir: {detections[-1]['timestamp']}")
    
    print("\n" + "-"*70)
    print("DETAIL PER MARKER ID")
    print("-"*70)
    
    for marker_id in sorted(marker_stats.keys()):
        dets = marker_stats[marker_id]
        print(f"\n🏷️  Marker ID: {marker_id}")
        print(f"   Jumlah deteksi: {len(dets)}")
        print(f"   Pertama kali: {dets[0]['timestamp']}")
        print(f"   Terakhir kali: {dets[-1]['timestamp']}")
        
        # Hitung rata-rata size
        avg_size = sum(d['size'] for d in dets) / len(dets)
        print(f"   Rata-rata ukuran: {avg_size:.1f} px")
    
    print("\n" + "="*70)

def display_detailed_log(detections, limit=20):
    """Tampilkan log detail"""
    
    print("\n" + "="*70)
    print(" "*25 + "DETAILED LOG")
    print("="*70)
    
    total = len(detections)
    display_count = min(limit, total)
    
    print(f"\nMenampilkan {display_count} dari {total} deteksi terakhir:\n")
    
    # Ambil deteksi terakhir
    recent_detections = detections[-display_count:]
    
    for i, det in enumerate(recent_detections, 1):
        print(f"#{i}")
        print(f"  Timestamp   : {det['timestamp']}")
        print(f"  Marker ID   : {det['marker_id']}")
        print(f"  Position    : {det['position']}")
        print(f"  Size        : {det['size']} px")
        print(f"  Status      : {det['status']}")
        print()
    
    if total > limit:
        print(f"... dan {total - limit} deteksi lainnya")
    
    print("="*70)

def search_by_marker_id(detections, marker_id):
    """Cari deteksi berdasarkan marker ID"""
    
    filtered = [d for d in detections if d['marker_id'] == marker_id]
    
    if not filtered:
        print(f"\n❌ Tidak ada deteksi untuk Marker ID {marker_id}")
        return
    
    print("\n" + "="*70)
    print(f"DETEKSI UNTUK MARKER ID {marker_id}")
    print("="*70)
    print(f"\nTotal deteksi: {len(filtered)}\n")
    
    for i, det in enumerate(filtered, 1):
        print(f"#{i} - {det['timestamp']}")
        print(f"     Position: {det['position']} | Size: {det['size']} px")
        print()
    
    print("="*70)

def main():
    log_file = "aruco_detection_log.txt"
    
    # Cek argumen
    if len(sys.argv) > 1:
        if sys.argv[1] == "--help" or sys.argv[1] == "-h":
            print("ArUco Detection Log Viewer")
            print("\nUsage:")
            print("  python3 view_log.py              # Tampilkan summary")
            print("  python3 view_log.py --detail     # Tampilkan log detail")
            print("  python3 view_log.py --id <ID>    # Cari berdasarkan marker ID")
            print("  python3 view_log.py --all        # Tampilkan semua log detail")
            return
    
    # Parse log file
    print(f"📂 Membaca file: {log_file}")
    detections = parse_log_file(log_file)
    
    if detections is None:
        return
    
    if not detections:
        print("\n⚠️  File log kosong atau belum ada deteksi")
        print("   Jalankan program aruco_detection dan deteksi beberapa marker dulu!")
        return
    
    # Analisis
    marker_stats = analyze_detections(detections)
    
    # Tampilkan berdasarkan argumen
    if len(sys.argv) > 1:
        if sys.argv[1] == "--detail":
            display_detailed_log(detections, limit=20)
        elif sys.argv[1] == "--all":
            display_detailed_log(detections, limit=len(detections))
        elif sys.argv[1] == "--id" and len(sys.argv) > 2:
            try:
                marker_id = int(sys.argv[2])
                search_by_marker_id(detections, marker_id)
            except ValueError:
                print("❌ Marker ID harus berupa angka!")
        else:
            print("❌ Argumen tidak valid. Gunakan --help untuk bantuan")
    else:
        # Default: tampilkan summary
        display_summary(detections, marker_stats)

if __name__ == "__main__":
    main()
