//Tracker SoM AT Pasthrough Firmware


SYSTEM_MODE(MANUAL)

//#define MODEM_DEBUG

static Ring_Buffer at_rx_buffer;
static Ring_Buffer at_tx_buffer;

// scanmode
//   parameter 1 - mode     : 0 = Automatic, 1 = GSM only, 3 = LTE only
//   parameter 2 - effect   : 0 = Take effect after UE reboots, 1 = Take effect immediately
static char cmd1[] = "AT+QCFG=\"nwscanmode\",1,1\r\n";

// iotopmode
//   parameter 1    : 0 = LTE CAT-M1 only, 1 = LTE CAT-NB1 only, 2 = LTE CAT-M1 and CAT-NB1
//   parameter 2    : 1 = Take effect immediately
static char cmd2[] = "AT+QCFG=\"iotopmode\",0,1\r\n";

void setup() {
    Serial.begin(115200);

    // Init GPIO for the Quectel power system
    pinMode(BGPWR, OUTPUT);
    pinMode(BGRST, OUTPUT);
    pinMode(BGVINT, INPUT);
    digitalWrite(BGPWR, LOW);
    digitalWrite(BGRST, LOW);
    HAL_Delay_Milliseconds(100);

#if defined(MODEM_DEBUG)
    // Read Quectel modem status
    if (!digitalRead(BGVINT)) {
        RGB.control(true);
        RGB.color(255, 0, 0); // Red
        delay(300);
        // Asset Tracker power initial status error
        while(1); // Do not do anything when modem initial status error
    }
#endif

    // Turn on Quectel modem
    digitalWrite(BGPWR, HIGH); 
    HAL_Delay_Milliseconds(500);
    digitalWrite(BGPWR, LOW);

    HAL_USART_Init(HAL_USART_SERIAL2, &at_rx_buffer, &at_tx_buffer);
    HAL_USART_BeginConfig(HAL_USART_SERIAL2, 115200, SERIAL_FLOW_CONTROL_RTS_CTS | SERIAL_8N1, 0);

    // Send scanmode command to the modem
    int i;
    delay(5000);
    for (i=0; i<strlen((const char*) cmd1); i++)
        HAL_USART_Write_Data(HAL_USART_SERIAL2, cmd1[i]);

    // Send iotopmode command to the modem
    delay(1000);
    for (i=0; i<strlen((const char*) cmd2); i++)
        HAL_USART_Write_Data(HAL_USART_SERIAL2, cmd2[i]);
}

void loop() {
    // Everythig from the modem will be forwarded to the USB
    while (HAL_USART_Available_Data(HAL_USART_SERIAL2)) {
        char ch = HAL_USART_Read_Data(HAL_USART_SERIAL2);
        Serial.write(ch);
    }
    
    // Everythig from the USB will be forwarded to the modem
    while (Serial.available()) {
        char ch = Serial.read();
        HAL_USART_Write_Data(HAL_USART_SERIAL2, ch);
    }
}
