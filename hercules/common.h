
#define AWS_IOT_PUBLISH_TOPIC   "dev-robot-output"
#define AWS_IOT_SUBSCRIBE_TOPIC "dev-robot-input"

#define I2C_ESP8266_ADDRESS  1
#define I2C_HERCULES_ADDRESS 2

#define CMD_STR_MOVE   "move"
#define CMD_INT_MOVE   1
#define CMD_STR_CAMERA "camera"
#define CMD_INT_CAMERA 2

#define JSON_MAX_SIZE 1024

// 0 - 100
#define MAX_MOTOR_SPEED 70
#define MAX_REVERSE_SPEED 0
#define TURN_COEFF 0.5

// looks like my Hercules-Dual-15A-6-20V-Motor-Controller has one side reversed
#define REVERSE_RIGHT 1
#define REVERSE_LEFT  0

// if motors are running and no cmd recieved for EMERGENCY_STOP_TIMEOUT_MS then stop
#define EMERGENCY_STOP_TIMEOUT_MS 333

#define NTP_SERVERS "pool.ntp.org", "time.nist.gov"

// 300, 600, 1200, 2400, 4800, 9600, 14400, 19200
#define SERIAL_SPEED 2400
#define SERIAL_PACKET_START 127

#define VOLTMETER_PIN A2
#define AMMEMETER_PIN A3
