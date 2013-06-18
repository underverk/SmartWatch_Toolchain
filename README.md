SmartWatch toolchain
====================

Open source toolchain to compile code for the Sony SmartWatch the "Arduino way".  
This toolchain was created as a proof of concept that it is possible to program Arduino-like code on almost anything.  

Download the Sony SmartWatch edition of the Arduino IDE to compile and run sketches using this toolchain:  
`[link-is-coming-do-not-panic]`


### To build from the command line:

You will need some tools:  

GNU Tools for ARM - https://launchpad.net/gcc-arm-embedded/+download  
dfu-util (for downloading to device) - http://dfu-util.gnumonks.org/releases/  

Windows users additionally need to download MinGW or some other GNU development environment.  
MinGW provides a step-by-step quide here: http://www.mingw.org/wiki/Getting_Started  

Then simply, from the root of this repo:  
* `> make`  will build bin/firmware.bin  
* `> make program`  will download to the device  
* `> make clean` will clean it all up  