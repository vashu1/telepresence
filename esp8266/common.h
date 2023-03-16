
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
#define MAX_MOTOR_SPEED 100
#define MAX_REVERSE_SPEED 33
#define TURN_COEFF 0.5

// if motors are running and no cmd recieved for EMERGENCY_STOP_TIMEOUT_MS then stop
#define EMERGENCY_STOP_TIMEOUT_MS 333

#define NTP_SERVERS "pool.ntp.org", "time.nist.gov"
