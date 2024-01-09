/**
 * @file i2c.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-06-17
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <i2c.h>
#include <timer.h>
#include <src/config.h>
#include "src/test_board_leds.h"

static volatile uint8_t ReceiveBuffer[MAX_BUFFER_SIZE] = {0};
static volatile uint8_t RXByteCtr = 0;
static volatile uint8_t ReceiveIndex = 0;
static volatile uint8_t TransmitBuffer[MAX_BUFFER_SIZE] = {0};
static volatile uint8_t TXByteCtr = 0;
static volatile uint8_t TransmitIndex = 0;
static uint8_t TransmitRegAddr = 0;
static volatile uint8_t StopBit_flag = 0;

static volatile uint8_t SlaveAddr = 0;
static volatile uint8_t SlaveReceivedReg = 0;
static volatile uint8_t SlaveReceivedData = 0xFF;
static volatile uint8_t ReqSlaveAddr = 0;
static volatile uint8_t ReqSlaveReceivedReg = 0;
Slave_data_t * slave_data;
Arms_mock_tx_data * mock_tx;

I2C_Mode i2cMasterMode = IDLE_MODE;
I2C_Mode i2cSlaveMode = IDLE_MODE;


int i2cInit_Slave(void){
  SlaveAddr = SLAVE_ADDR_DEFAULT;
  // Enable Write-access to modify port mapping registers
  PMAPPWD = 0x02D52;
  // I2C Pins
  P2SEL |= BIT1 | BIT2;
  P2MAP1 = PM_UCB0SDA;
  P2MAP2 = PM_UCB0SCL;
  // Disable Write-Access to modify port mapping registers
  PMAPPWD = 0;
  UCB0CTL1 |= UCSWRST;           // Enable SW reset
  UCB0CTL0 = UCMODE_3 + UCSYNC;  // I2C synchronous mode
  UCB0I2COA = SlaveAddr;  // Own Address
  UCB0CTL1 &= ~UCSWRST;    // Clear SW reset, resume operation
  UCB0IE = UCSTTIE;
  slave_data = malloc(sizeof(Slave_data_t));
  mock_tx   =  malloc(sizeof(Arms_mock_tx_data));
  mock_tx->pca_l_mock_data = 0x00;
  mock_tx->pca_r_mock_data = 0x00;
  slave_data->pca_l_data = 0xFF;
  slave_data->pca_r_data = 0xFF;
  slave_data->pca_reg_addr = 0xFF;
  slave_data->pca_addr = 0xFF;
  slave_data->pca_l_config_data = 0xFF;
  slave_data->pca_r_config_data = 0xFF;
  return 0;
}

void i2cInit_Master(void){
    P8SEL |= (BIT5 | BIT6);                 //Select i2c function for pins 8.5/8.6
    UCB1CTL1 |= UCSWRST;                      // Enable SW reset
    UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC;             // I2C Master, synchronous mode
    UCB1CTL1 = UCSSEL_2 + UCSWRST;
    UCB1BR0 = 10;                            // fSCL = SMCLK/10 = ~100kHz
    UCB1BR1 = 0;// Use SMCLK, keep SW reset
    UCB1CTL1 &= ~UCSWRST;  // Clear the software reset.
    UCB1IE |= UCNACKIE;
}


static inline void i2cEnable_Slave(void){
    UCB0CTL1 &= ~UCSWRST;
    UCB0IE |= UCSTTIE;
}

static inline void i2cDeinit_Slave(void){
    UCB0IE &= ~(UCNACKIE | UCRXIE | UCTXIE | UCSTTIE | UCSTPIE);
    UCB0CTL1 |= UCSWRST;
}


I2C_Mode I2C_Master_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count)
{
    /* Initialize state machine */
    i2cMasterMode = TX_REG_ADDRESS_MODE;
    TransmitRegAddr = reg_addr;
    RXByteCtr = count;
    TXByteCtr = 0;
    ReceiveIndex = 0;
    TransmitIndex = 0;

    /* Initialize slave address and interrupts */
    UCB1I2CSA = dev_addr;
    UCB1IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
    UCB1IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB1IE |= UCTXIE;                        // Enable TX interrupt

    UCB1CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
 //   __bis_SR_register(LPM0_bits + GIE);              // Enter LPM0 w/ interrupts

    return i2cMasterMode;

}

