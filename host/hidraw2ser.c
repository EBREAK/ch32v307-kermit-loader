#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <hidapi/hidapi.h>

hid_device *hid_handle;

void stdin_read(void)
{
	uint8_t buf[8];
	int ret;
	ret = read(STDIN_FILENO, buf, 7);
	if (ret == 0) {
		exit(EXIT_SUCCESS); // EOF
	}
	if (ret < 0) {
		perror("STDIN READ");
		exit(EXIT_FAILURE);
	}
	memmove(&buf[1], &buf[0], ret);
	buf[0] = ret;
	if (hid_write(hid_handle, buf, 8) < 0) {
		fprintf(stderr, "HID WRITE FAIL: %ls\n", hid_error(hid_handle));
		exit(EXIT_FAILURE);
	}
}

void hiddev_read(void)
{
	uint8_t buf[8];
	int ret;
	ret = hid_read_timeout(hid_handle, buf, 8, 1);
	if (ret < 0) {
		fprintf(stderr, "HID READ FAIL: %ls\n", hid_error(hid_handle));
		exit(EXIT_FAILURE);
	}
	if (ret == 0) {
		return;
	}
	int idx = 0;
	int len = buf[0];
	if (len > 7) {
		len = 7;
	}
	while (idx < len) {
		ret = write(STDOUT_FILENO, &buf[1 + idx], len - idx);
		if (ret < 0) {
			perror("STDOUT WRITE FAIL");
			exit(EXIT_FAILURE);
		}
		idx += ret;
	}
}

void main_loop(void)
{
	fd_set rfds;
	struct timeval tv;
	int ret;
	while (1) {
		hiddev_read();
		FD_ZERO(&rfds);
		FD_SET(STDIN_FILENO, &rfds);
		memset(&tv, 0, sizeof(tv));
		tv.tv_sec = 0;
		tv.tv_usec = 1 * 1000;
		ret = select(128, &rfds, NULL, NULL, &tv);
		if (ret == 0) {
			continue;
		}
		if (ret < 0) {
			perror("SELECT FAIL");
			exit(EXIT_FAILURE);
		}
		if (FD_ISSET(STDIN_FILENO, &rfds)) {
			stdin_read();
		}
	}
}

const char *hid_bus_type2name_lut[] = {
	/** Unknown bus type */
	[HID_API_BUS_UNKNOWN] = "UNKNOWN",
	[HID_API_BUS_USB] = "USB",
	[HID_API_BUS_BLUETOOTH] = "BLUETOOTH",
	[HID_API_BUS_I2C] = "I2C",
	[HID_API_BUS_SPI] = "SPI",
};

void exit_cb(void)
{
	if (hid_handle != 0) {
		hid_exit();
	}
}

int main(void)
{
	/* --- HIDAPI R&D: this is just to force the compiler to ensure
	   each of those functions are implemented (even as a stub)
	   by each backend. --- */
	(void)&hid_open;
	(void)&hid_open_path;
	(void)&hid_read_timeout;
	(void)&hid_get_input_report;
	(void)&hid_send_output_report;
	(void)&hid_get_feature_report;
	(void)&hid_send_feature_report;
	(void)&hid_get_report_descriptor;
	atexit(exit_cb);
	hid_handle = hid_open(0x1234, 0x5678, NULL);
	if (!hid_handle) {
		fprintf(stderr, "OPEN HID DEVICE FAIL\n");
		exit(EXIT_FAILURE);
	}
	struct hid_device_info *hiddev_info;
	hiddev_info = hid_get_device_info(hid_handle);
	if (hiddev_info == NULL) {
		fprintf(stderr, "GET HID DEVICE INFO FAIL: %ls\n",
			hid_error(hid_handle));
		exit(EXIT_FAILURE);
	}
	printf("MANUFACTURER: %ls\n", hiddev_info->manufacturer_string);
	printf("PRODUCT: %ls\n", hiddev_info->product_string);
	printf("VENDOR ID: 0X%04X\n", hiddev_info->vendor_id);
	printf("PRODUCT ID: 0X%04X\n", hiddev_info->product_id);
	printf("PATH: %s\n", hiddev_info->path);
	printf("BUS TYPE: %s\n",
	       hid_bus_type2name_lut[hiddev_info->bus_type %
				     (sizeof(hid_bus_type2name_lut) /
				      sizeof(hid_bus_type2name_lut[0]))]);
	hid_set_nonblocking(hid_handle, 1);
	main_loop();
	return 0;
}
