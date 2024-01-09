# BNVD Test Board Firmware
This project contains the code for the bnvd-test-board. It uses an MCP430F6 proccessor and a number of peripherals, to be able to simulate real use cases.
The test-board relies on the PC software to receive Commands and after the execution, it will Respond with the result. The communication with the PC is achieved through a USB to Serial-UART interface.
The Command and Response messages are based on a fixed format that is described below.

## Message Format
**FORM:**			 START_CHAR-HEADER:PERIPHERAL:COMMAND2PERIPH:ARGUMENTS

**FIELDS LENGHT:**	 1B:3B:3B:3B:16B 

**EXAMPLE:**		 #CMD:ADC:GET:2(adc_channel)
