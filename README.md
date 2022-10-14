# Description
This project enumerates the usb keyboard and mouse, including composite and compound devices with ch559.  It can also receive the keyboard 
and mouse data. Sending report to usb keyboard is also implemented. 

# How to Use?
* Open usb_host_ch559.uvproj with MDK4.
* Option for target -> C51 -> Define, add DEBUG for debug version, otherwise for release version.
* flash the generated hex file into ch559, you can observe with a serial port tool.
