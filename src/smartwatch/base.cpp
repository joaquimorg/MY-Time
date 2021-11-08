#include "base.h"

BLEDfu  bledfu;  // OTA DFU service
BLEUart bleuart; // uart over ble

TimerHandle_t buttonTimer;

// callback invoked when central connects
void connect_callback(uint16_t conn_handle) {
  //smartwatch.push_message(Smartwatch::Messages::BleConnected);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  (void)conn_handle;
  (void)reason;
  //smartwatch.push_message(Smartwatch::Messages::BleDisconnected);
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
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void button_timer_callback(TimerHandle_t xTimer) {
    xTimerStop(xTimer, 0);
    smartwatch.push_message(Smartwatch::Messages::OnButtonEvent);
}

void button_callback(void) {
    if ( digitalRead(KEY_ACTION) == HIGH ) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTimerStartFromISR(buttonTimer, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); 
    }
}

//*****************************************************************************

void setup(void) {
    dwt_enable();    

    // Smartwatch module init
    smartwatch.init();

    // Bluetooth Config
    Bluefruit.begin(1, 0);
    Bluefruit.Periph.setConnectCallback(connect_callback);
    Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
    Bluefruit.setAppearance(BLE_APPEARANCE_GENERIC_WATCH);

    // To be consistent OTA DFU should be added first if it exists
    bledfu.begin();

    bleuart.begin();

    buttonTimer = xTimerCreate ("buttonTimer", 300, pdFALSE, NULL, button_timer_callback);
    pinio.init();
    
    startAdv();  

}


void loop(void) {
    //vTaskDelay(ms2tick(1000));
    //digitalToggle(LCD_LIGHT_3);
    smartwatch.hardware_update();
}
