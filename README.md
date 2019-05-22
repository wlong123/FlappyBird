# FlappyBird
This project was created using an FRDM-K64F board and an Adafruit 128x32 I2C OLED display. The display was put onto a breadboard and wired to pins on the board. The code in this repository enables the board to communicate with the display using I2C protocols and controls the flow of the game. All updates to the game state are done using an interrupt handler which occurs 30 times a second to ensure gameplay is smooth. Additionally, we used the SW3 switch on the board to take in user input to enable to users bird to "flap".
