#include "sspi.h"

void spiex_init(spiex_t *spi, uint32_t cs, uint32_t sclk, uint32_t miso, uint32_t mosi)
{
    spiex_write_pin(spi->cs, 1);
    spiex_write_pin(spi->sclk, 0);
    spi->cs = cs;
    spi->miso = miso;
    spi->mosi = mosi;
    spi->sclk = sclk;
}

void spiex_start(spiex_t *spi)
{
    spiex_write_pin(spi->cs, 0);
}

void spiex_stop(spiex_t *spi)
{
    spiex_write_pin(spi->cs, 1);
}

void spiex_swap_data(spiex_t *spi, uint8_t *send_data, uint8_t *recv_data, uint32_t size)
{
    uint32_t i, j;
    for (j = 0; j < size; j++)
    {
        *(recv_data + j) = 0x00;
        for (i = 0; i < 8; i++)
        {
            spiex_write_pin(spi->mosi, *(send_data + j) & (0x80 >> i));
            spiex_write_pin(spi->sclk, 1);
            if (spiex_read_pin(spi->miso))
            {
                *(recv_data + j) |= (0x80 >> i);
            }
            spiex_write_pin(spi->sclk, 0);
        }
    }
}

void spiex_send_data(spiex_t *spi, uint8_t *data, uint32_t size)
{
    uint32_t i, j;
    for (j = 0; j < size; j++)
    {
        for (i = 0; i < 8; i++)
        {
            spiex_write_pin(spi->mosi, *(data + j) & (0x80 >> i));
            spiex_write_pin(spi->sclk, 1);
            spiex_write_pin(spi->sclk, 0);
        }
    }
}

void spiex_receive_data(spiex_t *spi, uint8_t *data, uint32_t size)
{
    uint32_t i, j;
    for (j = 0; j < size; j++)
    {
        *(data + j) = 0x00;
        for (i = 0; i < 8; i++)
        {
            spiex_write_pin(spi->mosi, 0);
            spiex_write_pin(spi->sclk, 1);
            if (spiex_read_pin(spi->miso))
            {
                *(data + j) |= (0x80 >> i);
            }
            spiex_write_pin(spi->sclk, 0);
        }
    }
}

void spiex_write_cmd(spiex_t *spi, uint8_t cmd)
{
    spiex_start(spi);
    spiex_send_data(spi, &cmd, 1);
    spiex_stop(spi);
}

void spiex_write_data(spiex_t *spi, uint8_t *data, uint32_t size)
{
    spiex_start(spi);
    spiex_send_data(spi, data, size);
    spiex_stop(spi);
}

void spiex_read_data(spiex_t *spi, uint8_t *data, uint32_t size)
{
    spiex_start(spi);
    spiex_receive_data(spi, data, size);
    spiex_stop(spi);
}