I2C_Mode I2C_Master_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count)
{
    uint16_t timeout = 0;
    /* Initialize state machine */
    i2cMasterMode = TX_REG_ADDRESS_MODE;
    TransmitRegAddr = reg_addr;

    //Copy register data to TransmitBuffer
    memcpy(TransmitBuffer, reg_data, count);

    TXByteCtr = count;
    RXByteCtr = 0;
    ReceiveIndex = 0;
    TransmitIndex = 0;
    StopBit_flag = 0;

    /* Initialize slave address and interrupts */
    UCB1I2CSA = dev_addr;
    UCB1IFG &= ~(UCTXIFG + UCRXIFG);       // Clear any pending interrupts
    UCB1IE &= ~UCRXIE;                       // Disable RX interrupt
    UCB1IE |= UCTXIE;                        // Enable TX interrupt

    UCB1CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    //    __bis_SR_register(LPM0_bits + GIE);              // Enter LPM0 w/ interrupts
    timeout = millis();
    while(!(StopBit_flag)){
          if (ellapsed_millis(timeout) > 2500){                  //if timeout flag is set then break
              i2cMasterMode = TIMEOUT_MODE;
              break;
          }
    }
    return i2cMasterMode;
}

static void I2C_Slave_ChangeAddr(uint8_t slave_addr){
    i2cDeinit_Slave();
    UCB0I2COA = slave_addr;
    SlaveAddr = slave_addr;
    i2cEnable_Slave();
}

uint8_t I2C_Slave_Listen(uint8_t slave_addr, uint8_t slave_reg){
    I2C_Slave_ChangeAddr(slave_addr);
    ReqSlaveReceivedReg = slave_reg;
    ReqSlaveAddr = slave_addr;      //Block slave address change.
    if (slave_data == NULL){
        slave_data = malloc(sizeof(Slave_data_t));
    }
    return 0;
}

uint8_t I2C_Slave_ReadReg(void){
    uint8_t tmp = 0;
    tmp = slave_data->pca_reg_addr;
    return tmp;
}

uint8_t I2C_Slave_ReadData(void){
    uint8_t tmp = 0;
    if (ReqSlaveAddr == PCA9534_L){
        tmp = slave_data->pca_l_data;
    }
    else if (ReqSlaveAddr == PCA9534_R){
        tmp = slave_data->pca_r_data;
    }
    return tmp;
}

uint8_t I2C_Slave_set_mock_data(uint8_t pca_l, uint8_t pca_r){
    mock_tx->pca_l_mock_data = pca_l;
    mock_tx->pca_r_mock_data = pca_r;
    return 0;
}

static void I2C_Slave_tick_reception(void){
    if (SlaveAddr == PCA9534_L){
        if ((SlaveReceivedReg == 0x01) && (slave_data->pca_l_config_data != 0xFF)){
            slave_data->pca_l_data = SlaveReceivedData;
            I2C_Slave_ChangeAddr(PCA9534_R);

        }
        if (SlaveReceivedReg == 0x03){
            slave_data->pca_l_config_data = SlaveReceivedData;
        }
    }
    else if (SlaveAddr == PCA9534_R){
        if ((SlaveReceivedReg == 0x01) && (slave_data->pca_r_config_data != 0xFF)){
            slave_data->pca_r_data = SlaveReceivedData;
            I2C_Slave_ChangeAddr(PCA9534_L);
        }
        if (SlaveReceivedReg == 0x03){
            slave_data->pca_r_config_data = SlaveReceivedData;
        }
    }
}

