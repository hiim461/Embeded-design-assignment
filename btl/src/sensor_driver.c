// sensor_driver.c

#include "data_types.h"
#include "hal_data.h" // Chứa các instance I2C/SPI được cấu hình trong FSP
#include <stdio.h>

// --- THAY THẾ BẰNG CODE TÍCH HỢP FSP THỰC TẾ ---

/**
 * @brief Khởi tạo các cảm biến (I2C/SPI) và module RTC.
 */
void driver_init(void) {
    // 1. I2C/SPI FSP: Mở các instance I2C/SPI đã cấu hình
    // fsp_err_t err = R_SCI_I2C_Open(&g_i2c_master_ctrl, &g_i2c_master_cfg);
    // (Kiểm tra lỗi và xử lý)
    
    // 2. Cảm biến: Khởi tạo các driver độc lập
    // zmod_4410_init(...);
    // icp_10101_init(...);
    
    // 3. RTC FSP: Khởi tạo RTC
    // R_RTC_Open(&g_rtc_ctrl, &g_rtc_cfg);
    
    printf("Hardware Drivers Initialized.\n");
}

/**
 * @brief Đọc dữ liệu từ cả hai cảm biến.
 * @return FSP_SUCCESS nếu đọc thành công.
 */
fsp_err_t driver_read_sensors(SensorData_t* data) {
    fsp_err_t err = FSP_SUCCESS;
    
    // Đọc ICP-10101 (Áp suất, Nhiệt độ, Độ cao)
    // err |= icp_10101_read(g_i2c_master_ctrl, &data->pressure_Pa, &data->temperature_C, &data->altitude_m);
    
    // Đọc ZMOD4410 (TVOC, IAQ)
    // err |= zmod_4410_read(g_i2c_master_ctrl, &data->tvoc_ppb, &data->iaq_index);

    // ********* TÍCH HỢP CODE ĐỌC THỰC TẾ VÀO ĐÂY *********
    // (Giả sử thành công cho mục đích logic)
    data->temperature_C = 25.5; data->pressure_Pa = 101325; data->altitude_m = 10;
    data->iaq_index = 50; data->tvoc_ppb = 1000;
    // *******************************************************
    
    return err; 
}

/**
 * @brief Lấy thời gian UTC từ RTC FSP và định dạng thành chuỗi.
 */
void hal_time_get_utc(char* time_buffer, size_t buffer_size) {
    // 1. Đọc dữ liệu thời gian từ RTC FSP
    // rtc_time_t current_time;
    // R_RTC_CalendarTimeGet(&g_rtc_ctrl, &current_time);
    
    // 2. Định dạng thành chuỗi ISO 8601 (YYYY-MM-DDThh:mm:ssZ)
    // Dùng snprintf hoặc tương đương
    
    // ********* TÍCH HỢP CODE RTC FSP VÀO ĐÂY *********
    snprintf(time_buffer, buffer_size, "2025-11-20T%02d:%02d:%02dZ", 10, 20, 30); 
    // *******************************************************
}