#include <memory> 
#include "base.h"
#include "smartwatch.h"
#include "fast_spi.h"
#include "i2c.h"
#include "utils.h"
#include "watchdog.h"

std::unique_ptr < Smartwatch > smartwatch;

BLEDfu bledfu; // OTA DFU service
BLEUart bleuart; // uart over ble

TimerHandle_t buttonTimer;
TimerHandle_t tpTimer;
//TimerHandle_t powerTimer;

bool send_info = true;

void stop_timer_callback(TimerHandle_t xTimer) {
    xTimerStop(xTimer, 0);
}

void button_callback(void) {
    if (digitalRead(KEY_ACTION) == HIGH) {
        if (xTimerIsTimerActive(buttonTimer) == pdFALSE) {
            smartwatch->push_message(Smartwatch::Messages::OnButtonEvent);
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(buttonTimer, & xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

void tp_callback(void) {
    if (digitalRead(TP_IRQ) == LOW) {
        //smartwatch->touch.get();
        //smartwatch->lvglmodule.set_touch_data(smartwatch->touch.getGesture(), smartwatch->touch.getEvent(), smartwatch->touch.getX(), smartwatch->touch.getY());
        if (xTimerIsTimerActive(tpTimer) == pdFALSE) {
            smartwatch->push_message(Smartwatch::Messages::OnTouchEvent);
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(tpTimer, & xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

/*
void charging_callback(void) {
    if (xTimerIsTimerActive(chargingTimer) == pdFALSE) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTimerStartFromISR(chargingTimer, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        if (digitalRead(CHARGE_IRQ) == HIGH) {
            smartwatch->set_charging(true);
            //smartwatch->push_message(Smartwatch::Messages::OnChargingEvent);
        } else {
            smartwatch->set_charging(false);
        }
    }
}


void power_callback(void) {
    if (digitalRead(CHARGE_BASE_IRQ) == HIGH) {
        if (xTimerIsTimerActive(powerTimer) == pdFALSE) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(powerTimer, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            //smartwatch->push_message(Smartwatch::Messages::OnPowerEvent);
        }
    }
}
*/

/* ****************************************************************************************************** */

void send_data_ble(const uint8_t * content, size_t len) {
    bleuart.write(content, len);
}

void ble_send_version(void) {
    uint8_t data[4] = {
        0x00,
        COMMAND_PT_VERSION,
        0x00,
        0x01
    };
    send_data_ble(data, 4);
}

void ble_send_battery(void) {

    uint8_t data[11] = {};
    uint8_t status = 0;

    if (smartwatch->battery.get_percent_remaining() == -1) {
        status = 0x01;
    } else if (smartwatch->is_charging()) {
        status = 0x02;
    } else {
        status = 0x03;
    }

    uint8_t i = 0;
    data[i++] = 0x00;
    data[i++] = COMMAND_PT_BATTERY;

    i += packInt(& data[i], smartwatch->battery.get_percent_remaining());
    i += packFloat(& data[i], (float)smartwatch->battery.get_voltage() / 1000);
    i += packByte(& data[i], status);

    send_data_ble(data, i);
}

void ble_send_steps(void) {

    uint8_t data[6] = {};

    uint8_t i = 0;
    data[i++] = 0x00;
    data[i++] = COMMAND_PT_STEPS;

    i += packInt(& data[i], 0);//smartwatch->stepCount.getStepCounterOutput());
    
    send_data_ble(data, i);
}

void ble_send_hr(void) {

    uint8_t data[3] = {};

    uint8_t i = 0;
    data[i++] = 0x00;
    data[i++] = COMMAND_PT_HEARTRATE;

    data[i++] = 0;//smartwatch->heartRate.getLastHR();
    
    send_data_ble(data, i);
}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle) {

    BLEConnection* conn = Bluefruit.Connection(conn_handle);

    // request PHY changed to 2MB
    //Serial.println("Request to change PHY");
    conn->requestPHY();
        
    // request mtu exchange
    //Serial.println("Request to change MTU");
    conn->requestMtuExchange(128);

    // request connection interval of 7.5 ms
    //conn->requestConnectionParameter(6); // in unit of 1.25

    // delay a bit for all the request to complete
    delay(1000);
    
    smartwatch->push_message(Smartwatch::Messages::BleConnected);
    send_info = true;
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

static uint32_t get_int() {
    return bleuart.read8() << 24 | bleuart.read8() << 16 | bleuart.read8() << 8 | bleuart.read8();
}

static uint8_t get_byte() {
    return bleuart.read8();
}

void get_notification(void) {

    char subject[31] = {};
    char body[61] = {};

    uint32_t id = get_int();
    uint8_t type = get_byte();
    uint8_t hour = get_byte();
    uint8_t minute = get_byte();

    uint8_t sizeSubject = get_byte();
    bleuart.read(subject, sizeSubject);
    //subject[size + 1] = 0x00;

    uint8_t sizeBody = get_byte();
    bleuart.read(body, sizeBody);
    //body[size + 1] = 0x00;

    smartwatch->notification.add_notification(id, type, smartwatch->rtc_time.get_timestamp(), 0, 0, 0, hour, minute, subject, sizeSubject, body, sizeBody);

    smartwatch->push_message(Smartwatch::Messages::NewNotification);

}

void get_weather(void) {
    uint8_t size;

    smartwatch->weather.currentTemp = get_byte();
    smartwatch->weather.currentHumidity = get_byte();
    smartwatch->weather.todayMaxTemp = get_byte();
    smartwatch->weather.todayMinTemp = get_byte();

    size = get_byte();
    smartwatch->weather.location = (char *)malloc(size + 1);
    bleuart.read(smartwatch->weather.location, size);
    smartwatch->weather.location[size + 1] = 0x00;

    size = get_byte();
    smartwatch->weather.currentCondition = (char *)malloc(size + 1);
    bleuart.read(smartwatch->weather.currentCondition, size);
    smartwatch->weather.currentCondition[size + 1] = 0x00;

    smartwatch->weather.newData = true;
    smartwatch->weather.hasData = true;

}

void decode_message(uint8_t msgType, int16_t msgSize) {
    switch (msgType) {
        case COMMAND_TIME_UPDATE:
            if (msgSize == 4) {
                smartwatch->rtc_time.set_time(get_int());
                smartwatch->push_message(Smartwatch::Messages::BleData);
            }
            break;
        case COMMAND_NOTIFICATION:
            if (msgSize > 4) {
                get_notification();
            }
            break;
        case COMMAND_DELETE_NOTIFICATION:
            if (msgSize == 4) {
                smartwatch->notification.clear_notification_by_id(get_int());
                smartwatch->push_message(Smartwatch::Messages::BleData);
            }
            break;
        case COMMAND_WEATHER:
            if (msgSize > 4) {
                get_weather();
                //smartwatch->vibration.vibrate(64, 50);
            }
            break;
        case COMMAND_FIND_DEVICE:
            //if (msgSize = 1) {
            smartwatch->set_notification("Find Device", "\nHello\nI'm here !!!", Smartwatch::MessageType::Info);
            smartwatch->push_message(Smartwatch::Messages::ShowMessage);
            smartwatch->vibration.vibrate(128, 500);

            //}
            break;

        case COMMAND_VIBRATION:
            smartwatch->push_message(Smartwatch::Messages::BleData);
            // smartwatch->set_notification("Find Device", "\nHello\nI'm here !!!",
            // Smartwatch::MessageType::Info);
            // smartwatch->push_message(Smartwatch::Messages::ShowMessage);
            smartwatch->vibration.vibrate(32, 50);
            vTaskDelay(pdMS_TO_TICKS(100));
            smartwatch->vibration.vibrate(64, 50);
            vTaskDelay(pdMS_TO_TICKS(100));
            smartwatch->vibration.vibrate(128, 50);
            vTaskDelay(pdMS_TO_TICKS(100));
            smartwatch->vibration.vibrate(64, 50);
            vTaskDelay(pdMS_TO_TICKS(100));
            smartwatch->vibration.vibrate(32, 50);
            
            //smartwatch->dont_sleep(true); smartwatch->backlight.set_level(3);
            break;

        default:
            // smartwatch->set_notification("Notification", "New notification on your
            // phone.", Smartwatch::MessageType::Info);
            // smartwatch->push_message(Smartwatch::Messages::ShowMessage);
            // smartwatch->vibration.vibrate(64, 50);
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
        //smartwatch->push_message(Smartwatch::Messages::BleData);
        bleuart.flush();
        inputEnd = 1;
        msgSize = 0;
        msgType = 0;
    }

}

// callback invoked when pairing passkey is generated
// - passkey: 6 keys (without null terminator) for displaying
// - match_request: true when authentication method is Numberic Comparison.
//             Then this callback's return value is used to accept (true) or
//              reject (false) the pairing process. Otherwise, return value has
// no effect
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
    } else {
        smartwatch->set_notification("Passkey", "Incorrect passkey!", Smartwatch::MessageType::Error);
        smartwatch->push_message(Smartwatch::Messages::ShowMessage);
    }
}

void connection_secured_callback(uint16_t conn_handle) {
    (void)conn_handle;
    // smartwatch->set_notification("Secured", "callback.",
    // Smartwatch::MessageType::Info);
    // smartwatch->push_message(Smartwatch::Messages::ShowMessage);
}

/* **************************************************************************** */

void startAdv(void) {
    // Advertising packet
    Bluefruit
        .Advertising
        .addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit
        .Advertising
        .addTxPower();

    // Include bleuart 128-bit uuid
    Bluefruit
        .Advertising
        .addService(bleuart);

    // Secondary Scan Response packet (optional) Since there is no room for 'Name'
    // in Advertising packet
    Bluefruit
        .ScanResponse
        .addName();

    /* Start Advertising
    * - Enable auto advertising if disconnected
    * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
    * - Timeout for fast mode is 30 seconds
    * - Start(timeout) with timeout = 0 will advertise forever (until connected)
    *
    * For recommended advertising interval
    * https://developer.apple.com/library/content/qa/qa1931/_index.html
    */
    Bluefruit
        .Advertising
        .restartOnDisconnect(true);
    //Bluefruit.Advertising.setInterval(400, 401);  // in unit of 0.625 ms
    Bluefruit
        .Advertising
        .setInterval(32, 244); // in unit of 0.625 ms
    Bluefruit
        .Advertising
        .setFastTimeout(30); // number of seconds in fast mode
    Bluefruit
        .Advertising
        .start(0); // 0 = Don't stop advertising after n seconds
}

void setup(void) {

    // Smartwatch init

    init_i2c();
    init_fast_spi();

    smartwatch = std::make_unique <Smartwatch>();

    smartwatch->init();

    pinMode(KEY_ENABLE, OUTPUT);
    digitalWrite(KEY_ENABLE, HIGH);

    pinMode(KEY_ACTION, INPUT);
    pinMode(KEY_ACTION, INPUT_PULLDOWN);
    attachInterrupt(KEY_ACTION, button_callback, RISING); //ISR_DEFERRED |

    pinMode(TP_IRQ, INPUT);
    pinMode(TP_IRQ, INPUT_SENSE_LOW);
    attachInterrupt(TP_IRQ, tp_callback, FALLING); // ISR_DEFERRED |

    /*pinMode(CHARGE_IRQ, INPUT_SENSE_HIGH);
    attachInterrupt(CHARGE_IRQ, charging_callback, CHANGE);

    pinMode(CHARGE_BASE_IRQ, INPUT_SENSE_HIGH);
    attachInterrupt(CHARGE_BASE_IRQ, power_callback, CHANGE);*/

    pinMode(CHARGE_IRQ, INPUT);
    pinMode(CHARGE_BASE_IRQ, INPUT);

    buttonTimer = xTimerCreate("buttonTimer", 300, pdFALSE, NULL, stop_timer_callback);

    tpTimer = xTimerCreate("tpTimer", 100, pdFALSE, NULL, stop_timer_callback);

    // powerTimer = xTimerCreate("powerTimer", 1000, pdFALSE, NULL,
    // stop_timer_callback); Bluetooth Config
    Bluefruit.begin(1, 0);
    Bluefruit.setAppearance(BLE_APPEARANCE_GENERIC_WATCH);
    //Bluefruit.setTxPower(8);    // Check bluefruit.h for supported values

    /* To use dynamic PassKey for pairing, we need to have
    * - IO capacities at least DISPPLAY
    *    - Display only: user have to enter 6-digit passkey on their phone
    *    - DIsplay + Yes/No: user ony need to press Accept on both central and device
    * - Register callback to display/print dynamic passkey for central
    *
    * For complete mapping of the IO Capabilities to Key Generation Method, check out this article
    * https://www.bluetooth.com/blog/bluetooth-pairing-part-2-key-generation-methods/
    */
    Bluefruit
        .Security
        .setIOCaps(true, false, false); // display = true, yes/no = true, keyboard = false
    //Bluefruit.Security.setMITM(true);
    Bluefruit
        .Security
        .setPairPasskeyCallback(pairing_passkey_callback);

    // Set complete callback to print the pairing result
    Bluefruit
        .Security
        .setPairCompleteCallback(pairing_complete_callback);

    // Set connection secured callback, invoked when connection is encrypted
    Bluefruit
        .Security
        .setSecuredCallback(connection_secured_callback);

    Bluefruit
        .Periph
        .setConnectCallback(connect_callback);
    Bluefruit
        .Periph
        .setDisconnectCallback(disconnect_callback);

    // To be consistent OTA DFU should be added first if it exists
    // bledfu.setPermission(SECMODE_ENC_WITH_MITM, SECMODE_ENC_WITH_MITM);
    bledfu.begin();
    
    //bleuart.setPermission(SECMODE_ENC_WITH_MITM, SECMODE_ENC_WITH_MITM);    
    bleuart.setRxCallback(bleuart_rx_callback, true);
    bleuart.bufferTXD(false);
    bleuart.begin();

    startAdv();

    watchdog_init(5000);
}

void send_ble_data() {
    if (send_info) {
        ble_send_version();
        send_info = false;
    }

    ble_send_battery();
    ble_send_steps();
    ble_send_hr();
}

void loop(void) {
    vTaskDelay(ms2tick(1000));
    // digitalToggle(LCD_LIGHT_3); smartwatch->hardware_update(); ble_send_battery();

    if (digitalRead(KEY_ACTION) == HIGH) 
        return;
    watchdog_feed();
}