#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
  uint8_t rx_val = 0;

  switch(__even_in_range(UCB1IV,0xC))
  {
      case USCI_NONE:break;                             // Vector 0 - no interrupt
      case USCI_I2C_UCALIFG:break;                      // Interrupt Vector: I2C Mode: UCALIFG
      case USCI_I2C_UCNACKIFG:      // Interrupt Vector: I2C Mode: UCNACKIFG  Not akn interrupt
          i2cMasterMode = NACK_MODE;
          break;
      case USCI_I2C_UCSTTIFG:       // Interrupt Vector: I2C Mode: UCSTTIFG Start condition interrup
          break;
      case USCI_I2C_UCSTPIFG:
          break;                     // Interrupt Vector: I2C Mode: UCSTPIFG Stop condition interrupt
      case USCI_I2C_UCRXIFG:
              rx_val = UCB1RXBUF;
              if (RXByteCtr)
              {
                ReceiveBuffer[ReceiveIndex++] = rx_val;
                RXByteCtr--;
              }

              if (RXByteCtr == 1)
              {
                UCB1CTL1 |= UCTXSTP;
              }
              else if (RXByteCtr == 0)
              {
                UCB1IE &= ~UCRXIE;
                i2cMasterMode = IDLE_MODE;
              //  __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
              }
          break;                      // Interrupt Vector: I2C Mode: UCRXIFG
      case USCI_I2C_UCTXIFG:
        switch (i2cMasterMode)
        {
        case TX_REG_ADDRESS_MODE:
            UCB1TXBUF = TransmitRegAddr;
            if (RXByteCtr)
                i2cMasterMode = SWITCH_TO_RX_MODE;   // Need to start receiving now
            else
                i2cMasterMode = TX_DATA_MODE;        // Continue to transmission with the data in Transmit Buffer
            break;

        case SWITCH_TO_RX_MODE:
            UCB1IE |= UCRXIE;              // Enable RX interrupt
            UCB1IE &= ~UCTXIE;             // Disable TX interrupt
            UCB1CTL1 &= ~UCTR;            // Switch to receiver
            i2cMasterMode = RX_DATA_MODE;    // State state is to receive data
            UCB1CTL1 |= UCTXSTT;          // Send repeated start
            if (RXByteCtr == 1)
            {
                //Must send stop since this is the N-1 byte
                while((UCB1CTL1 & UCTXSTT));
                UCB1CTL1 |= UCTXSTP;      // Send stop condition
            }
            break;

        case TX_DATA_MODE:
            if (TXByteCtr)
            {
                UCB1TXBUF = TransmitBuffer[TransmitIndex++];
                TXByteCtr--;

            }
            else
            {
                //Done with transmission
                UCB1CTL1 |= UCTXSTP;     // Send stop condition
                StopBit_flag = 1;
                i2cMasterMode = IDLE_MODE;
                UCB1IE &= ~UCTXIE;                       // disable TX interrupt
                //__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
            }
            break;

        default:
            __no_operation();
            break;
        }
          break;                      // Interrupt Vector: I2C Mode: UCTXIFG
      default: break;
    }
  }




#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    //Must read from UCB0RXBUF
    uint8_t rx_val = 0;
//    uint8_t tx_val = 8;

    switch(__even_in_range(UCB0IV, 0xC))
    {
        case USCI_NONE:break;                             // Vector 0 - no interrupt
        case USCI_I2C_UCALIFG:
            break;                      // Interrupt Vector: I2C Mode: UCALIFG
        case USCI_I2C_UCNACKIFG:
            break;
        case USCI_I2C_UCSTTIFG:       // Interrupt Vector: I2C Mode: UCSTTIFG Start condition interrupt
            if (UCB0CTL1 & UCTR){
                i2cSlaveMode = TX_DATA_MODE;
                UCB0IE &= ~UCSTTIE;
                UCB0IE |= UCTXIE;
                UCB0IE &= ~UCRXIE;
            }
            else {
                i2cSlaveMode = RX_REG_ADDRESS_MODE;
                UCB0IE &= ~UCSTTIE;
                UCB0IE |= UCRXIE;
                UCB0IE &= ~UCTXIE;
            }
            break;
        case USCI_I2C_UCSTPIFG:
            if (i2cSlaveMode == NACK_MODE){
                if (SlaveAddr == PCA9534_L){
                    UCB0CTL1 |= UCSWRST;
                    UCB0I2COA = PCA9534_R;
                    SlaveAddr = PCA9534_R;
                    UCB0CTL1 &= ~UCSWRST;
                }
                else if (SlaveAddr == PCA9534_R){
                    UCB0CTL1 |= UCSWRST;
                    UCB0I2COA = PCA9534_L;
                    SlaveAddr = PCA9534_L;
                    UCB0CTL1 &= ~UCSWRST;
               }
            }
            i2cSlaveMode = IDLE_MODE;
            UCB0IE |= UCSTTIE;
            break;                     // Interrupt Vector: I2C Mode: UCSTPIFG Stop condition interrupt
        case USCI_I2C_UCTXIFG:
            if (SlaveAddr == PCA9534_L){
                UCB0TXBUF = mock_tx->pca_l_mock_data;
            }
            else if (SlaveAddr == PCA9534_R){
                UCB0TXBUF = mock_tx->pca_r_mock_data;
            }
            i2cSlaveMode = NACK_MODE;
            UCB0IE |= UCSTPIE;
            break;
        case USCI_I2C_UCRXIFG:
            rx_val = UCB0RXBUF;
            switch (i2cSlaveMode){
            case RX_REG_ADDRESS_MODE:
              SlaveReceivedReg = rx_val;
              i2cSlaveMode = RX_DATA_MODE;
              UCB0IE |= UCSTTIE;
              break;
            case RX_DATA_MODE:
              UCB0IE &= ~UCSTTIE;
              UCB0IE &= ~UCRXIE;
              SlaveReceivedData = rx_val;
              i2cSlaveMode = IDLE_MODE;
              I2C_Slave_tick_reception();
              UCB0IE |= UCSTTIE;
              break;
            }
            break;
        default :
          __no_operation();
          break;
        }
 }


