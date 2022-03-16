#include <user_interface.h>

#include <pcf8574_esp.h>

#include "settings.h"
#include "relays.h"
#include "mqtt.h"

#define SDA_GPIO 13
#define SCL_GPIO 14

#define I2C_LED_PANEL0_ADDRESS 0x27

#define NUMBER_OF_RELAYS 8
#define BOILER_RELAY 0

namespace relays
{
    long long oldMillis = 0;

    PCF857x i2c_relays(I2C_LED_PANEL0_ADDRESS, &Wire);
    os_timer_t BoilerTimer;

    bool boilerGotSwitchedOff = false;

    void SwitchRelay(const size_t relay, const bool isOn)
    {
        // Serial.printf("Switching relay #%u %s.\r\n", relay, isOn ? "on" : "off");
        relays[relay] = isOn;
        i2c_relays.write(relay, isOn ? 0 : 1);

        switch (relay)
        {
        case BOILER_RELAY:
            if (isOn)
            {
                mqtt::PublishData("RESULT/POWER0", "on", true);
                mqtt::PublishData("RESULT/POWER0/DURATION", ((String)settings::boilerDelay).c_str(), false);
                os_timer_arm(&BoilerTimer, settings::boilerDelay * 60 * 1000, true); //  start timer for auto switch off
            }
            else
            {
                mqtt::PublishData("RESULT/POWER0", "off", true);
                os_timer_disarm(&BoilerTimer);
            }
            break;

        default:
            break;
        }
    }

    void BoilerTimerCallback(void *pArg)
    {
        boilerGotSwitchedOff = true;
    }

    void ScanI2C()
    {
        byte error, address;
        int nDevices;

        Serial.println("\nScanning for I2C devices...");

        nDevices = 0;
        for (address = 1; address < 127; address++)
        {
            Wire.beginTransmission(address);
            error = Wire.endTransmission();

            if (error == 0)
            {
                Serial.print("Device ");
                Serial.print(nDevices);
                Serial.print(":\t0x");
                if (address < 16)
                    Serial.print("0");
                Serial.println(address, HEX);

                nDevices++;
            }
            else if (error == 4)
            {
                Serial.print("Unknow error at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.println(address, HEX);
            }
        }
        if (nDevices == 0)
            Serial.println("No I2C devices found.\n");
    }

    void setup()
    {
        Wire.begin(SDA_GPIO, SCL_GPIO);
        ScanI2C();

        for (size_t i = 0; i < NUMBER_OF_RELAYS; i++)
        {
            SwitchRelay(i, false);
        }

        os_timer_setfn(&BoilerTimer, BoilerTimerCallback, NULL);
    }

    void loop()
    {
        // if (millis() - oldMillis > 1000)
        // {
        //     relays[0] = !relays[0];
        //     SwitchRelay(0, relays[0]);
        //     oldMillis = millis();
        // }

        if (boilerGotSwitchedOff)
        {
            SwitchRelay(BOILER_RELAY, false);
            boilerGotSwitchedOff = false;
        }
    }
}
