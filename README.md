# Archery Game - Build on AK Embedded Base Kit

<center><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/epcb_archery_game.webp" alt="epcb archery game" width="100%"/></center>

<hr>


<div align="center">
    <video src="https://github.com/ak-embedded-software/archery-game/assets/54855481/d493703c-bf5b-4fd2-ae04-b86784a01231" alt="epcb archery game" height=200/>
</div>


<hr>

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
**Các khái niệm trong event-driven:**

- **Event Driven:** Nôn na là một hệ thống gửi thư (gửi message) để thực thi các công việc. Trong đó, Task đóng vai trò là người nhận thư, Signal đại diện cho nội dung công việc. Task & Signal nền tảng của một hệ Event Driven.
- **Task:** Thông thường mỗi Task sẽ nhận một nhóm công công việc nào nào đó, ví dụ: quản lý state-machine, quản lý hiển thị của màn hình, quản lý việc cập nhật phần mềm, quản lý hệ thống watchdog ... 
- **Message:** Được chia làm 2 loại chính, Message chỉ chứa Signal, hoặc vừa chứa Signal và Data. Message tương đương với Signal.
- **Handler:** Chỗ thực thi một công việc nào đó thì gọi là Handler.

Chi tiết các khái niệm các bạn tham khảo tại bài viết: [AK Embedded Base Kit - STM32L151 - Event Driven: Task & Signal](https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-event-driven-task-signal)

### 2.1 Sơ đồ trình tự
**Sơ đồ trình tự** được sử dụng để mô tả trình tự của các Message và luồng tương tác giữa các đối tượng trong một hệ thống.

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/sequence_object/archery_game_UML.webp" alt="archery game UML" width="720"/></p>
<p align="center"><strong><em>Hình 5:</em></strong> The sequence diagram</p>

### Ghi chú:
**SCREEN_ENTRY:** Cài đặt các thiết lập ban đầu cho đối tượng trong game.
- **Level setup:** Thiết lập thông số cấp độ cho game.
- **AR_GAME_ARCHERY_SETUP:** Thiết lập thông số ban đầu cho đối tượng Archery
- **AR_GAME_ARROW_SETUP:** Thiết lập thông số ban đầu cho các đối tượng Arrow
- **AR_GAME_METEOROID_SETUP:** Thiết lập thông số ban đầu cho các đối tượng Meteoroid
- **AR_GAME_BANG_SETUP:** Thiết lập thông số ban đầu cho các đối tượng Bang
- **AR_GAME_BORDER_SETUP:** Thiết lập thông số ban đầu cho đối tượng Border
- **Setup timer - Time tick:** Khởi tạo Timer - Time tick cho game.
- **STATE (GAME_ON):** Cập nhật trạng thái game -> GAME_ON

**GAME PLAY:** Quá trình hoạt động của game.

**GAME PLAY - Normal:** Game hoạt động ở trạng thái bình thường.
- **AR_GAME_TIME_TICK:** Signal do Timer - Time tick gửi đến.
- **AR_GAME_ARCHERY_UPDATE:** Cập nhật trạng thái Archery.
- **AR_GAME_ARROW_RUN:** Cập nhật di chuyển của các Arrow theo thời gian.
- **AR_GAME_METEOROID_RUN:** Cập nhật di chuyển của các Meteoroid theo thời gian.
- **AR_GAME_METEOROID_DETONATOR:** Kiểm tra các Meteoroid có bị Arrow phá hủy.
- **AR_GAME_BANG_UPDATE:** Cập nhật hoạt ảnh vụ nổ theo thời gian
- **AR_GAME_BORDER_UPDATE:** Kiểm tra số điểm hiện tại để cập nhật tăng độ khó game.
- **AR_GAME_CHECK_GAME_OVER:** Kiểm tra Meteoroid chạm vào Border. Nếu chạm vào thì gửi Signal - **AR_GAME_RESET** đến **Screen**.

**GAME PLAY - Action:** Game hoạt động ở trạng thái có tác động của các nút nhấn.
- **AR_GAME_ARCHERY_UP:** Player nhấn nút **[Up]** điều khiển Archery di chuyển lên.
- **AR_GAME_ARCHERY_DOWN:** Player nhấn nút **[Down]** điều khiển Archery di chuyển xuống.
- **AR_GAME_ARROW_SHOOT:** Player nhấn nút **[Mode]** điều khiển Archery bắn Arrow ra.

