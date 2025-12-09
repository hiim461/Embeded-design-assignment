// data_buffer.h
#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include "data_types.h" // Cần thiết để sử dụng DataPacket_t
#include <stdbool.h>

/**
 * @brief Khởi tạo module buffer (Flash FSP).
 */
void buffer_init(void);

/**
 * @brief Thêm gói dữ liệu vào cuối buffer (FIFO).
 * @param packet Con trỏ đến gói dữ liệu cần lưu.
 * @return true nếu thêm thành công, false nếu buffer đầy.
 */
bool buffer_add_packet(const DataPacket_t* packet);

/**
 * @brief Đọc gói dữ liệu cũ nhất từ buffer (không xóa).
 * @param packet Con trỏ để lưu gói dữ liệu đọc được.
 * @return true nếu đọc thành công, false nếu buffer rỗng.
 */
bool buffer_read_oldest_packet(DataPacket_t* packet);

/**
 * @brief Xóa gói dữ liệu cũ nhất khỏi buffer (bằng cách cập nhật tail_ptr sau khi gửi thành công).
 * @return true nếu xóa thành công, false nếu buffer rỗng.
 */
bool buffer_delete_oldest_packet(void);

/**
 * @brief Kiểm tra buffer có rỗng không.
 * @return true nếu rỗng.
 */
bool buffer_is_empty(void);

#endif /* DATA_BUFFER_H */
