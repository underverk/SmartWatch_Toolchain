SmartWatch toolchain
====================

Open source toolchain to compile code for the Sony SmartWatch the "Arduino" way.  
This toolchain was created as a proof of concept that it is possible to program Arduino-like code on almost anything. 

Download the Sony SmartWatch edition of the Arduino IDE to compile and run sketches using this toolchain:  
link-is-coming-don't-panic


### To build from the command line:

You will need some tools:  

GNU Tools for ARM: https://launchpad.net/gcc-arm-embedded/+download  
dfu-util (for downloading to device): http://dfu-util.gnumonks.org/releases/  

Windows users additionally need to download and configure MinGW or some other build environment.
http://www.mingw.org/ and make sure your PATH environment variable has both the MinGW bin and msys\bin, as well as dfu-util folders in it.


Then simply, from the root of this repo:  
* `> make´  will build bin/firmware.bin  
* `> make program´  will download to the device  
* `> make clean´ will clean it all up  