**RESET GAME:** Quá trình cài đặt lại các thông số trước khi thoát game.
- **STATE (GAME_OVER):** Cập nhật trạng thái game -> GAME_OVER
- **AR_GAME_RESET:** Signal cài đặt lại game do Border gửi đến.
- **AR_GAME_ARCHERY_RESET:** Cài đặt lại đối tượng Archery trước khi thoát.
- **AR_GAME_ARROW_RESET:** Cài đặt lại đối tượng Arrow trước khi thoát.
- **AR_GAME_METEOROID_RESET:** Cài đặt lại đối tượng Meteoroid trước khi thoát.
- **AR_GAME_BANG_RESET:** Cài đặt lại đối tượng Bang trước khi thoát.
- **AR_GAME_BORDER_RESET:** Cài đặt lại đối tượng Border trước khi thoát.
- **Save and reset Score:** Lưu số điểm hiện tại và Cài đặt lại.
- **Timer remove - Timer tick:** Xóa Timer - Time tick
- **Setup timer - Timer exit:** Tạo 1 timer one shot để thoát game. Nhằm tạo ra một khoảng delay cho người chơi có thể nhận thức được là mình đã game over trước khi chuyển sang màn hình thông báo game over.

**EXIT:** Thoát khỏi game và chuyển sang màn hình Game Over.
- **AR_GAME_EXIT:** Signal do Timer exit gửi đến.
- **STATE (GAME_OFF):** Cập nhật trạng thái game -> GAME_OFF
- **Change the screen - SCREEN_TRAN(scr_game_over_handle, &scr_game_over):** Chuyển màn hình sang màn hình Game Over.

### 2.2 Chi tiết

Sau khi xác định được các đối tượng trong game mà chúng ta cần, tiếp theo chúng ta phải liệt kê ra các thuộc tính, các task, các signal và bitmap mà trong game sẽ sử dụng tới.
Việc liệt kê càng chi tiết thì việc làm game diễn ra càng nhanh và tạo tình rõ ràng minh bạch cho phần tài nguyên giúp phần code game diễn ra suông sẻ hơn.

#### 2.2.1 Thuộc tính đối tượng
Việc liệt kê các thuộc tính của đối tượng trong game có các tác dụng quan trọng sau:
- Giúp xác định rõ thông tin về đối tượng trong game.
- Giúp xác định cấu trúc dữ liệu phù hợp để lưu trữ thông tin của đối tượng.
- Khi bạn xác định trước các thuộc tính cần thiết, bạn giảm thiểu khả năng bỏ sót hoặc nhầm lẫn trong việc xử lý và sử dụng các thuộc tính.

**Trạng thái** của một đối tượng được biểu diễn bởi các **thuộc tính**. Trong trò chơi này các đối tượng có các thuộc tính cụ thể là:
- **visible:** Quy định hiển thị, ẩn/hiện của đối tượng.
- **x, y:** Quy định vị trí của đối tượng trên màn hình.
- **action_image:** Quy định hoạt ảnh tạo animation.

Ví dụ:

    typedef struct {
        bool visible;
        uint32_t x, y;
        uint8_t action_image;
    } ar_game_archery_t;

    extern ar_game_archery_t archery;

**Áp dụng struct cho các đối tượng:**
|struct|Các biến|
|------|--------|
|ar_game_archery_t|archery|
|ar_game_arrow_t|arrow[MAX_NUM_ARROW]|
|ar_game_bang_t|bang[NUM_BANG]|
|ar_game_border_t|border|
|ar_game_meteoroid_t|meteoroid[NUM_METEOROIDS]|

**(*)** Các đối tượng có số lượng nhiều thì sẽ được khai báo dạng mảng.

**Các biến quan trọng:**
- **ar_game_score:** Điểm của trò chơi.
- **ar_game_status:** Trạng thái quả trò chơi.
  - GAME_OFF: Tắt .
  - GAME_ON: Bật.
  - GAME_OVER: Đã thua.

- **ar_game_setting_t** settingsetup : Cấu hình cấp độ của trò chơi.
  - settingsetup.silent : Bật/tắt chế độ im lặng.
  - settingsetup.num_arrow : Cấu hình số lượng mũi tên.
  - settingsetup.arrow_speed : Cấu hình tốc độ mũi tên.
  - settingsetup.meteoroid_speed : Cấu hình tốc độ của thiên thạch.

