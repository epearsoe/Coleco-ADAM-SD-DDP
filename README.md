Coleco ADAM SD DDP version 1.4 6/17/2020 (initial release 01/01/2020)

Use at your own risk.

NOTE: The Arduino pinouts and some components have changed as of version 1.3.

Required Components:

1. Arduino MEGA 2560 R3 (~$13.00)
2. 0.96" I2C IIC Serial 128X64 White OLED LED Display Module for Arduino SSD1306 (~$7.00)
3. Arduino ARM MCU SD Card Module Slot Socket Reader N150 (<$4.00) *If you use a Micro SD card shield you may need to use pin 53 for CS instead of pin 10. You would need to adjust the code to support this.
4. 4 Momentary Push Buttons (<$5.00)
5. 4 10K resistors (~$1.00)
6. 1 1K resistor
7. 1 SN74LS06N
8. Breadboard (for button mounting) (<$2.00)

Arduino to ADAM Connection:

1. Follow the pin chart on page 4 and the connection diagram on page 5 of the "Coleco ADAM SD DDP Info Pack" to connect the ADAM to the Arduino, the Arduino to the OLED, SD Card Reader, and push buttons.

NOTE: Be careful and DO NOT connect anything to the voltage pins on the ADAM DDP Drive connector. This could damage your ADAM, Arduino or PC/Mac. See pages 2 & 3 of the "Coleco ADAM SD DDP Info Pack".

Installation:

1. Install the Arduino IDE and configure it for the Mega2560: https://www.arduino.cc/
2. Install the SD Card Library to your Arduino IDE: https://github.com/greiman/SdFat
3. Install two Adafruit libraries need to be installed: Adafruit_SSD1306, which handles the low-level communication with the hardware, and Adafruit_GFX, which builds atop this to add graphics functions like lines, circles and text. In recent versions of the Arduino IDE software (1.6.2 and later), this is most easily done through the Arduino Library Manager.
4. Install Fast Digital I/O library DIO2. https://www.codeproject.com/Articles/732646/Fast-digital-I-O-for-Arduino
5. How to install a library: https://www.arduino.cc/en/guide/libraries
6. Download the latest SD DDP release from GitHub. Load it in the Arduino IDE. You should see all of the SD DDP tabs open in the Arduino IDE.
7. Connect the Mega2560 to the USB port. Press the "Upload" button in the Arduino IDE to compile and upload.
8. You should see the SD DDP starting up on the OLED.

Operation:

Using a PC or Mac, copy up to 200 files to a FAT32 formated SD card. These must be in ADAM DDP format commonly found on the web. You can find many images at:

http://adamarchive.org/

Power-On Sequence:
1. Turn on the SD DDP.
2. Turn on the ADAM computer.
3. Select a DDP image to mount using the forward and backward selection buttons.
4. Mount a DDP image with the mount button. When an image is mounted it's filename will show in reverse text on the OLED.
5. Pull the computer reset switch (the left switch) on the ADAM computer to load and run the mounted image.

Known Quirks of the ADAM SD DDP Drive:
1. Because the DDP uses all active-low signals there is no way for the SD DDP to know when the ADAM is turned on or off.  If you mount an image before turning on the ADAM computer all buttons will cease to function until the ADAM is turned on.
2. The "Tape Mode" button.  Coleco used two different tape formats for the ADAM; a center directory format and a right directory format (primarily used for "Super Games").  If you plan to use a real DDP drive with the ADAM SD DDP you should use the "Tape Mode" button to set the ADAM SD DDP to the same format as the tape you will be using in the real DDP. If you want to switch tape formats you will need to select it with the "Tape Mode" button and then pull the ADAM computer reset switch. This problem exists also if you are using two real ADAM DDP drives.

ADAM SD DDP Drive OLED Display Legend:
1. In the top left corner of the screen will be the letters "CD" for center directory tape mode or "RD" for right directory tape mode.  This can be changed using the "Tape Mode" button.
2. In the top center of the screen will be the current drive status. This could show "STOP", ">>" (fast forward), "<<" (fast rewind), "> NNN" (slow forward where NNN is the current track), or "<" (slow rewind).
3. Below the top line will be a filename of a DDP image (if any .DDP files are found on the SD card). If the text is in reverse mode this means this file is "mounted".

Included are two utility programs called deinterleave.exe and interleave.exe for Windows 10 PCs. Deinterleave.exe will convert a ADAM DSK image to an ADAM DDP formated image.  Interleave.exe does the opposite. It will convert an ADAM DDP image to an ADAM DSK formatted image.