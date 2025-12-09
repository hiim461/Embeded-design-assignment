// // mqtt_client.c

// #include "mqtt_client.h"
// #include "hal_data.h" // Chứa các instance Wi-Fi/NetX Duo FSP
// #include <stdio.h>
// #include "FreeRTOS.h"
// #include "task.h"

// bool is_connected = false;

// // --- THAY THẾ BẰNG CODE TÍCH HỢP FSP THỰC TẾ ---

// /**
//  * @brief Thực hiện kết nối Wi-Fi và MQTT Broker.
//  */
// fsp_err_t wifi_mqtt_connect(void) {
//     // 1. Tích hợp Wi-Fi FSP (NetX Duo)
//     // err = R_WIFI_Connect(...);
    
//     // 2. Tích hợp MQTT Client (Paho MQTT hoặc tương đương)
//     // err = Paho_MQTT_Client_Connect(...);

//     // ********* TÍCH HỢP CODE KẾT NỐI THỰC TẾ VÀO ĐÂY *********
//     // (Kiểm tra và cập nhật biến is_connected)
//     is_connected = true; 
//     // *******************************************************
    
//     if (is_connected) {
//         printf("Wi-Fi and MQTT connected.\n");
//         return FSP_SUCCESS;
//     }
//     return FSP_ERR_NOT_OPEN;
// }

// /**
//  * @brief Gửi gói dữ liệu JSON bằng MQTT Client.
//  */
// bool mqtt_publish_packet(const char* topic, const char* json_data) {
//     if (!is_connected) {
//         return false;
//     }
    
//     // Tích hợp hàm gửi MQTT thực tế (Paho MQTT Publish)
//     // fsp_err_t err = Paho_MQTT_Publish(topic, json_data, QoS_1);

//     // ********* TÍCH HỢP CODE PUBLISH THỰC TẾ VÀO ĐÂY *********
//     // (Kiểm tra kết quả gửi. Nếu fail và lỗi là do mất kết nối, set is_connected = false)
//     // Giả sử 5% fail:
//     if (rand() % 100 < 5) { 
//         is_connected = false;
//         printf("CONNECTION LOST during publish!\n");
//         return false;
//     }
//     // *******************************************************
    
//     return true; 
// }

// // Task chạy liên tục để kiểm tra và cố gắng tái kết nối
// void connection_monitoring_task(void *pvParameters) {
//     while(1) {
//         if (!is_connected) {
//             printf("Attempting to reconnect...\n");
//             if (wifi_mqtt_connect() == FSP_SUCCESS) {
//                 printf("Reconnected successfully!\n");
//                 is_connected = true; // Cập nhật trạng thái
//             }
//         }
//         vTaskDelay(pdMS_TO_TICKS(5000));
//     }
// }

// mqtt_client.c
#include "data_types.h"
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"

// Biến trạng thái kết nối toàn cục
bool is_connected = false;

// --- Cấu hình Giả lập ---
// Tỉ lệ cố gắng kết nối lại thành công (ví dụ: 80%)
#define RECONNECT_SUCCESS_RATE 80 
// Tỉ lệ mất kết nối ngẫu nhiên khi đang gửi (ví dụ: 5%)
#define PUBLISH_FAILURE_RATE 5 

/**
 * @brief Thực hiện kết nối Wi-Fi và MQTT Broker (Giả lập).
 * @return FSP_SUCCESS nếu kết nối thành công.
 */
fsp_err_t wifi_mqtt_connect(void) {
    printf("Attempting to connect Wi-Fi and MQTT Broker... (SIMULATED)\n");
    vTaskDelay(pdMS_TO_TICKS(1000)); // Giả lập thời gian kết nối
    
    // Giả lập tỉ lệ kết nối lại thành công
    if ((rand() % 100) < RECONNECT_SUCCESS_RATE) {
        is_connected = true; 
        printf("Wi-Fi and MQTT connected successfully (SIMULATED).\n");
        return FSP_SUCCESS;
    } else {
        is_connected = false; 
        printf("ERROR: Connection attempt failed (SIMULATED).\n");
        return FSP_ERR_NOT_OPEN;
    }
}

/**
 * @brief Gửi gói dữ liệu JSON bằng MQTT Client (Giả lập).
 * @param topic Topic MQTT.
 * @param json_data Chuỗi dữ liệu JSON.
 * @return true nếu gửi thành công, false nếu thất bại hoặc mất kết nối.
 */
bool mqtt_publish_packet(const char* topic, const char* json_data) {
    if (!is_connected) {
        printf("ERROR: Cannot publish, MQTT is disconnected.\n");
        return false;
    }
    
    // In payload ra console để kiểm tra
    printf("PUBLISHING (SIMULATED) to %s: %s\n", topic, json_data);
    
    // Giả lập tỉ lệ mất kết nối ngẫu nhiên khi đang gửi
    if ((rand() % 100) < PUBLISH_FAILURE_RATE) { 
        is_connected = false;
        printf("CONNECTION LOST during publish! (SIMULATED)\n");
        return false;
    }
    
    return true; 
}

/**
 * @brief Task chạy liên tục để kiểm tra và cố gắng tái kết nối.
 */
void connection_monitoring_task(void *pvParameters) {
    while(1) {
        if (!is_connected) {
            printf("CONN_MON Task: Attempting to reconnect...\n");
            // Gọi hàm kết nối giả lập
            if (wifi_mqtt_connect() == FSP_SUCCESS) {
                printf("CONN_MON Task: Reconnected successfully!\n");
                // is_connected đã được cập nhật trong wifi_mqtt_connect
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Kiểm tra sau mỗi 5 giây
    }
}