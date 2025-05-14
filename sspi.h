#ifndef __SOFTSPI_H
#define __SOFTSPI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 使用前需要将下面的宏函数补充完整
#define spiex_write_pin(pin, level)
#define spiex_read_pin(pin)

typedef struct {
    uint32_t cs;
    uint32_t miso;
    uint32_t mosi;
    uint32_t sclk;
} spiex_t;

// 上层封装函数
void spiex_init(spiex_t *spi, uint32_t cs, uint32_t sclk, uint32_t miso,
                uint32_t mosi);
void spiex_write_cmd(spiex_t *spi, uint8_t cmd);
void spiex_write_data(spiex_t *spi, uint8_t *data, uint32_t size);
void spiex_read_data(spiex_t *spi, uint8_t *data, uint32_t size);

// 底层时序函数
void spiex_start(spiex_t *spi);
void spiex_stop(spiex_t *spi);
void spiex_swap_data(spiex_t *spi, uint8_t *send_data, uint8_t *recv_data,
                     uint32_t size);
void spiex_send_data(spiex_t *spi, uint8_t *data, uint32_t size);
void spiex_receive_data(spiex_t *spi, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif