# Tetris Game - C++ SDL2

Đây là một dự án game Xếp gạch (Tetris) cổ điển được viết bằng ngôn ngữ **C++** và thư viện đồ họa **SDL2**, được triển khai trên Ubuntu 24.04 LTS. Dự án minh họa cách áp dụng lập trình hướng đối tượng (OOP) và quản lý tài nguyên trong phát triển game.

## Cấu trúc thư mục 
* assets/ : chứa hình ảnh, âm thanh và font chữ
* lib/ : chứa các file header .h 
* src/ : chứa toàn bộ file nguồn .cpp 

## Tính năng chính
* Lối chơi Tetris cổ điển (di chuyển, xoay khối gạch, tính điểm).
* Đồ họa sử dụng SDL2_image cho texture và SDL2_ttf cho font chữ.
* Cấu trúc mã nguồn phân tách rõ ràng (Board, Game, Tetromino, Renderer).
* Hệ thống quản lý build hiện đại với CMake.

## Cách điều khiển 
* Phím mũi tên Trái/ Phải: Di chuyển khối gạch sang ngang 
* Phím mũi tên Lên: Xoay khối gạch 
* Phím mũi tên Xuống: Rơi nhanh (Soft drop)
* Phím Space: Rơi tức thì (Hard drop)

## Yêu cầu hệ thống
Để biên dịch và chạy game, bạn cần cài đặt:
* **Trình biên dịch C++** (GCC/G++, Clang hoặc MSVC).
* **CMake** (phiên bản 3.20 trở lên).
* **Thư viện SDL2**:
  * SDL2 core
  * SDL2_image
  * SDL2_ttf

## Hướng dẫn cài đặt và Biên dịch

Thay vì dùng Makefile thủ công, dự án này sử dụng **CMake** để quản lý quá trình build một cách linh hoạt.

### Bước 1: Tạo thư mục build
Mở terminal tại thư mục gốc của dự án và chạy:

mkdir build
cd build

### Bước 2: Cấu hình dự án
Chạy lệnh sau trong terminal: 

cmake ..

### Bước 3: Biên dịch dự án 
Tiếp tục chạy lệnh sau:

cmake --build .

### Bước 4: Chạy chương trình:
Sau bước 3, file thực thi game sẽ xuất hiện trong thư mục build. Chạy lệnh:

./game
