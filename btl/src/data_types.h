// data_types.h
#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// --- FSP Includes ---
// Quay về cách include đơn giản nhất, vì đường dẫn đã được cấu hình
#include "fsp_common_api.h"
#include "hal_data.h"
// --------------------------------------------------------

// --- Cấu hình Mạng & Buffer ---
#define MAX_PAYLOAD_SIZE 256
#define MAX_TIMESTAMP_SIZE 30
#define MQTT_TOPIC "ckra6m5/sensor_data"
#define SENSOR_READ_INTERVAL_MS 15000 // 15 giây

// --- Cấu trúc Dữ liệu Cảm biến ---
typedef struct {
    float temperature_C; // Nhiệt độ (°C)
    float pressure_Pa;   // Áp suất (Pa)
    float altitude_m;    // Độ cao (m)
    float tvoc_ppb;      // TVOC (ppb) - ZMOD4410
    int32_t iaq_index;   // IAQ (Index) - ZMOD4410
} SensorData_t;

// --- Cấu trúc Gói Dữ liệu (Để gửi và lưu buffer) ---
typedef struct {
    char timestamp_utc[MAX_TIMESTAMP_SIZE];
    char json_payload[MAX_PAYLOAD_SIZE];
} DataPacket_t;

// --- Khai báo hàm FSP và Driver cần tích hợp ---
void hal_time_get_utc(char* time_buffer, size_t buffer_size);
fsp_err_t driver_read_sensors(SensorData_t* data);
fsp_err_t wifi_mqtt_connect(void);

// --- Khai báo hàm MQTT/Network ---
bool mqtt_publish_packet(const char* topic, const char* json_data);
void connection_monitoring_task(void *pvParameters);

// --- Khai báo hàm Buffer (được định nghĩa trong data_buffer.h) ---
void buffer_init(void);
bool buffer_add_packet(const DataPacket_t* packet);
bool buffer_read_oldest_packet(DataPacket_t* packet);
bool buffer_delete_oldest_packet(void);
bool buffer_is_empty(void);

#endif /* DATA_TYPES_H */
