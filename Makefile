JNI_DIR := jni

SRC_FILES := $(JNI_DIR)/main.c

OUTPUT := fan_controller

INSTALL_PATH := /usr/local/bin
SERVICE_PATH := /etc/systemd/system
SERVICE_FILE := fan_controller.service

all: android linux

android:
	@echo "Building for Android..."
	ndk-build NDK_PROJECT_PATH=$(PWD) APP_BUILD_SCRIPT=$(JNI_DIR)/Android.mk

linux: $(SRC_FILES)
	@echo "Building for Linux..."
	$(CC) -o $(OUTPUT) $(SRC_FILES)

install: linux $(SERVICE_FILE)
	@echo "Installing fan_controller..."
	install -m 0755 $(OUTPUT) $(INSTALL_PATH)
	install -m 0644 $(SERVICE_FILE) $(SERVICE_PATH)
	systemctl daemon-reload
	systemctl enable $(SERVICE_FILE)
	systemctl start $(SERVICE_FILE)

uninstall:
	@echo "Uninstalling fan_controller..."
	systemctl stop $(SERVICE_FILE)
	systemctl disable $(SERVICE_FILE)
	rm -f $(INSTALL_PATH)/$(OUTPUT)
	rm -f $(SERVICE_PATH)/$(SERVICE_FILE)
	systemctl daemon-reload

clean:
	@echo "Cleaning up..."
	rm -f $(OUTPUT)
	rm -rf libs obj

.PHONY: all android linux install clean

