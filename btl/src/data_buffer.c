// data_buffer.c

#include "data_buffer.h"
#include "hal_data.h" // Cho Flash FSP
#include <stdio.h>

// --- Cần Tích hợp FLASH FSP API vào các hàm bên dưới ---

void buffer_init(void) {
    // R_FLASH_LP_Open(&g_flash_ctrl, &g_flash_cfg);
    // Cần khởi tạo các con trỏ head/tail bằng cách đọc từ sector quản lý.
    printf("Offline Buffer Initialized (Flash/SD Card).\n");
}

bool buffer_add_packet(const DataPacket_t* packet) {
    // 1. Tính toán địa chỉ ghi kế tiếp (head_ptr)
    // 2. R_FLASH_LP_Write(&g_flash_ctrl, (uint32_t)packet->json_payload, address, packet_size);
    // 3. Cập nhật và lưu lại head_ptr vào sector quản lý.
    printf("Data added to Flash buffer.\n");
    return true; 
}

bool buffer_read_oldest_packet(DataPacket_t* packet) {
    if (buffer_is_empty()) return false;
    // 1. Đọc tail_ptr từ sector quản lý.
    // 2. R_FLASH_LP_Read(&g_flash_ctrl, read_address, (uint32_t)packet->json_payload, packet_size);
    return true;
}

bool buffer_delete_oldest_packet(void) {
    if (buffer_is_empty()) return false;
    // 1. Cập nhật tail_ptr (Con trỏ đọc)
    // 2. Lưu lại tail_ptr vào sector quản lý.
    // Lưu ý: Không cần xóa vật lý cho đến khi cần xoá cả sector.
    printf("Oldest packet marked as deleted.\n");
    return true;
}

bool buffer_is_empty(void) {
    // Logic kiểm tra head_ptr == tail_ptr
    return false; // Giả định
}