#### 2.2.2 Task
Trong lập trình event-driven, task là một đơn vị độc lập đảm nhiệm một nhóm công việc nhất định. Khi hệ thống scheduler tìm thấy message liên quan đến task trong hàng đợi, hệ thống sẽ gọi hàm thực thi của task để xử lý message được gửi đến. Một số tác dụng quan trọng của task:
- **Xử lý sự kiện:** Task được sử dụng để xử lý các message được bắn đến khi có sự kiện xảy ra. Mỗi task có thể được liên kết với một sự kiện cụ thể và thực thi một loạt các hành động khi sự kiện đó xảy ra.
- **Đồng bộ hóa:** Task cung cấp cơ chế đồng bộ hóa cho việc xử lý các sự kiện. Khi một sự kiện xảy ra, task tương ứng được kích hoạt và thực thi. Các task khác sẽ đợi cho đến khi task hiện tại hoàn thành trước khi được kích hoạt. Điều này giúp đảm bảo rằng các hành động xử lý sự kiện được thực hiện theo một thứ tự nhất định và tránh xung đột.
- **Quản lý luồng điều khiển:** Task cho phép quản lý luồng sự kiện trong ứng dụng event-driven. Bằng cách sử dụng task, bạn có thể xác định thứ tự thực thi của các hành động khi xảy ra các sự kiện khác nhau.
- **Tách biệt logic:** Sử dụng task giúp tách biệt logic xử lý sự kiện, điều này giúp Source code rõ ràng, dễ đọc.
- **Phân cấp nhiệm vụ:** Task level cho phép sắp xếp trình tự ưu tiên xử lý các message của task ở trong hàng đợi của hệ thống. Trong game các task level của game điều là 4 nên task nào được gọi trước sẽ xử lý trước. 

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/table_task.webp" alt="archery tasks design" width="720"/></p>
<p align="center"><strong><em>Hình 6:</em></strong> Bảng Task của các đối tượng</p>

#### 2.2.3 Message & Signal
**Message** được chia làm 2 loại chính, Message chỉ chứa Signal và Message vừa chứa Signal và Data. **Message** tương đương với **Signal**

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/table_signal.webp" alt="archery signals design" width="720"/></p>
<p align="center"><strong><em>Hình 7:</em></strong> Bảng Signal của từng Task</p>

**(*)** Tác dụng của các Signal trong game: xem tại Ghi chú - Hình 5

## III. Hướng dẫn chi tiết code trong đối tượng
### 3.1 Archery
**Sequence diagram:**

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/sequence_object/archery_sequence.webp" alt="archery sequence" width="640"/></p>
<p align="center"><strong><em>Hình 8:</em></strong> Archery sequence</p>

**Tóm tắt nguyên lý:** Archery sẽ nhận Signal thông được gửi từ 2 nguồn là Screen và Button. Quá trình xử lý của đối tượng phần làm 3 giai đoạn:
- **Giai đoạn 1:** Bắt đầu game, cài đặt các thông số của Archery như vị trí và hình ảnh.
- **Giai đoạn 2:** Chơi game, trong giai đoạn này chia làm 2 hoạt động là:
  - Cập nhật: Screen gửi Signal cập nhật cho Archery mỗi 100ms để cập nhật trạng thái hiện tại của Archery.
  - Hành động: Button gửi Signal di chuyển lên/xuống cho Archery mỗi khi nhấn nút.
- **Giai đoạn 3:** Kết thúc game, thực hiện cài đặt lại trạng thái của Archery trước khi thoát game.

**Code:**

Trong code bạn có thể dùng macro để thay thế hàm void trong nhiều trường hợp.

    #define TEN_DOAN_CODE()
    do { \
        /*code*/ \
    } while(0);

Khai báo: Thư viện, struct và biến.

    #include "ar_game_archery.h"

    ar_game_archery_t archery;
    static uint32_t archery_y = AXIS_Y_ARCHERY;

AR_GAME_ARCHERY_SETUP() là một macro được dùng định nghĩa để cài đặt trạng thái ban đầu của trò chơi bắn cung. Nó đặt các giá trị của biến archery và sử dụng các hằng số được định nghĩa trước đó để thiết lập tọa độ, màu sắc và hình ảnh của cung.

    #define AR_GAME_ARCHERY_SETUP() \
    do { \
        archery.x = AXIS_X_ARCHERY; \
        archery.y = AXIS_Y_ARCHERY; \
        archery.visible = WHITE; \
        archery.action_image = 1; \
    } while (0);

