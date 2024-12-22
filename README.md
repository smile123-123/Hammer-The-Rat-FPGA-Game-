# Hammer-The-Rat-FPGA-Game 🐀🔨

  This game was done using the [Nexys A7-100T](https://digilent.com/shop/nexys-a7-fpga-trainer-board-recommended-for-ece-curriculum/?srsltid=AfmBOor-j94smkyUzJJtgCD7DYBEJNTsaMscAzg0IxDlpLPNAo556EUs) board. We made use of the ADXL362 chip on the board which is a 3-axis accelerometer communicated through Serial Peripheral Interface (SpiCore), 12-bit VGA output(vgaCore) to dynamically update the scores and display the starting screen, USB HID for keyboards (ps2Core) to signal a start, and general purpose inputs and outputs. 

How it works : The moniter screen would start a prompt asking the user to click anywhere to start. A 16x16 pixel rat would run across the screen to a target where a hammer is seen. To control the hammer the user has to physically hold the fpga board in an upright position and motion it hit when the rat gets to where the target is and the score depending on how accurately the mouse was hit will be displayed on the top of the screen. 

https://github.com/smile123-123/Hammer-The-Rat-FPGA-Game-/blob/main/Vitis_files/main_video_test.cpp
