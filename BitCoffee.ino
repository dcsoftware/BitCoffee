#include <MPN532_SPI.h>
#include "NfcAdapter.h"
#include <MNdefMessage.h>


PN532_SPI pn532spi(SPI, 10);
MyCard nfc(pn532spi);

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	Serial.begin(115200);
	delay(1000);

	nfc.setId("00005678");
	nfc.setSecretKey("ABCDEFGHIJ");
	nfc.setLastTransactionId(20000000);

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
