#include <memory>
#include "base.h"
#include "smartwatch.h"
#include "fast_spi.h"
#include "i2c.h"

std::unique_ptr<Smartwatch> smartwatch;

BLEDfu      bledfu;  // OTA DFU service
BLEUart     bleuart; // uart over ble

TimerHandle_t buttonTimer;
TimerHandle_t chargingTimer;
TimerHandle_t powerTimer;

// callback invoked when central connects
void connect_callback(uint16_t conn_handle) {
    smartwatch->push_message(Smartwatch::Messages::BleConnected);
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
    //Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void stop_timer_callback(TimerHandle_t xTimer) {
    xTimerStop(xTimer, 0);
}

void button_callback(void) {
    if (digitalRead(KEY_ACTION) == HIGH) {
        if( xTimerIsTimerActive( buttonTimer ) == pdFALSE ) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(buttonTimer, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            smartwatch->push_message(Smartwatch::Messages::OnButtonEvent);
        }
    }
}

void tp_callback(void) {
    if (digitalRead(TP_IRQ) == LOW) {
        smartwatch->push_message(Smartwatch::Messages::OnTouchEvent);
    }
}

void charging_callback(void) {
    if (digitalRead(CHARGE_IRQ) == HIGH) {
        if( xTimerIsTimerActive( chargingTimer ) == pdFALSE ) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(chargingTimer, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            smartwatch->push_message(Smartwatch::Messages::OnChargingEvent);
        }
    }
}


void power_callback(void) {
    if (digitalRead(CHARGE_BASE_IRQ) == HIGH) {
        if( xTimerIsTimerActive( powerTimer ) == pdFALSE ) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTimerStartFromISR(powerTimer, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            smartwatch->push_message(Smartwatch::Messages::OnPowerEvent);
        }
    }
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

    if ( inputEnd == 1 && bleuart.read8() == COMMAND_BASE ) {
        inputEnd = 0;
        msgType = bleuart.read8();
        msgSize = bleuart.read16();
    }

    if ( bleuart.available() >= msgSize && inputEnd == 0 ) {        
        //smartwatch->setDebug(countrx++);
        smartwatch->push_message(Smartwatch::Messages::BleData);
        

        decode_message(msgType, msgSize);

        bleuart.flush();
        inputEnd = 1;
        msgSize = 0;
        msgType = 0;
    }    
        
}


//*****************************************************************************

void setup(void) {
    dwt_enable();

    // Smartwatch init
    pinMode(KEY_ENABLE, OUTPUT);
    digitalWrite(KEY_ENABLE, HIGH);

    pinMode(KEY_ACTION, INPUT_PULLDOWN);
    attachInterrupt(KEY_ACTION, button_callback, RISING); //ISR_DEFERRED | 

    pinMode(TP_IRQ, INPUT_SENSE_LOW);
    attachInterrupt(TP_IRQ, tp_callback, FALLING); // ISR_DEFERRED | 

    pinMode(CHARGE_IRQ, INPUT_PULLDOWN);
    attachInterrupt(CHARGE_IRQ, charging_callback, RISING); 

    pinMode(CHARGE_BASE_IRQ, INPUT_PULLDOWN);
    attachInterrupt(CHARGE_BASE_IRQ, power_callback, RISING); 

    init_fast_spi();
    init_i2c();

    smartwatch = std::make_unique<Smartwatch>();

    smartwatch->init();

    buttonTimer = xTimerCreate("buttonTimer", 300, pdFALSE, NULL, stop_timer_callback);

    chargingTimer = xTimerCreate("chargingTimer", 300, pdFALSE, NULL, stop_timer_callback);

    powerTimer = xTimerCreate("powerTimer", 300, pdFALSE, NULL, stop_timer_callback);

    // Bluetooth Config
    Bluefruit.begin(1, 0);
    Bluefruit.Periph.setConnectCallback(connect_callback);
    Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
    Bluefruit.setAppearance(BLE_APPEARANCE_GENERIC_WATCH);

    // To be consistent OTA DFU should be added first if it exists
    bledfu.begin();

    bleuart.begin();
    bleuart.setRxCallback(bleuart_rx_callback, false);

    startAdv();    

}


void loop(void) {
    //vTaskDelay(ms2tick(1000));
    //digitalToggle(LCD_LIGHT_3);
    smartwatch->hardware_update();
}
