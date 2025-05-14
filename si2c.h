#ifndef __SOFTI2C_H
#define __SOFTI2C_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 使用前需要将下面的宏函数补充完整
#define si2c_write_pin(pin, level)
#define si2c_read_pin(pin)
#define si2c_delay(us)

typedef struct {
    uint32_t delay_us;
    uint32_t scl;
    uint32_t sda;
} si2c_t;

typedef enum {
    si2c_dir_transmitter,
    si2c_dir_receiver,
} si2c_dir_t;

// 上层封装函数
void si2c_init(si2c_t *i2c, uint32_t scl, uint32_t sda, uint32_t delay_us);
uint32_t si2c_write_cmd(si2c_t *i2c, uint8_t addr, uint8_t reg, uint8_t cmd);
uint32_t si2c_write_data(si2c_t *i2c, uint8_t addr, uint8_t cmd, uint8_t *data,
                         uint32_t size);
uint32_t si2c_read_data(si2c_t *i2c, uint8_t addr, uint8_t cmd, uint8_t *data,
                        uint32_t size);

// 底层时序函数
void si2c_start(si2c_t *i2c);
void si2c_stop(si2c_t *i2c);
void si2c_send_ack(si2c_t *i2c, uint32_t ack);
uint32_t si2c_receive_ack(si2c_t *i2c);
uint32_t si2c_send_addr(si2c_t *i2c, uint8_t addr, si2c_dir_t dir);
uint32_t si2c_send_data(si2c_t *i2c, uint8_t *data, uint32_t size);
void si2c_receive_data(si2c_t *i2c, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif