# Hammer-The-Rat-FPGA-Game

  This game was done using the [Nexys A7-100T](https://digilent.com/shop/nexys-a7-fpga-trainer-board-recommended-for-ece-curriculum/?srsltid=AfmBOor-j94smkyUzJJtgCD7DYBEJNTsaMscAzg0IxDlpLPNAo556EUs) board. We made use of the ADXL362 chip on the board which is a 3-axis accelerometer communicated through Serial Peripheral Interface (SpiCore), 12-bit VGA output(vgaCore) to dynamically update the scores and display the starting screen, USB HID for keyboards (ps2Core) to signal a start, and general purpose inputs and outputs. 
