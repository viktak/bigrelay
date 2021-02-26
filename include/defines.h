#ifndef DEFINES_H
#define DEFINES_H

#define MQTT_CUSTOMER "viktak"
#define MQTT_PROJECT  "spiti"

#define HARDWARE_ID "NeoPixel & I2C"
#define HARDWARE_VERSION "1.0"
#define SOFTWARE_ID "I2C relay driver - water heater"

#define DEBUG_SPEED 921600

#define JSON_SETTINGS_SIZE (JSON_OBJECT_SIZE(11) + 270)
#define JSON_MQTT_COMMAND_SIZE 300

#define CONTROL_COMMAND_JSON_SIZE 200

#define CONNECTION_STATUS_LED_GPIO 0

#define SDA_GPIO 13
#define SCL_GPIO 14

#define WATER_RELAY 0

#define I2C_LED_PANEL0_ADDRESS 0x27


//  Default values
#define DEFAULT_BOILER_DELAY 20

#endif