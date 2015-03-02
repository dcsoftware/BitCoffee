/**************************************************************************/
/*!
 @file     emulatetag.h
 @author   Armin Wieser
 @license  BSD
 
 Implemented using NFC forum documents & library of libnfc
 */
/**************************************************************************/

#ifndef __MYCARD_H__
#define __MYCARD_H__

#include <MPN532.h>

#define C_APDU_CLA   0
#define C_APDU_INS   1 // instruction
#define C_APDU_P1    2 // parameter 1
#define C_APDU_P2    3 // parameter 2
#define C_APDU_LC    4 // length command
#define C_APDU_DATA  5 // data

#define C_APDU_P1_SELECT_BY_ID   0x00
#define C_APDU_P1_SELECT_BY_NAME 0x04

// Response APDU
#define R_APDU_SW1_COMMAND_COMPLETE 0x90
#define R_APDU_SW2_COMMAND_COMPLETE 0x00

#define R_APDU_SW1_NDEF_TAG_NOT_FOUND 0x6a
#define R_APDU_SW2_NDEF_TAG_NOT_FOUND 0x82

#define R_APDU_SW1_FUNCTION_NOT_SUPPORTED 0x6A
#define R_APDU_SW2_FUNCTION_NOT_SUPPORTED 0x81

#define R_APDU_SW1_MEMORY_FAILURE 0x65
#define R_APDU_SW2_MEMORY_FAILURE 0x81

#define R_APDU_SW1_END_OF_FILE_BEFORE_REACHED_LE_BYTES 0x62
#define R_APDU_SW2_END_OF_FILE_BEFORE_REACHED_LE_BYTES 0x82
#define R_PRIV_ADDRESS_BYTE1 0xAA
#define R_PRIV_ADDRESS_BYTE2 0xAA
#define R_SW1_ERROR_AUTH 0xB1
#define R_SW2_ERROR_AUTH 0xB2
#define R_SW1_STATUS_WAITING 0x22
#define R_SW2_STATUS_WAITING 0x33
#define R_SW1_STATUS_RECHARGED 0x33
#define R_SW2_STATUS_RECHARGED 0x44
#define R_SW1_STATUS_PURCHASE 0x44
#define R_SW2_STATUS_PURCHASE 0x55
#define R_SW1_STATUS_DATA_UPDATED 0x55
#define R_SW2_STATUS_DATA_UPDATED 0x66
#define R_SW1_PRIV_APP_SELECTED 0x66
#define R_SW2_PRIV_APP_SELECTED 0x77

// ISO7816-4 commands
#define SELECT_FILE 0xA4
#define READ_BINARY 0xB0
#define UPDATE_BINARY 0xD6
#define AUTHENTICATE 0x20
#define LOG_IN 0x30
#define READING_STATUS 0x40
#define UPDATE_CREDIT 0x50

#define S_COM_RECHARGE 0x52
#define S_COM_PURCHASE 0x50

#define SERIAL_COMMAND_CONNECTION "connection:"
#define SERIAL_COMMAND_RECHARGE "recharge:"
#define SERIAL_COMMAND_PURCHASE "purchase:"
#define SERIAL_COMMAND_SET_DATA "set_data:"
#define SERIAL_COMMAND_GET_TIME "get_time:"
#define SERIAL_COMMAND_SET_TIME "set_time:"
#define SERIAL_COMMAND_GET_DATE "get_date:"
#define SERIAL_COMMAND_LOG "log:";
#define SERIAL_RESPONSE_OK "ok;"
#define SERIAL_RESPONSE_ERROR "err;"
#define SERIAL_VALUE_REQUEST "req;"
#define SERIAL_COMMAND_START "<"
#define SERIAL_COMMAND_END ">"


#define NDEF_MAX_LENGTH 128  // altough ndef can handle up to 0xfffe in size, arduino cannot.
typedef enum {COMMAND_COMPLETE, TAG_NOT_FOUND, FUNCTION_NOT_SUPPORTED, MEMORY_FAILURE,
	END_OF_FILE_BEFORE_REACHED_LE_BYTES, PRIV_APPLICATION_SELECTED, STATUS_WAITING, STATUS_RECHARGED,
	STATUS_PURCHASE, STATUS_DATA_UPDATED, AUTH_ERROR} responseCommand;

typedef enum {WAITING, CONNECTED, AUTHENTICATED, LOGGED, WAITING_SERIAL,
	RECHARGE, PURCHASE, DISCONNECTED, ERROR_AUTH } CardState;

typedef enum {LOGIN, LOGOUT, PURCHASE_TRANSACTION, RECHARGE_TRANSACTION, NOTHING} Event;


class MyCard{
    
public:
    MyCard(PN532Interface &interface) : pn532(interface), uidPtr(0), tagWrittenByInitiator(false), tagWriteable(true), updateNdefCallback(0) { }


    bool init();

    bool emulate(const uint16_t tgInitAsTargetTimeout = 0);
    
    void setSecretKey(String key);
    String getSecretKey();
    void setId(String id);
    String getId();
    void setLastTransactionId(long lastId);
    String getLastTransactionId();

    /*
     * @param uid pointer to byte array of length 3 (uid is 4 bytes - first byte is fixed) or zero for uid
     */
    void setUid(uint8_t* uid = 0);
    
    void setNdefFile(const uint8_t* ndef, const int16_t ndefLength);
    
    void getContent(uint8_t** buf, uint16_t* length){
        *buf = ndef_file + 2; // first 2 bytes = length
        *length = (ndef_file[0] << 8) + ndef_file[1];
    }
    
    bool writeOccured(){
        return tagWrittenByInitiator;
    }
    
    void setTagWriteable(bool setWriteable){
        tagWriteable = setWriteable;
    }
    
    uint8_t* getNdefFilePtr(){
        return ndef_file;
    }
    
    uint8_t getNdefMaxLength(){
        return NDEF_MAX_LENGTH;
    }
    
    void attach(void (*func)(uint8_t *buf, uint16_t length)) {
        updateNdefCallback = func;
    };

    
private:
    PN532 pn532;
    uint8_t ndef_file[NDEF_MAX_LENGTH];
    uint8_t* uidPtr;
    bool tagWrittenByInitiator;
    bool tagWriteable;
    void (*updateNdefCallback)(uint8_t *ndef, uint16_t length);
    
    void setResponse(responseCommand cmd, uint8_t* buf, uint8_t* sendlen, uint8_t sendlenOffset = 0);
    void sendRequest(Event event);
};

#endif