AR_GAME_ARCHERY_UP() là một macro được sử dụng để di chuyển cung lên trên. Nó giảm giá trị của archery_y bằng một giá trị STEP_ARCHERY_AXIS_Y và kiểm tra nếu giá trị mới bằng 0, nó được gán lại là 10.

    #define AR_GAME_ARCHERY_UP() \
    do { \
        archery_y -= STEP_ARCHERY_AXIS_Y; \
        if (archery_y == 0) {archery_y = 10;} \
    } while(0);

AR_GAME_ARCHERY_DOWN() là một macro được sử dụng để di chuyển cung xuống dưới. Nó tăng giá trị của archery_y bằng một giá trị STEP_ARCHERY_AXIS_Y và kiểm tra nếu giá trị mới vượt quá 50, nó được gán lại là 50.

    #define AR_GAME_ARCHERY_DOWN() \
    do { \
        archery_y += STEP_ARCHERY_AXIS_Y; \
        if (archery_y > 50) {archery_y = 50;} \
    } while(0);

AR_GAME_ARCHERY_RESET() là một macro được sử dụng để đặt lại trạng thái ban đầu của trò chơi cung bắn. Nó đặt lại giá trị của archery, archery_y và làm cho cung trở nên không hiển thị.

    #define AR_GAME_ARCHERY_RESET() \
    do { \
        archery.x = AXIS_X_ARCHERY; \
        archery.y = AXIS_Y_ARCHERY; \
        archery.visible = BLACK; \
        archery_y = AXIS_Y_ARCHERY; \
    } while(0);

Hàm ar_game_archery_handle() là một hàm xử lý các thông điệp (messages) liên quan đến trò chơi cung bắn. Nó chứa một câu lệnh switch-case để xử lý các thông điệp khác nhau. Các thông điệp được gửi đến hàm này thông qua một tham số msg có kiểu dữ liệu ak_msg_t. Mỗi case trong switch-case xử lý một thông điệp cụ thể.

    void ar_game_archery_handle(ak_msg_t* msg) {
        switch (msg->sig) {
        case AR_GAME_ARCHERY_SETUP: {
            APP_DBG_SIG("AR_GAME_ARCHERY_SETUP\n");
            AR_GAME_ARCHERY_SETUP();
        }
            break;

        case AR_GAME_ARCHERY_UPDATE: {
            APP_DBG_SIG("AR_GAME_ARCHERY_UPDATE\n");
            archery.y = archery_y;
        }
            break;

        case AR_GAME_ARCHERY_UP: {
            APP_DBG_SIG("AR_GAME_ARCHERY_UP\n");
            AR_GAME_ARCHERY_UP();
        }
            break;

        case AR_GAME_ARCHERY_DOWN: {
            APP_DBG_SIG("AR_GAME_ARCHERY_DOWN\n");
            AR_GAME_ARCHERY_DOWN();
        }
            break;

        case AR_GAME_ARCHERY_RESET: {
            APP_DBG_SIG("AR_GAME_ARCHERY_RESET\n");
            AR_GAME_ARCHERY_RESET();
        }
            break;

        default:
            break;
        }
    }


### 3.2 Arrow

**Sequence diagram:**

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/sequence_object/arrow_sequence.webp" alt="arrow sequence" width="640"/></p>
<p align="center"><strong><em>Hình 9:</em></strong> Arrow sequence</p>

**Tóm tắt nguyên lý:** Arrow sẽ nhận Signal thông được gửi từ 2 nguồn là Screen và Button. Quá trình xử lý của đối tượng phần làm 3 giai đoạn:
- **Giai đoạn 1:** Bắt đầu game, cài đặt các thông số của Arrow. Tất cả Arrow vào trạng thái lặn, không hiển thị trên màn hình.
- **Giai đoạn 2:** Chơi game, trong giai đoạn này chia làm 2 hoạt động là:
  - Cập nhật: Screen gửi Signal di chuyển cho Arrow mỗi 100ms để tăng trạng thái của Arrow tạo sự di chuyển cho Arrow.
  - Hành động: Button gửi Signal bắn tên cho Arrow mỗi khi nhấn nút. Arrow sẽ sẽ kiểm tra số mũi tên và nếu còn thì sẽ cập nhật trạng thái để bắn mũi tên ra tại vị trí hiện tại của Archery
- **Giai đoạn 3:** Kết thúc game, thực hiện cài đặt lại trạng thái của Arrow trước khi thoát game.

