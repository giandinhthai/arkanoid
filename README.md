# Arkanoid Game

## Giới thiệu
Arkanoid là một tựa game cổ điển với thiết kế đơn giản, lần đầu tiên được phát hành vào năm 1986 do Taito phát triển. Trong phần hiện thực này, Arkanoid được thiết kế đơn giản bao gồm các khối gạch, một quả bóng và một thanh veus.

## Cài đặt
### Yêu cầu
- Hệ điều hành: Windows
- GCC: 7.3.0 MinGW hoặc cao hơn
- Visual Studio Code
- MinGW Makefile

### Hướng dẫn cài đặt
Trò chơi được xây dựng bằng C++ với OOP kết hợp thư viện đồ họa SFML. Thực hiện các lệnh sau để cài đặt:

1. Mở folder Arkanoid trong Visual Studio Code.
2. Chỉnh sửa compiler mặc định thành mingw g++.
3. Mở terminal của Visual Studio Code và chạy các dòng sau:
    ```sh
    make all     # Biên dịch chương trình
    ./arkanoid.exe  # Chạy chương trình
    ```

## Cách chơi
Trò chơi bao gồm các khối gạch, một quả bóng và một thanh veus. Quả bóng sẽ bị bật lại khi va vào các khối gạch, thanh veus hoặc các cạnh của cửa sổ trò chơi (trừ cạnh dưới). 

Nhiệm vụ của bạn là phá hủy tất cả khối gạch (khối gạch bị phá hủy khi va phải quả bóng) bằng cách di chuyển thanh veus đỡ không cho quả bóng bị rơi xuống. Thanh veus di chuyển tương ứng với phím trái và phải trên bàn phím. 

Trò chơi kết thúc khi tất cả các khối gạch bị phá hủy (chiến thắng) hoặc quả bóng bị rơi xuống cạnh dưới cửa sổ trò chơi (thất bại).

## Class Diagram
![Screenshot from 2024-07-25 19-58-53](https://github.com/user-attachments/assets/a9684d14-4b3b-4b16-8f5b-be4848d47900)
