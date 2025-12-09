
#ifndef SENSORS_H_
#define SENSORS_H_
/* --- PROTOCOL FRAMING --- */
#define PROTOCOL_START_BYTE  0xAA
#define PROTOCOL_END_BYTE    0x55

/* --- SYSTEM STATES --- */
typedef enum {
    STATE_INIT = 0,
    STATE_CONNECTING,
    STATE_STREAMING,
    STATE_BUFFERING,
    STATE_RETRANSMIT,
    STATE_ERROR
} SystemState_t;

/* --- DATA PACKET (Packed for UART efficiency) --- */
/* We use #pragma pack to ensure the struct is exactly 31 bytes */
#pragma pack(push, 1)
typedef struct {

    /* ICP10101 Data (12 bytes) */
    float    pressure;    // Pa
    float    temperature; // C
    float    altitude;    // m

    /* ZMOD4410 Data (16 bytes) */
    float    iaq;
    float    tvoc;
    float    eco2;
    float    etoh;
} SystemData_t;
#pragma pack(pop)



#include "hal_data.h"
#include "icp10101_driver.h"
#include "ra6m5_i2c.h"
#include <stdio.h>

/* Init hardware (I2C, GPIOs, Config registers) */
void sensors_init(void);

/* Reads both sensors and fills the SystemData_t struct */
void sensors_read_all(SystemData_t *data);


#endif /* SENSORS_H_ */
