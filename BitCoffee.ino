#include <MPN532_SPI.h>
#include "NfcAdapter.h"
#include <MNdefMessage.h>

#define LEO_READY "ready"


PN532_SPI pn532spi(SPI, 10);
MyCard nfc(pn532spi);

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	Serial.begin(9600);
	while(!Serial);
	Serial1.begin(9600);

	Serial.println("log: waiting leo initializzation;");

	while(!Serial1.available());

	char serial1Buff[64];

	Serial1.readBytesUntil(10, serial1Buff, sizeof(serial1Buff));
	if(0 == memcmp(serial1Buff, LEO_READY, 5)) {
		Serial1.println("request");
		while(!Serial1.available()) ;
		Serial1.readBytesUntil(10, serial1Buff, sizeof(serial1Buff));
		String s = serial1Buff;
		nfc.setId(s.substring(0, 8));
		nfc.setSecretKey(s.substring(9, 19));
		String last = s.substring(20, 29);
		char c[9];
		last.toCharArray(c, sizeof(c));
		c[sizeof(c) - 1] = 0;
		nfc.setLastTransactionId(atol(c));

	}
//	nfc.setId("00005678");
//	nfc.setSecretKey("ABCDEFGHIJ");
//	nfc.setLastTransactionId(20000000);

	nfc.init();

	Serial.print("log: ID: ");
	Serial.print(nfc.getId());
	Serial.print(", Secret: ");
	Serial.print(nfc.getSecretKey());
	Serial.print(", Last Transaction: ");
	Serial.print(nfc.getLastTransactionId());
	Serial.println(";");
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	Serial.println("log:main loop;");
    nfc.emulate(5000);
    Serial.println("log:fine emulazione;");
}
