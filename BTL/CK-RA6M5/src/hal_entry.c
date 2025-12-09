#include "hal_data.h"
#include "icp10101_driver.h"
#include "sensors.h"
#include "ra6m5_i2c.h"
#include "uart.h"
#include <stdio.h>
#include "zmod4xxx.h"
#include "zmod4410_driver.h"

void hal_entry(void) {
    uart_init();
    sensors_init();
    printUART("=== START SCAN ===\n");
    SystemData_t ptr = {0};  //Khởi tạo

    char msg[256];
    while(1){
        sensors_read_all(&ptr);
        sprintf(msg,"dev=ck-RA6M5-01 sensor=icp10101 temp=%.2f C press=%.2f Pa alt=%.2f m \ndev=ck-RA6M5-01 sensor=zmod4410 eco2=%.2f ppm etoh=%.2f ppm iaq=%.2f tvoc=%.2f mg/m3\r\n",
                    ptr.temperature,
                    ptr.pressure,
                    ptr.altitude,
                    ptr.eco2,
                    ptr.etoh,
                    ptr.iaq,
                    ptr.tvoc);
        printUART(msg);
        R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);
    }
}
