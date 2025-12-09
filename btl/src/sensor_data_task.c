// sensor_data_task.c

#include "data_types.h"
#include "mqtt_client.h"
#include "data_buffer.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

// (Giữ nguyên hàm create_data_packet và send_data_from_buffer từ gợi ý trước)

void sensor_data_task(void *pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(15000); // 15 giây

    // Khởi tạo
    driver_init(); // I2C, RTC
    buffer_init(); // Flash/SD Card
    
    // Khởi tạo và kết nối mạng/MQTT ban đầu
    wifi_mqtt_connect(); 

    // Bắt đầu task giám sát kết nối (để tái kết nối tự động)
    xTaskCreate(connection_monitoring_task, "Conn_Mon", 1024, NULL, 1, NULL);

    while (1) {
        printf("\n[CYCLE START]\n");
        
        // 1. Đọc và tạo gói dữ liệu mới
        DataPacket_t new_packet = create_data_packet();
        
        // 2. Kiểm tra và xử lý
        if (is_connected) {
            if (!mqtt_publish_packet(MQTT_TOPIC, new_packet.json_payload)) {
                // Gửi thất bại -> LƯU vào buffer
                buffer_add_packet(&new_packet);
            } else {
                // Gửi thành công -> Gửi tiếp toàn bộ dữ liệu buffer (FIFO)
                send_data_from_buffer();
            }
        } else {
            // Mất kết nối -> LƯU dữ liệu mới vào buffer
            buffer_add_packet(&new_packet);
        }

        vTaskDelay(xDelay); 
    }
}