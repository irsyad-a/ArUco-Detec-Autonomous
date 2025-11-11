#!/usr/bin/env python3
"""
ArUco Marker Generator
Generate ArUco markers untuk testing
"""

import cv2
import cv2.aruco as aruco
import os

def generate_aruco_markers(num_markers=20, marker_size=200, output_dir="aruco_markers"):
    """
    Generate ArUco markers
    
    Args:
        num_markers: Jumlah markers yang akan di-generate
        marker_size: Ukuran marker dalam pixels
        output_dir: Folder output
    """
    

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"✓ Directory '{output_dir}' dibuat")
    

    dictionary = aruco.getPredefinedDictionary(aruco.DICT_5X5_50)
    
    print(f"\n{'='*50}")
    print(f"ArUco Marker Generator")
    print(f"{'='*50}")
    print(f"Dictionary: DICT_5X5_50")
    print(f"Marker Size: {marker_size}x{marker_size} pixels")
    print(f"Jumlah: {num_markers} markers")
    print(f"Output: {output_dir}/")
    print(f"{'='*50}\n")
    
    # Generate markers
    for marker_id in range(num_markers):
        # Generate marker image
        marker_image = dictionary.generateImageMarker(marker_id, marker_size)
        
        # Tambahkan border putih dan label
        import numpy as np
        
        # Ukuran dengan border dan label
        border_size = 40
        label_height = 60
        total_height = marker_size + 2*border_size + label_height
        total_width = marker_size + 2*border_size
        
        # Buat canvas putih
        canvas = np.ones((total_height, total_width), dtype=np.uint8) * 255
        
        # Paste marker di tengah
        canvas[border_size:border_size+marker_size, 
               border_size:border_size+marker_size] = marker_image
        
        # Tambahkan text label
        text = f"ArUco ID: {marker_id}"
        font = cv2.FONT_HERSHEY_SIMPLEX
        font_scale = 1.0
        thickness = 2
        
        # Hitung posisi text agar centered
        text_size = cv2.getTextSize(text, font, font_scale, thickness)[0]
        text_x = (total_width - text_size[0]) // 2
        text_y = marker_size + border_size + 45
        
        cv2.putText(canvas, text, (text_x, text_y), font, font_scale, (0, 0, 0), thickness)
        
        # Simpan file
        filename = os.path.join(output_dir, f"aruco_marker_{marker_id:02d}.png")
        cv2.imwrite(filename, canvas)
        print(f"✓ Generated: {filename}")
    
    print(f"\n{'='*50}")
    print(f"✓ Selesai! {num_markers} markers berhasil di-generate")
    print(f"{'='*50}")
    print(f"\nCara menggunakan:")
    print(f"1. Buka folder '{output_dir}/'")
    print(f"2. Print markers yang Anda butuhkan")
    print(f"3. Gunakan untuk testing dengan ./aruco_detection")
    print(f"\nTips:")
    print(f"- Print dengan ukuran minimal 5x5 cm")
    print(f"- Gunakan kertas putih bersih")
    print(f"- Tinta hitam pekat untuk hasil terbaik")
    print(f"{'='*50}\n")

if __name__ == "__main__":
    # Generate 20 markers pertama (ID 0-19)
    generate_aruco_markers(
        num_markers=20,
        marker_size=200,
        output_dir="aruco_markers"
    )
