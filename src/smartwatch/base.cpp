#include <memory>
#include "base.h"
#include "smartwatch.h"
#include "fast_spi.h"
#include "i2c.h"
#include "utils.h"


std::unique_ptr<Smartwatch> smartwatch;

BLEDfu      bledfu;  // OTA DFU service
BLEUart     bleuart; // uart over ble

TimerHandle_t buttonTimer;
TimerHandle_t chargingTimer;
TimerHandle_t powerTimer;

void send_data_ble(const uint8_t *content, size_t len) {
    bleuart.write(content, len);
}

void ble_send_version(void) {
    uint8_t data[4] = {0x00, COMMAND_PT_VERSION, 0x00, 0x01};
    send_data_ble(data, 4);
}

void ble_send_battery(void) {

    uint8_t data[11] = {};
    uint8_t status = 0;

    if ( smartwatch->battery.get_percent_remaining() == -1 ) {
        status = 0x01;
    } else if ( smartwatch->is_charging() ) {
        status = 0x02;
    } else {
        status = 0x03;
    }

    uint8_t i = 0;
    data[i++] = 0x00;
    data[i++] = COMMAND_PT_BATTERY;

    i += packInt(&data[i], smartwatch->battery.get_percent_remaining());
    i += packFloat(&data[i], (float)smartwatch->battery.get_voltage() / 1000);
    i += packByte(&data[i], status);


    send_data_ble(data, i);
}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle) {
    smartwatch->push_message(Smartwatch::Messages::BleConnected);
    ble_send_version();
    ble_send_battery();
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
    (void)conn_handle;
    (void)reason;
    smartwatch->push_message(Smartwatch::Messages::BleDisconnected);
    bleuart.flush();
}

void startAdv(void) {
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();

    // Include bleuart 128-bit uuid
    Bluefruit.Advertising.addService(bleuart);

    // Secondary Scan Response packet (optional)
    // Since there is no room for 'Name' in Advertising packet
    Bluefruit.ScanResponse.addName();

    /* Start Advertising
     * - Enable auto advertising if disconnected
     * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     * - Timeout for fast mode is 30 seconds
     * - Start(timeout) with timeout = 0 will advertise forever (until connected)
     *
     * For recommended advertising interval
     * https://developer.apple.com/library/content/qa/qa1931/_index.html
     */
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(400, 401);    // in unit of 0.625 ms
    //Bluefruit.Advertising.setInterval(32, 244);   // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);       // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                 // 0 = Don't stop advertising after n seconds  
}

void stop_timer_callback(TimerHandle_t xTimer) {
    xTimerStop(xTimer, 0);
}

void button_callback(void) {
    if (digitalRead(KEY_ACTION) == HIGH) {
        if (xTimerIsTimerActive(buttonTimer) == pdFALSE) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(buttonTimer, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            smartwatch->push_message(Smartwatch::Messages::OnButtonEvent);
        }
    }
}

void tp_callback(void) {
    if (digitalRead(TP_IRQ) == LOW) {
        smartwatch->touch.read();
        //smartwatch->touch.get();
        smartwatch->push_message(Smartwatch::Messages::OnTouchEvent);
    }
}

void charging_callback(void) {
    if (xTimerIsTimerActive(chargingTimer) == pdFALSE) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTimerStartFromISR(chargingTimer, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        if (digitalRead(CHARGE_IRQ) == HIGH) {
            smartwatch->set_charging(true);
        } else {
            smartwatch->set_charging(false);
        }
        smartwatch->push_message(Smartwatch::Messages::OnChargingEvent);
    }
}


void power_callback(void) {
    //if (digitalRead(CHARGE_BASE_IRQ) == HIGH) {
        if (xTimerIsTimerActive(powerTimer) == pdFALSE) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(powerTimer, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            smartwatch->push_message(Smartwatch::Messages::OnPowerEvent);
        }
    //}
}

static uint32_t get_int() {
    return bleuart.read8() << 24 | bleuart.read8() << 16 | bleuart.read8() << 8 | bleuart.read8();
}

void decode_message(uint8_t msgType, int16_t msgSize) {
    switch (msgType) {
    case COMMAND_TIME_UPDATE:
        if (msgSize == 4) {
            smartwatch->rtc_time.set_time(get_int());
        }
        break;

    default:
        break;
    }
}


uint16_t countrx = 0;
uint8_t inputEnd = 1;

int16_t msgSize = 0;
uint8_t msgType = 0;

// Invoked when receiving data from bleuart
void bleuart_rx_callback(uint16_t conn_hdl) {
    (void)conn_hdl;

    if (inputEnd == 1 && bleuart.read8() == COMMAND_BASE) {
        inputEnd = 0;
        msgType = bleuart.read8();
        msgSize = bleuart.read16();
    }

    if (bleuart.available() >= msgSize && inputEnd == 0) {
        //smartwatch->setDebug(countrx++);

        decode_message(msgType, msgSize);
        smartwatch->push_message(Smartwatch::Messages::BleData);

        bleuart.flush();
        inputEnd = 1;
        msgSize = 0;
        msgType = 0;
    }

}

