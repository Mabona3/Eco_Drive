# Arduino-Nano Connections
----------------------------------------------------------

The GPS will be connected using SPI and the Wi-Fi module will be connected in `UART`
The `UART` pins are D0 as RX and D1 as TX actually they should be labeled RX and TX
The `SPI` pins uses:
- D12 as CIPO 
- D11 as COPI 
- D10 as SS
- D13 is the clock

When Testing on the laptop the UART only is used with the Wi-Fi module and i can send the data using the usb with the monitor or `socat` is also available:
- Black: GND 
- 2-Whites and 0-Gray are GPIO
- Orange is the RX
- Blue is TX
- Green is RST
- Yellow is CH_PD connected as VCC with the Purple

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html
