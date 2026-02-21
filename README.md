# Pháo Hoa Tết - Câu Đối Nhấp Nháy 🎆🧧

![SFML](https://img.shields.io/badge/SFML-2.5-green?style=flat-square)
![C++](https://img.shields.io/badge/C++-17-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=flat-square)

Chương trình mô phỏng pháo hoa rực rỡ và hiệu ứng câu đối Tết rơi nhấp nháy, được viết bằng C++ và thư viện đồ họa SFML. Đây là món quà nho nhỏ dành tặng không khí Tết cổ truyền, với các hiệu ứng sinh động và hỗ trợ hiển thị tiếng Việt.

## ✨ Tính năng nổi bật

*   **Hiệu ứng pháo hoa đa sắc:** Hàng trăm hạt (particle) được bắn lên từ vị trí chuột, tạo thành những chùm pháo hoa với màu sắc lễ hội (vàng, đỏ, cam...).
*   **Câu đối Tết rơi nhấp nháy:**
    *   Hai câu đối Tết truyền thống với font chữ tiếng Việt được hiển thị.
    *   Các chữ trong câu đối từ từ rơi từ trên cao xuống đúng vị trí, có độ trễ và hiệu ứng nảy nhẹ.
    *   Sau khi dừng lại, các chữ sẽ nhấp nháy với hiệu ứng phát sáng và thay đổi kích thước.
*   **Nền Tết sống động:**
    *   Tự động tạo gradient nền nếu không tìm thấy ảnh.
    *   Hoa mai (hoặc hoa anh đào) rơi nhẹ nhàng khắp màn hình.
    *   Hỗ trợ ảnh nền `phongnentet.jpg` và khung giấy đỏ `red_paper.png` cho câu đối.
*   **Tương tác chuột:** Nhấp chuột trái vào bất kỳ vị trí nào trên màn hình để tạo ra một màn pháo hoa mới.
*   **Hỗ trợ tiếng Việt:** Cố gắng tải nhiều loại font phổ biến trên Windows, Linux và macOS để hiển thị tiếng Việt có dấu một cách tốt nhất.

## 🚀 Cách cài đặt và chạy

### Yêu cầu hệ thống
*   Trình biên dịch hỗ trợ C++17 (g++, clang, MSVC).
*   Thư viện [SFML](https://www.sfml-dev.org/) phiên bản 2.5 trở lên (bao gồm tất cả các module: Graphics, Window, System).
*   ## 🚀 Cách cài đặt và chạy

### **Yêu cầu hệ thống**
- SFML 2.5 hoặc cao hơn
- Compiler C++ hỗ trợ C++17
- Hệ điều hành: Windows, Linux, hoặc macOS

### **Cài đặt trên Windows (Code::Blocks)**
1. Tải và cài đặt [Code::Blocks](http://www.codeblocks.org/)
2. Tải [SFML](https://www.sfml-dev.org/download.php) cho MinGW
3. Tạo project mới trong Code::Blocks
4. Thêm file `main.cpp` vào project
5. Cấu hình linker với các thư viện SFML:
   - `sfml-graphics`
   - `sfml-window`
   - `sfml-system`
6. Build và chạy chương trình


### Hướng dẫn build

1.  **Clone repository:**
    ```bash
    git clone https://github.com/roboeinlife-glitch/Phao_Hoa_Chu_NhapNhay.git
    cd Phao_Hoa_Chu_NhapNhay
    ```

2.  **Cài đặt SFML:**
    *   **Trên Ubuntu/Debian:**
        ```bash
        sudo apt-get install libsfml-dev
        ```
    *   **Trên Windows/macOS:** Tải SFML từ [trang chủ](https://www.sfml-dev.org/download.php) và cấu hình đường dẫn thư viện cho trình biên dịch của bạn.

3.  **Biên dịch:**
    Sử dụng lệnh sau (ví dụ với g++ trên Linux/macOS):
    ```bash
    g++ -std=c++17 main.cpp -o PhaoHoaTet -lsfml-graphics -lsfml-window -lsfml-system
    ```
    Trên Windows (với MinGW), bạn có thể cần thêm đường dẫn cụ thể:
    ```bash
    g++ -std=c++17 main.cpp -o PhaoHoaTet.exe -I<SFML-include-path> -L<SFML-lib-path> -lsfml-graphics -lsfml-window -sfml-system
    ```

4.  **Chạy chương trình:**
    ```bash
    ./PhaoHoaTet
    ```
    (Hoặc `PhaoHoaTet.exe` trên Windows)

## 🎮 Cách sử dụng

*   **Khởi động:** Chương trình sẽ tự động chạy, bạn sẽ thấy các chữ trong câu đối từ từ rơi xuống và hiệu ứng hoa mai rơi nền.
*   **Tạo pháo hoa:** Di chuyển chuột và **nhấp chuột trái** tại vị trí bạn muốn bắn pháo hoa. Mỗi lần nhấp là một chùm pháo hoa mới được tạo ra.
*   **Thoát:** Nhấn nút **Đóng (X)** trên cửa sổ chương trình.

## 📁 Cấu trúc file và tài nguyên

Để chương trình hoạt động đẹp mắt nhất, bạn có thể đặt các file tài nguyên sau cùng thư mục với file thực thi (hoặc trong đường dẫn tìm kiếm của chương trình):

*   `main.cpp`: Mã nguồn chính của chương trình.
*   `phongnentet.jpg`: (Tùy chọn) Ảnh nền Tết. Nếu không có, chương trình sẽ tự tạo nền gradient.
*   `red_paper.png`: (Tùy chọn) Ảnh khung giấy đỏ cho câu đối.
*   `cherry_blossom.png`: (Tùy chọn) Ảnh hoa mai (hoặc hoa anh đào) để tạo hiệu ứng rơi.
*   **Font chữ:** Chương trình sẽ tự động tìm và sử dụng các font hệ thống hỗ trợ tiếng Việt (như Arial, Times New Roman, DejaVu Sans...). Bạn cũng có thể đặt các font `.ttf` vào cùng thư mục.

## 🛠️ Cấu hình và tùy chỉnh

Bạn có thể dễ dàng thay đổi các thông số trong file `main.cpp` để điều chỉnh hiệu ứng:

*   `SCREEN_WIDTH`, `SCREEN_HEIGHT`: Kích thước cửa sổ.
*   `GRAVITY`: Lực hút tác động lên các hạt pháo hoa và chữ cái.
*   `BOUNCE_DAMPING`: Hệ số nảy của chữ khi chạm đích.
*   `festiveColors`: Mảng màu sắc cho pháo hoa và chữ.
*   Nội dung câu đối: Bạn có thể sửa các dòng chữ trong mảng `leftTexts` và `rightTexts` ở hàm `initCouplets()`.

## 📸 Hình ảnh minh họa

(Chèn ảnh chụp màn hình chương trình tại đây nếu bạn muốn)

## 🤝 Đóng góp

Mọi ý kiến đóng góp, báo lỗi hoặc đề xuất tính năng mới đều được chào đón. Bạn có thể tạo **Issue** hoặc **Pull Request** trên GitHub.

## 📜 Giấy phép

Dự án này được phân phối dưới giấy phép MIT. Xem file `LICENSE` để biết thêm chi tiết.

---

**Chúc Mừng Năm Mới An Khang - Thịnh Vượng!** 🎉

[Trở về trang chủ](https://github.com/roboeinlife-glitch)