**Code:** Tương tự Archery (link tham khảo [Archery_game](https://github.com/ak-embedded-software/archery-game.git))

### 3.3 Bang

**Sequence diagram:**

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/sequence_object/bang_sequence.webp" alt="bang sequence" width="640"/></p>
<p align="center"><strong><em>Hình 10:</em></strong> Bang sequence</p>

**Tóm tắt nguyên lý:** Bang sẽ nhận Signal thông được gửi từ Screen. Quá trình xử lý của đối tượng phân làm 3 giai đoạn:
- **Giai đoạn 1:** Bắt đầu game, cài đặt các thông số của Bang. Cho tất cả các bang về trạng thái lặn, không xuất hiện trên màn hình.
- **Giai đoạn 2:** Chơi game, Vụ nổ chỉ xuất sau khi Meteoroid bị phá hủy. Vụ nổ bao gồm các hoạt ảnh được cập nhật sau mỗi 100ms sau 3 hoạt ảnh thì sẽ tự reset.
- **Giai đoạn 3:** Kết thúc game, thực hiện cài đặt lại trạng thái của Arrow trước khi thoát game.

**Code:** Tương tự Archery (link tham khảo [Archery_game](https://github.com/ak-embedded-software/archery-game.git))

### 3.4 Border

**Sequence diagram:**

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/sequence_object/border_sequence.webp" alt="border sequence" width="640"/></p>
<p align="center"><strong><em>Hình 11:</em></strong> Border sequence</p>

**Tóm tắt nguyên lý:** Border là 1 đối tượng bất động trong game. Có nhiệm vụ update level khi đến mốc điểm quy định và kiểm tra game over.
- **Giai đoạn 1:** Bắt đầu game, cài đặt thông số vị trí và hiển thị của Border.
- **Giai đoạn 2:** Chơi game, thực hiện các nhiệm vụ theo chu kỳ 100ms
  - Kiểm tra số điểm nếu số điểm thêm 200 thì tăng tốc độ của Meteoroid.
  - Kiểm tra vị trí của các Meteoroid nếu Meteoroid chạm vào Border thì gửi tín hiệu Reset đến Screen
- **Giai đoạn 3:** Kết thúc game, thực hiện cài đặt lại trạng thái của Border trước khi thoát game.

**Code:** Tương tự Archery (link tham khảo [Archery_game](https://github.com/ak-embedded-software/archery-game.git))

###  3.5 Meteoroid

**Sequence diagram:**

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/sequence_object/meteoroid_sequence.webp" alt="meteoroid sequence" width="640"/></p>
<p align="center"><strong><em>Hình 12:</em></strong> Meteoroid sequence</p>

**Tóm tắt nguyên lý:** Meteoroid là đối tượng xuất hiện và di chuyển liên tục trong game nhận signal từ Screen. Chia làm 3 giai đoạn:
- **Giai đoạn 1:** Bắt đầu game, cài đặt thông số của Meteoroid. Cấp điểm xuất phát ngẫu nghiên cho Meteoroid, hiển thị lên màn hình.
- **Giai đoạn 2:** Chơi game, thực hiện các nhiệm vụ theo chu kỳ 100ms
  - Cập nhật vị trí và hoạt ảnh di chuyển cho Meteoroid
  - Kiểm tra vị trí của các Arrow nếu Arrow chạm vào Meteoroid thì thực hiện reset Arrow và Meteoroid rồi tạo Bang.
- **Giai đoạn 3:** Kết thúc game, thực hiện cài đặt lại trạng thái của Meteoroid trước khi thoát game.

**Code:** Tương tự Archery (link tham khảo [Archery_game](https://github.com/ak-embedded-software/archery-game.git))

## IV. Hiển thị và âm thanh trong trò chơi bắn cung
### 4.1 Đồ họa

Trong trò chơi, màn hình hiện thị là 1 màn hình **LCD OLed 1.3"** có kích thước là **128px*64px**. Nên các đối tượng được hiển thị trong game phải có kích thước hiển thị phù hợp với màn hình nên cần được thiết kế riêng. 

Đồ họa được thiết kế từng phần theo từng đối tượng bằng phần mềm [Photopea](https://www.photopea.com/)

#### 4.1.1 Thiết kế đồ họa cho các đối tượng

<p align="center"><img src="https://github.com/ak-embedded-software/archery-game/blob/main/resources/images/table_bitmap.webp" alt="archery game bitmap" width="720"/></p>
<p align="center"><strong><em>Hình 13:</em></strong> Bitmap của các đối tượng</p>

**Bitmap** là một cấu trúc dữ liệu được sử dụng để lưu trữ và hiển thị hình ảnh trong game.

**Animation** là ứng dụng việc nối ảnh của của nhiều ảnh liên tiếp tạo thành hoạt ảnh cho đổi tượng muốn miêu tả. Trong game, biến “action_image” trong đối tượng được sử dụng nối các ảnh theo thứ tự tạo thành animation.

**Ghi chú:** Trong thiết kế trên có nhiều ảnh khác nhau cho cùng 1 đối tượng để tạo animation cho đối tượng đó nhằm tăng tính chân thật lúc chơi game.

#### 4.1.2 Code

**Archer display:**
```cpp
void ar_game_archery_display() {
if (archery.visible == WHITE && settingsetup.num_arrow != 0) {
    view_render.drawBitmap( archery.x, \
                            archery.y - 10, \
                            bitmap_archery_I, \
                            SIZE_BITMAP_ARCHERY_X, \
                            SIZE_BITMAP_ARCHERY_Y, \
                            WHITE);
}
else if (archery.visible == WHITE && settingsetup.num_arrow == 0) {
    view_render.drawBitmap( archery.x, \
                            archery.y - 10, \
                            bitmap_archery_II, \
                            SIZE_BITMAP_ARCHERY_X, \
                            SIZE_BITMAP_ARCHERY_Y, \
                            WHITE);
}
}
```

**Arrow display:**
```cpp
void ar_game_arrow_display() {
    for (uint8_t i = 0; i < MAX_NUM_ARROW; i++) {
        if (arrow[i].visible == WHITE) {
            view_render.drawBitmap( arrow[i].x, \
                                    arrow[i].y, \
                                    bitmap_arrow, \
                                    SIZE_BITMAP_ARROW_X, \
                                    SIZE_BITMAP_ARROW_Y, \
                                    WHITE);
        }
    }
}
```

**Meteoroid display:**
```cpp
void ar_game_meteoroid_display() {
    for (uint8_t i = 0; i < NUM_METEOROIDS; i++) {
        if (meteoroid[i].visible == WHITE) {
            if (meteoroid[i].action_image == 1) {
                view_render.drawBitmap( meteoroid[i].x, \
                                        meteoroid[i].y, \
                                        bitmap_meteoroid_I, \
                                        SIZE_BITMAP_METEOROIDS_X, \
                                        SIZE_BITMAP_METEOROIDS_Y, \
                                         WHITE);
            }
            else if (meteoroid[i].action_image == 2) {
                view_render.drawBitmap( meteoroid[i].x, \
                                        meteoroid[i].y, \
                                        bitmap_meteoroid_II, \
                                        SIZE_BITMAP_METEOROIDS_X, \
                                        SIZE_BITMAP_METEOROIDS_Y, \
                                        WHITE);
            }
            else if (meteoroid[i].action_image == 3) {
                view_render.drawBitmap( meteoroid[i].x, \
                                        meteoroid[i].y, \
                                        bitmap_meteoroid_III, \
                                        SIZE_BITMAP_METEOROIDS_X, \
                                        SIZE_BITMAP_METEOROIDS_Y, \
                                        WHITE);
            }
        }
    }
}
```

**Bang display:**
```cpp
void ar_game_bang_display() {
    for (uint8_t i = 0; i < NUM_BANG; i++) {
        if (bang[i].visible == WHITE) {
            if (bang[i].action_image == 1) {
                view_render.drawBitmap( bang[i].x, \
                                        bang[i].y, \
                                        bitmap_bang_I, \
                                        SIZE_BITMAP_BANG_I_X, \
                                        SIZE_BITMAP_BANG_I_Y, \
                                        WHITE);
            }
            else if (bang[i].action_image == 2) {
                view_render.drawBitmap( bang[i].x, \
                                        bang[i].y, \
                                        bitmap_bang_II, \
                                        SIZE_BITMAP_BANG_I_X, \
                                        SIZE_BITMAP_BANG_I_Y, \
                                        WHITE);
            }
            else if (bang[i].action_image == 3) {
                view_render.drawBitmap( bang[i].x + 2, \
                                        bang[i].y - 1, \
                                        bitmap_bang_III, \
                                        SIZE_BITMAP_BANG_II_X, \
                                        SIZE_BITMAP_BANG_II_Y, \
                                        WHITE);
            }
        }
    }
}
```

**Border display:**
```cpp
void ar_game_border_display() {
    if (border.visible == WHITE) {
        view_render.drawFastVLine(  border.x, \
                                    AXIS_Y_BORDER_ON, \
                                    AXIS_Y_BORDER_UNDER, \
                                    WHITE);
        for (uint8_t i = 0; i < NUM_METEOROIDS; i++) {
            view_render.fillCircle( border.x, \
                                    meteoroid[i].y + 5, \
                                    1, \
                                    WHITE);
        }
    }
}
```

**Game frame display:**
```cpp
void ar_game_frame_display() {
    view_render.setTextSize(1);
    view_render.setTextColor(WHITE);
    view_render.setCursor(2,55);
    view_render.print("Arrow:");
    view_render.print(settingsetup.num_arrow);
    view_render.setCursor(60,55);
    view_render.print(" Score:");
    view_render.print(ar_game_score);
    view_render.drawLine(0, LCD_HEIGHT,    LCD_WIDTH, LCD_HEIGHT,    WHITE);
    view_render.drawLine(0, LCD_HEIGHT-10, LCD_WIDTH, LCD_HEIGHT-10, WHITE);
    view_render.drawRect(0, 0, 128, 64, 1);
}
```

**Screen display:**
```cpp
void view_scr_archery_game() {
    if (ar_game_status == GAME_ON) {
        ar_game_frame_display();
        ar_game_archery_display();
        ar_game_arrow_display();
        ar_game_meteoroid_display();
        ar_game_bang_display();
        ar_game_border_display();
    }
    else if (ar_game_status == GAME_OVER) {
        view_render.clear();
        view_render.setTextSize(2);
        view_render.setTextColor(WHITE);
        view_render.setCursor(17, 24);
        view_render.print("YOU LOSE");
    }
}
```

</details>

### 4.2 Âm thanh
Âm thành được thiết kế qua website [Arduino Music](https://www.instructables.com/Arduino-Music-From-Sheet-Music/)

Trong khi chơi, để trò chơi thêm phần xinh động và chân thật thì việc có âm thanh là điều cần thiết. 

Các âm thanh cần thiết kế: nút nhấn, bắn tên, vụ nổ, nhạc game.

**Code:**

```cpp
// Âm thanh Bắt đầu game 
BUZZER_PlayTones(tones_SMB);

// Âm thanh Vụ nổ 
BUZZER_PlayTones(tones_BUM);

// Âm thanh nút nhấn
BUZZER_PlayTones(tones_cc);

// Âm thanh cảnh báo
BUZZER_PlayTones(tones_3beep);

// Merry Christmas
BUZZER_PlayTones(tones_merryChristmas);

/*________________BUZZER______________*/

void BUZZER_Sleep(bool sleep);
/*  sleep = 0 : bật âm thanh 
    sleep = 1 : tắt âm thanh */

static const Tone_TypeDef tones_BUM[] = {
    {3000,3},
    {4500,6},
    {   0,0}
};

static const Tone_TypeDef tones_cc[] = {
    {2000,2}, 
    {   0,0}, 
};

static const Tone_TypeDef tones_startup[] = {
    {2000,3},
    {   0,3},
    {3000,3},
    {   0,3},
    {4000,3},
    {   0,3},
    {1200,4},
    {   0,6},
    {4500,6},
    {   0,0}     // <-- tones end
};

static const Tone_TypeDef tones_3beep[] = {
    {4000, 3},
    {   0,10},
    {1000, 6},
    {   0,10},
    {4000, 3},
    {   0, 0}
};

// "Super Mario bros." =
static const Tone_TypeDef tones_SMB[] = {
    {2637,18}, // E7 x2
    {   0, 9}, // x3
    {2637, 9}, // E7
    {   0, 9}, // x3
    {2093, 9}, // C7
    {2637, 9}, // E7
    {   0, 9}, // x3
    {3136, 9}, // G7
    {   0,27}, // x3
    {1586, 9}, // G6
    {   0,27}, // x3

    {2093, 9}, // C7
    {   0,18}, // x2
    {1586, 9}, // G6
    {   0,18}, // x2
    {1319, 9}, // E6
    {   0,18}, // x2
    {1760, 9}, // A6
    {   0, 9}, // x1
    {1976, 9}, // B6
    {   0, 9}, // x1
    {1865, 9}, // AS6
    {1760, 9}, // A6
    {   0, 9}, // x1

    {1586,12}, // G6
    {2637,12}, // E7
    {3136,12}, // G7
    {3520, 9}, // A7
    {   0, 9}, // x1
    {2794, 9}, // F7
    {3136, 9}, // G7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2093, 9}, // C7
    {2349, 9}, // D7
    {1976, 9}, // B6
    {   0,18}, // x2

    {2093, 9}, // C7
    {   0,18}, // x2
    {1586, 9}, // G6
    {   0,18}, // x2
    {1319, 9}, // E6
    {   0,18}, // x2
    {1760, 9}, // A6
    {   0, 9}, // x1
    {1976, 9}, // B6
    {   0, 9}, // x1
    {1865, 9}, // AS6
    {1760, 9}, // A6
    {   0, 9}, // x1

    {1586,12}, // G6
    {2637,12}, // E7
    {3136,12}, // G7
    {3520, 9}, // A7
    {   0, 9}, // x1
    {2794, 9}, // F7
    {3136, 9}, // G7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2093, 9}, // C7
    {2349, 9}, // D7
    {1976, 9}, // B6

    {   0, 0}
};

// Merry Christmas
static const Tone_TypeDef tones_merryChristmas[] = {
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0,18}, // x2

    {2637, 9}, // E7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0,18}, // x2

    {2637, 9}, // E7
    {   0, 9}, // x1
    {3136, 9}, // G7
    {   0, 9}, // x1
    {2093, 9}, // C7
    {   0, 9}, // x1
    {2349, 9}, // D7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0,24}, // x2

    {2794, 9}, // F7
    {   0, 9}, // x1
    {2794, 9}, // F7
    {   0, 9}, // x1
    {2794, 9}, // F7
    {   0, 9}, // x1
    {2794, 9}, // F7
    {   0, 9}, // x1
    {2794, 9}, // F7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2349, 9}, // D7
    {   0, 9}, // x1
    {2349, 9}, // D7
    {   0, 9}, // x1
    {2637, 9}, // E7
    {   0, 9}, // x1
    {2349, 9}, // D7
    {   0, 9}, // x1
    {3136, 9}, // G7
    {   0, 0}  // <-- tones end
};
```

</details>

**Ghi chú:** Nếu không có thời gian hoặc không có khiếu âm nhạc thì tốt nhất nên dùng các thư viện trên github

## V. Quản lý EEPROM

EEPROM được sử dụng để lưu lại các dữ liệu cần giữ sau khi tắt nguồn, bao gồm bảng điểm và cấu hình trò chơi. Vì EEPROM có thể chứa dữ liệu rác, nên mỗi khối dữ liệu lưu xuống EEPROM sẽ được bọc thêm `Magic number` và `checksum` để đảm bảo tính toàn vẹn.

### 5.1 Cấu trúc quản lý

Mỗi record lưu vào EEPROM có dạng:

```text
+----------------------+----------------------+----------------------+
| Magic number         | Data                 | Checksum             |
| 4 bytes              |                      | 1 byte               |
+----------------------+----------------------+----------------------+
```

Trong đó:
- `Magic number`: Với mỗi ứng dụng có 1 Magic number riêng.
- `Data`: Dữ liệu cần lưu.
- `Checksum`: Tổng các byte từ `Magic number` đến hết phần `data`, dùng để kiểm tra dữ liệu có bị thay đổi hay không.

### 5.2 Tác dụng

Cơ chế sử dụng `Magic number` kết hợp với `Checksum` giúp đảm bảo tính hợp lệ và toàn vẹn của dữ liệu trong EEPROM:
- **Phát hiện dữ liệu bị lỗi hoặc bị thay đổi ngoài ý muốn:** `Checksum` cho phép kiểm tra tính toàn vẹn của dữ liệu, tránh sử dụng dữ liệu đã bị hỏng.
- **Phát hiện dữ liệu phù hợp:** với mỗi ứng dụng nên dùng 1 `Magic number` riêng, việc này giúp tránh đọc nhầm dữ liệu của firmware khác.

**Code:**
```cpp
extern bool ar_game_score_read(ar_game_score_t* data);
extern bool ar_game_score_write(ar_game_score_t* data);

extern bool ar_game_setting_read(ar_game_setting_t* data);
extern bool ar_game_setting_write(ar_game_setting_t* data);
```
