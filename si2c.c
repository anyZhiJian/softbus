#include "si2c.h"

#define SI2C_ACK 0
#define SI2C_NACK 1

void si2c_init(si2c_t *i2c, uint32_t scl, uint32_t sda, uint32_t delay_us)
{
    si2c_write_pin(i2c->scl, 1);
    si2c_write_pin(i2c->sda, 1);
    i2c->scl = scl;
    i2c->sda = sda;
    i2c->delay_us = delay_us;
}

void si2c_send_ack(si2c_t *i2c, uint32_t ack)
{
    si2c_write_pin(i2c->sda, ack);
    si2c_write_pin(i2c->scl, 1);
    si2c_delay(i2c->delay_us);
    si2c_write_pin(i2c->scl, 0);
}

uint32_t si2c_receive_ack(si2c_t *i2c)
{
    uint32_t ack;
    si2c_write_pin(i2c->sda, 1);
    si2c_write_pin(i2c->scl, 1);
    si2c_delay(i2c->delay_us);
    ack = si2c_read_pin(i2c->sda);
    si2c_write_pin(i2c->scl, 0);
    return ack;
}

void si2c_start(si2c_t *i2c)
{
    si2c_write_pin(i2c->sda, 1);
    si2c_write_pin(i2c->scl, 1);
    si2c_delay(i2c->delay_us);
    si2c_write_pin(i2c->sda, 0);
    si2c_write_pin(i2c->scl, 0);
}

void si2c_stop(si2c_t *i2c)
{
    si2c_write_pin(i2c->sda, 0);
    si2c_write_pin(i2c->scl, 1);
    si2c_write_pin(i2c->sda, 1);
}

uint32_t si2c_send_addr(si2c_t *i2c, uint8_t addr, si2c_dir_t dir)
{
    uint32_t i;
    if (dir == si2c_dir_receiver)
        addr |= 0x01;
    for (i = 0; i < 8; i++)
    {
        si2c_write_pin(i2c->sda, addr & (0x80 >> i));
        si2c_write_pin(i2c->scl, 1);
        si2c_delay(i2c->delay_us);
        si2c_write_pin(i2c->scl, 0);
    }
    return si2c_receive_ack(i2c);
}

uint32_t si2c_send_data(si2c_t *i2c, uint8_t *data, uint32_t size)
{
    uint32_t ack = 0;
    uint32_t i, j;
    for (j = 0; j < size; j++)
    {
        for (i = 0; i < 8; i++)
        {
            si2c_write_pin(i2c->sda, data[j] & (0x80 >> i));
            si2c_write_pin(i2c->scl, 1);
            si2c_delay(i2c->delay_us);
            si2c_write_pin(i2c->scl, 0);
        }
        ack |= si2c_receive_ack(i2c);
    }
    return ack;
}

void si2c_receive_data(si2c_t *i2c, uint8_t *data, uint32_t size)
{
    uint8_t recv_data;
    uint32_t i, j;
    for (j = 0; j < size; j++)
    {
        recv_data = 0x00;
        si2c_write_pin(i2c->sda, 1);
        for (i = 0; i < 8; i++)
        {
            si2c_write_pin(i2c->scl, 1);
            si2c_delay(i2c->delay_us);
            if (si2c_read_pin(i2c->sda))
            {
                recv_data |= (0x80 >> i);
            }
            si2c_write_pin(i2c->scl, 0);
        }
        if (j + 1 != size)
        {
            si2c_send_ack(i2c, SI2C_ACK);
        }
        data[j] = recv_data;
    }
    si2c_send_ack(i2c, SI2C_NACK);
}

uint32_t si2c_write_cmd(si2c_t *i2c, uint8_t addr, uint8_t reg, uint8_t cmd)
{
    uint32_t ack = SI2C_ACK;
    si2c_start(i2c);
    ack |= si2c_send_addr(i2c, addr, si2c_dir_transmitter);
    ack |= si2c_send_data(i2c, &reg, 1);
    ack |= si2c_send_data(i2c, &cmd, 1);
    si2c_stop(i2c);
    return ack;
}

uint32_t si2c_write_data(si2c_t *i2c, uint8_t addr, uint8_t cmd, uint8_t *data, uint32_t size)
{
    uint32_t ack = SI2C_ACK;
    si2c_start(i2c);
    ack |= si2c_send_addr(i2c, addr, si2c_dir_transmitter);
    ack |= si2c_send_data(i2c, &cmd, 1);
    ack |= si2c_send_data(i2c, data, size);
    si2c_stop(i2c);
    return ack;
}

uint32_t si2c_read_data(si2c_t *i2c, uint8_t addr, uint8_t cmd, uint8_t *data, uint32_t size)
{
    uint32_t ack = SI2C_ACK;
    si2c_start(i2c);
    ack |= si2c_send_addr(i2c, addr, si2c_dir_transmitter);
    ack |= si2c_send_data(i2c, &cmd, 1);
    si2c_start(i2c);
    ack |= si2c_send_addr(i2c, addr, si2c_dir_receiver);
    si2c_receive_data(i2c, data, size);
    si2c_stop(i2c);
    return ack;
}
