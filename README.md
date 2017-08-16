# LowPowerLab-Receiver

Use LowPowerLab instead of Jeelab for the rfm2pi, to improved RFM reliability.

1. Copy the compiled firmware to your pi
2. Stop emonhub: `sudo service emonhub stop`
3. Flash it: `avrdude -v -c arduino -p ATMEGA328P -P /dev/ttyAMA0 -b 38400 -U flash:w:firmware.hex`
4. Start emonhub again: `sudo service emonhub start`