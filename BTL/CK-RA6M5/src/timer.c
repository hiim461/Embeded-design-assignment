/*
 * timer.c
 *
 *  Created on: 6 thg 12, 2025
 *      Author: NGUYEN - PC
 */

#include "timer.h"
 #include "hal_data.h"

/* Biến đếm thời gian hệ thống (tăng lên mỗi 1ms) */
volatile uint32_t g_millis_counter = 0;

/* Hàm trả về thời gian hiện tại (ms) */
uint32_t millis(void)
{
    return g_millis_counter;
}

/* Hàm xử lý ngắt Timer (AGT0) */
void AGT0_Handler(void){
/* Xóa cờ ngắt (nếu cần thiết với thanh ghi cụ thể) */
    R_ICU->IELSR_b[0].IR = 0;
    R_AGT0->AGT16.CTRL.AGTCR_b.TUNDF = 0;

    /* Tăng biến đếm */
    g_millis_counter++;

    /* Code cũ của bạn: Đảo LED */
    R_PORT6->PODR_b.PODR1 ^= 1;
}