// callback invoked when pairing passkey is generated
// - passkey: 6 keys (without null terminator) for displaying
// - match_request: true when authentication method is Numberic Comparison.
//                  Then this callback's return value is used to accept (true) or
//                  reject (false) the pairing process. Otherwise, return value has no effect
bool pairing_passkey_callback(uint16_t conn_handle, uint8_t const passkey[6], bool match_request) {
    (void)conn_handle;

    memcpy(smartwatch->passkey, passkey, 6);
    smartwatch->passkey[6] = 0;
    smartwatch->push_message(Smartwatch::Messages::ShowPasskey);
    return true;
}

void pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status) {
    (void)conn_handle;
    if (auth_status == BLE_GAP_SEC_STATUS_SUCCESS) {
        smartwatch->set_notification("Passkey", "Pairing successful.", Smartwatch::MessageType::Info);
        smartwatch->push_message(Smartwatch::Messages::ShowMessage);
    }
    else {
        smartwatch->set_notification("Passkey", "Incorrect passkey!", Smartwatch::MessageType::Error);
        smartwatch->push_message(Smartwatch::Messages::ShowMessage);
    }
}

void connection_secured_callback(uint16_t conn_handle) {
    (void)conn_handle;
    //smartwatch->set_notification("Secured", "callback.", Smartwatch::MessageType::Info);
    //smartwatch->push_message(Smartwatch::Messages::ShowMessage);
}

//*****************************************************************************

void setup(void) {
    dwt_enable();

    // Smartwatch init

    init_fast_spi();
    init_i2c();

    smartwatch = std::make_unique<Smartwatch>();

    smartwatch->init();
    
    pinMode(KEY_ENABLE, OUTPUT);
    digitalWrite(KEY_ENABLE, HIGH);

    pinMode(KEY_ACTION, INPUT_PULLDOWN);
    attachInterrupt(KEY_ACTION, button_callback, RISING); //ISR_DEFERRED | 

    pinMode(TP_IRQ, INPUT_SENSE_LOW);
    attachInterrupt(TP_IRQ, tp_callback, FALLING); // ISR_DEFERRED | 

    pinMode(CHARGE_IRQ, INPUT_SENSE_HIGH);
    attachInterrupt(CHARGE_IRQ, charging_callback, CHANGE);

    pinMode(CHARGE_BASE_IRQ, INPUT_SENSE_HIGH);
    attachInterrupt(CHARGE_BASE_IRQ, power_callback, CHANGE);

    buttonTimer = xTimerCreate("buttonTimer", 300, pdFALSE, NULL, stop_timer_callback);

    chargingTimer = xTimerCreate("chargingTimer", 1000, pdFALSE, NULL, stop_timer_callback);

    powerTimer = xTimerCreate("powerTimer", 1000, pdFALSE, NULL, stop_timer_callback);

    // Bluetooth Config
    Bluefruit.begin(1, 0);
    Bluefruit.setAppearance(BLE_APPEARANCE_GENERIC_WATCH);
    //Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values

    /* To use dynamic PassKey for pairing, we need to have
    * - IO capacities at least DISPPLAY
    *    - Display only: user have to enter 6-digit passkey on their phone
    *    - DIsplay + Yes/No: user ony need to press Accept on both central and device
    * - Register callback to display/print dynamic passkey for central
    *
    * For complete mapping of the IO Capabilities to Key Generation Method, check out this article
    * https://www.bluetooth.com/blog/bluetooth-pairing-part-2-key-generation-methods/
    */
    Bluefruit.Security.setIOCaps(true, false, false); // display = true, yes/no = true, keyboard = false
    //Bluefruit.Security.setMITM(true);
    Bluefruit.Security.setPairPasskeyCallback(pairing_passkey_callback);

    // Set complete callback to print the pairing result
    Bluefruit.Security.setPairCompleteCallback(pairing_complete_callback);

    // Set connection secured callback, invoked when connection is encrypted
    Bluefruit.Security.setSecuredCallback(connection_secured_callback);

    Bluefruit.Periph.setConnectCallback(connect_callback);
    Bluefruit.Periph.setDisconnectCallback(disconnect_callback);    

    // To be consistent OTA DFU should be added first if it exists
    bledfu.setPermission(SECMODE_ENC_WITH_MITM, SECMODE_ENC_WITH_MITM);
    bledfu.begin();

    bleuart.begin();    
    bleuart.setPermission(SECMODE_ENC_WITH_MITM, SECMODE_ENC_WITH_MITM);    
    bleuart.setRxCallback(bleuart_rx_callback, false);
    bleuart.bufferTXD(false);

    startAdv();

}


void loop(void) {
    //vTaskDelay(ms2tick(1000));
    //digitalToggle(LCD_LIGHT_3);
    smartwatch->hardware_update();
    ble_send_battery();
}
