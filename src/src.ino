
// **********************************************************************************
// CHANGELOG
// **********************************************************************************
//
// V1.0: 15-08-2017: Initial version
//
//
//
// **********************************************************************************
#include <RFM69.h>         //get it here: https://github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     //get it here: https://github.com/lowpowerlab/RFM69
#include <SPI.h>           //included with Arduino IDE (www.arduino.cc)
#include <RFM69registers.h>
//****************************************************************************************************************
//**** IMPORTANT RADIO SETTINGS - YOU MUST CHANGE/CONFIGURE TO MATCH YOUR HARDWARE TRANSCEIVER CONFIGURATION! ****
//****************************************************************************************************************
#define LPL_lowbaudrate // uncomment to lower the baud rate resulting in longer range. Has to be set at transmitter and receiver.
#define NODEID          1 //the ID of this node
#define NETWORKID     200 //the network ID of all nodes this node listens/talks to
#define FREQUENCY     RF69_433MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define ENCRYPTKEY    "RExometer_enckey" //identical 16 characters/bytes on all nodes, not more not less!
//#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
#define ACK_TIME       1000  // # of ms to wait for an ack packet
//*****************************************************************************************************************************
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI      -70  //target RSSI for RFM69_ATC (recommended > -80)
//*****************************************************************************************************************************
// Serial baud rate must match your Pi/host computer serial port baud rate!
#define SERIAL_EN     //comment out if you don't want any serial verbose output
#define SERIAL_BAUD  38400
//*****************************************************************************************************************************

const int LEDpin = 9;

#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(LEDpin, OUTPUT);                     // Setup indicator LED
  digitalWrite(LEDpin, HIGH);
  delay(10);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);

  #ifdef LPL_lowbaudrate
    radio.writeReg(REG_BITRATEMSB, RF_BITRATEMSB_19200);
    radio.writeReg(REG_BITRATELSB, RF_BITRATELSB_19200);
    radio.writeReg(REG_FDEVMSB, RF_FDEVMSB_25000);
    radio.writeReg(REG_FDEVLSB, RF_FDEVLSB_25000);
    radio.writeReg(REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_3);
    radio.writeReg(REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF);
  #endif

#ifdef IS_RFM69HW
  radio.setHighPower(); //uncomment only for RFM69HW!
#endif
  radio.encrypt(ENCRYPTKEY);

#ifdef ENABLE_ATC
  radio.enableAutoPower(ATC_RSSI);
#endif
}

void loop() {

  if (radio.receiveDone())
  {
    Serial.print("OK ");
    Serial.print(radio.SENDERID, DEC);
    Serial.print(" ");
    for (byte i = 0; i<radio.DATALEN; i++)
    {
        Serial.print((word)radio.DATA[i]);
        Serial.print(" ");
    }
    Serial.print("(");
    Serial.print(radio.readRSSI());
    Serial.print(")");
    Serial.println();

    if (radio.ACKRequested())
    {
      byte theNodeID = radio.SENDERID;
      radio.sendACK();
      //DEBUG(" [ACK-sent]");
    }
    digitalWrite(LEDpin, HIGH); delay(20); digitalWrite(LEDpin, LOW);      // flash LED
  }
}
