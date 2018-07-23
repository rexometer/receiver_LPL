# LowPowerLab-Receiver

Use LowPowerLab instead of Jeelab for the rfm2pi, to improved RFM reliability.

1. Copy the compiled firmware to your pi: `git clone https://github.com/rexometer/receiver_LPL.git`
2. Stop emonhub: `sudo service emonhub stop`
3. Flash it: `avrdude -v -c arduino -p ATMEGA328P -P /dev/ttyAMA0 -b 38400 -U flash:w:receiver_LPL/compiled/firmware.hex` or use the 1200 baud rate version for longer range: `avrdude -v -c arduino -p ATMEGA328P -P /dev/ttyAMA0 -b 38400 -U flash:w:receiver_LPL/compiled/firmware_1200.hex`
4. Start emonhub again: `sudo service emonhub start`
