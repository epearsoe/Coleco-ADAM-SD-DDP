Coleco ADAM SD DDP firmware version 1.5 7/2/2020 (initial release 01/01/2020)

The information within this document is to be used at your own risk. Any loss of data or failure of hardware is the sole responsibility of the end
user. If you do not agree to these terms and conditions, do not continue.

NOTE: The Arduino pinouts and some components have changed as of version 1.3.

NOTE: Be careful and DO NOT connect anything to the voltage pins on the ADAM DDP Drive connector. This could damage your ADAM, Arduino or PC/Mac. See the "ADAM DDP Drive Connector Pinout" section in the "Coleco ADAM SD DDP Build Guide and Info Pack.pdf".

Description:

Emulates ADAM Digital Data Drive (DDP)
Full Compatibility: Works with all existing ADAMs. It bridges the ADAM Tape 6801 to
    the FAT16/FAT32 file system on the SD Card. This means no software to install on the ADAM.
Drive Emulation: Acts as a single ADAM DDP drive. Because of hard coded timing in the ADAM the SD DDP
    block load speed is equivalent to a real DDP. The SD DDP is a little faster at fast forwarding and
    rewinding.
Disk Selection: The MOUNT button selects which of the "ddp tapes" to use. You can browse through the available files using the BACK and FORWARD buttons.
Disks are Files: These "ddp tapes" are simply files on the SD Card, and can be manipulated as such
    when the SD card is plugged in to a PC/Mac.
Existing Disks: Works with .ddp images found on the Internet. Put the SD Card in a
    Using a PC or Mac, copy up to 100 files (per directory) to a FAT32 formated SD card. These must be in ADAM DDP format commonly found on the web. You can find many images at:

http://adamarchive.org/

ADAM SD DDP Versions:
The internal power version is for use with the Coleco ADAM SD DDP Internal Power PCB. The components on the PCB are powered by tapping 5V from pin J11-4 on the ADAM DDP Connector (or J13-4).

Building your own SD DDP:
Follow the instructions provided in the "Coleco ADAM SD DDP Build Guide and Info Pack.pdf".

Known Quirks of the ADAM SD DDP Drive:
1. The "Tape Mode" button.  Coleco used two different tape formats for the ADAM; a center directory format and a right directory format (primarily used for "Super Games").  If you plan to use a real DDP drive with the ADAM SD DDP you should use the "Tape Mode" button to set the ADAM SD DDP to the same format as the tape you will be using in the real DDP. If you want to switch tape formats you will need to select it with the "Tape Mode" button and then pull the ADAM computer reset switch. This problem exists also if you are using two real ADAM DDP drives.

ADAM SD DDP Drive OLED Display Legend:
1. In the top left corner of the screen will be the letters "CD" for center directory tape mode or "RD" for right directory tape mode.  This can be changed using the "Tape Mode" button.
2. In the top center of the screen will be the current drive status. This could show "STOP", ">>" (fast forward), "<<" (fast rewind), "> NNN" (slow forward where NNN is the current track), or "<" (slow rewind).
3. Below the top line will be a filename of a DDP image (if any .DDP files are found on the SD card). If the text is in reverse mode this means this file is "mounted".

Included are two utility programs called deinterleave.exe and interleave.exe for Windows 10 PCs. Deinterleave.exe will convert a ADAM DSK image to an ADAM DDP formated image.  Interleave.exe does the opposite. It will convert an ADAM DDP image to an ADAM DSK formatted image.