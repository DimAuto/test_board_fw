/**
 * @file i2c.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-07-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SRC_CORE_I2C_H_
#define SRC_CORE_I2C_H_

#include <msp430.h>
#include <stdint.h>
#include <src/config.h>


#define MAX_BUFFER_SIZE         20
#define SLAVE_ADDR_DEFAULT      PCA9534_L
#define SLAVE_MOCK_INPUT_DATA   0
#define SLAVE_MOCK_OUTPUT_DATA  0

typedef enum I2C_ModeEnum{
    IDLE_MODE,
    NACK_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    SWITCH_TO_RX_MODE,
    SWITCH_TO_TX_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    TIMEOUT_MODE,
    SLAVE_ADDR_RX
} I2C_Mode;

typedef struct {
    uint8_t pca_reg_addr;
    uint8_t pca_l_data;
    uint8_t pca_r_data;
    uint8_t pca_addr;
    uint8_t pca_l_config_data;
    uint8_t pca_r_config_data;
} Slave_data_t;

typedef struct {
    uint8_t pca_l_mock_data;
    uint8_t pca_r_mock_data;
} Arms_mock_tx_data;
/**
 * Init i2c as slave
 * @param slave_addr
 * @return
 */
int i2cInit_Slave(void);

/**
 * Init i2c as Master
 * @return
 */
void i2cInit_Master(void);

/**
 * Enables the i2c slave by reseting the software reset and
 * enabling the STT interrupt.
 */
static inline void i2cEnable_Slave(void);
/**
 * De-inits the slave.
 */
static inline void i2cDeinit_Slave(void);

/**
 * Changes the slave address.
 * @param slave_addr
 */
static void I2C_Slave_ChangeAddr(uint8_t slave_addr);
/**
 *
 * @param dev_addr
 * @param reg_addr
 * @param reg_data
 * @param count
 * @return
 */
I2C_Mode I2C_Master_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count);

/**
 *
 * @param dev_addr
 * @param reg_addr
 * @param count
 * @return
 */
I2C_Mode I2C_Master_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count);

/**
 *
 * @param slave_addr
 * @param slave_reg
 * @return
 */
uint8_t I2C_Slave_Listen(uint8_t slave_addr, uint8_t slave_reg);

static void I2C_Slave_tick_reception(void);

uint8_t I2C_Slave_ReadData(void);

uint8_t I2C_Slave_ReadReg(void);

uint8_t I2C_Slave_set_mock_data(uint8_t pca_l, uint8_t pca_r);

static uint8_t I2C_Slave_tick_transmition(void);

#endif /* SRC_CORE_I2C_H_ */
