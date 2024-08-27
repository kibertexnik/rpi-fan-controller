#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TEMP_PATH "/sys/devices/virtual/thermal/thermal_zone0/temp"
#define FAN_PATH "/sys/class/thermal/cooling_device1/cur_state"

typedef enum {
    OFF = 0,
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    FULL = 4
} FanSpeed;

int get_temp();
void set_fan_speed(FanSpeed speed);
void main_loop();

int main() {
    main_loop();
    return 0;
}

void main_loop() {
    time_t start = time(NULL);
    while (1) {
        int temp = get_temp();
        FanSpeed speed;

        if (temp > 70) {
            speed = FULL;
        } else if (temp > 60) {
            speed = HIGH;
        } else if (temp > 45) {
            speed = MEDIUM;
        } else if (temp > 35) {
            speed = LOW;
        } else {
            speed = OFF;
        }

        set_fan_speed(speed);
        printf("Temperature: %d, Speed: %d\n", temp, speed);
        sleep(2);
    }
}

int get_temp() {
    FILE *f = fopen(TEMP_PATH, "r");
    if (!f) {
        perror("Failed to open temp file");
        exit(EXIT_FAILURE);
    }

    char data[16];
    if (fgets(data, sizeof(data), f) == NULL) {
        perror("Failed to read temp file");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    fclose(f);
    return atoi(data) / 1000;
}

void set_fan_speed(FanSpeed speed) {
    FILE *f = fopen(FAN_PATH, "w");
    if (!f) {
        perror("Failed to open fan file");
        exit(EXIT_FAILURE);
    }

    if (fprintf(f, "%d", speed) < 0) {
        perror("Failed to write to fan file");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    fclose(f);
}
