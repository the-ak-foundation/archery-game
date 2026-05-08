# Archery Game - Build on AK Embedded Base Kit

| [VN] | [EN](README_en.md) |

## I. Giới thiệu

Archery game là một tựa game chạy trên AK Embedded Base Kit. Được xây dựng nhằm mục đích giúp các bạn có đam mê về lập trình nhúng có thể tìm hiểu và thực hành về lập trình event-driven. Trong quá trình xây dựng nên archery game, các bạn sẽ hiểu thêm về cách thiết kế và ứng dụng UML, Task, Signal, Timer, Message, State-machine,... 

### 1.1 Phần cứng

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/AK_Embedded_Base_Kit_STM32L151.webp" alt="AK Embedded Base Kit - STM32L151" width="480"/></p>
<p align="center"><strong><em>Hình 1:</em></strong> AK Embedded Base Kit - STM32L151</p>

[AK Embedded Base Kit](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu) là một evaluation kit dành cho các bạn học phần mềm nhúng nâng cao.

KIT tích hợp LCD **OLED 1.3", 3 nút nhấn, và 1 loa Buzzer phát nhạc**, với các trang bị này thì đã đủ để học hệ thống event-driven thông qua thực hành thiết kế máy chơi game.

KIT cũng tích hợp **RS485**, **NRF24L01+**, và **Flash** lên đến 32MB, thích hợp cho prototype các ứng dụng thực tế trong hệ thống nhúng hay sử dụng như: truyền thông có dây, không dây wireless, các ứng dụng lưu trữ data logger,...

### 1.2 Mô tả trò chơi và đối tượng
Phần mô tả sau đây về **“Archery game”** , giải thích cách chơi và cơ chế xử lý của trò chơi. Tài liệu này dùng để tham khảo thiết kế và phát triển trò chơi về sau.

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/menu_game.webp" alt="menu game" width="480"/></p>
<p align="center"><strong><em>Hình 2:</em></strong> Menu game</p>

Trò chơi bắt đầu bằng màn hình **Menu game** với các lựa chọn sau: 
- **Archery Game:** Chọn vào để bắt đầu chơi game.
- **Setting:** Chọn vào để cài đặt các thông số của game.
- **Charts:** Chọn vào để xem top 3 điểm cao nhất đạt được.
- **Exit:** Thoát menu vào màn hình chờ.

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/objects_in_the_game.webp" alt="archery game play screen" width="600"/></p>
<p align="center"><strong><em>Hình 3:</em></strong> Màn hình game play và các đối tượng</p>

#### 1.2.1 Các đối tượng (Object) trong game:
|Đối tượng|Tên đối tượng|Mô tả|
|---|---|---|
|**Cung tên**|Archery|Di chuyển lên/xuống để chọn vị trí bắn ra mũi tên|
|**Mũi tên**|Arrow|Bắn ra từ cung tên, dùng để phá hủy thiên thạch|
|**Vụ nổ**|Bang|Hiệu ứng xuất hiện khi thiên thạch bị phá hủy|
|**Ranh giới**|Border|Vùng an toàn phải bảo vệ không cho thiên thạch rơi vào|
|**Thiên thạch**|Meteoroid|Vật thể bay về phía cung tên với tốc độ tăng dần, có khả năng phá hủy ranh giới|

**(*)** Trong phần còn lại của tài liệu sẽ dùng tên của các đối tượng để đề cập đến đối tượng.

#### 1.2.2 Cách chơi game: 
- Trong trò chơi này bạn sẽ điều khiển Archery, di chuyển **lên/xuống** bằng hai nút **[Up]/[Down]**, để chọn vị trí **bắn ra** Arrow.
- Khi nhấn nút **[Mode]** Arrow sẽ được bắn ra, nhằm phá hủy các Meteoroid đang bay đến.
- Mục tiêu trò chơi là kiếm được càng nhiều điểm càng tốt, trò chơi sẽ kết thúc khi có Meteoroid chạm vào Border.

#### 1.2.3 Cơ chế hoạt động:
- **Cách tính điểm:** Điểm được tính bằng số lượng Meteoroid bị phá hủy. Mỗi Meteoroid bị phá hủy tương ứng với 10 điểm. Số điểm tích lũy được sẽ hiển thị ở góc dưới bên phải màn hình.
- **Độ khó:** Mỗi khi tích lũy được 200 điểm, tốc độ bay của Meteoroid sẽ tăng lên một cấp độ. Độ khó ban đầu có thể cài đặt trong phần **Setting**.
- **Giới hạn của Arrow:** Khi bắn thì số lượng Arrow hiện có sẽ giảm đi tương ứng số lượng Arrow đang bay, nếu Arrow hiện có giảm về "0" thì không thể bắn được và sẽ có âm thanh báo. Số lượng Arrow hiện có sẽ được hồi lại khi phá hủy được Meteoroid hoặc Arrow bay hết màn hình game. Số lượng Arrow được hiển thị ở góc dưới bên trái màn hình và có thể thay đổi trong phần **Setting**.

- **Animation:** Để trò chơi thêm phần sinh động thì các đối tượng sẽ có thêm hoạt ảnh lúc di chuyển.
- **Kết thúc trò chơi:** Khi Meteoroid chạm vào Border, trò chơi sẽ kết thúc. Các đối tượng sẽ được reset và số điểm sẽ được lưu. Bạn sẽ vào màn hình “Game Over” với 3 lựa chọn là:
  - **Restart:** chơi lại.
  - **Charts:** vào xem bảng xếp hạng.
  - **Home:** về lại menu game.

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/game_over.webp" alt="archery game over screen" width="480"/></p>
<p align="center"><strong><em>Hình 4:</em></strong> Màn hình Game_over</p>

## II. Thiết kế - ARCHERY GAME

### 2.1 Sơ đồ trình tự

### 2.2 Chi tiết

#### 2.2.1 Thuộc tính đối tượng

#### 2.2.2 Task

#### 2.2.3 Message & Signal

## III. Hướng dẫn chi tiết code trong đối tượng

### 3.1 Archery

### 3.2 Arrow

### 3.3 Bang

### 3.4 Border

### 3.5 Meteoroid

## IV. Hiển thị và âm thanh trong trò chơi bắn cung

### 4.1 Đồ họa

#### 4.1.1 Thiết kế đồ họa cho các đối tượng

#### 4.1.2 Code

### 4.2 Âm thanh

## V. Quản lý EEPROM

### 5.1 Cấu trúc quản lý

### 5.2 Tác dụng