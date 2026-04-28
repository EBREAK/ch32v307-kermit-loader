/********************************** (C) COPYRIGHT *******************************
 * File Name          : usb_desc.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/20
 * Description        : usb device descriptor,configuration descriptor,
 *                      string descriptors and other descriptors.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "usb_desc.h"

/* Device Descriptor */
const uint8_t MyDevDescr[] = {
	0x12, // bLength
	0x01, // bDescriptorType (Device)
	0x00, 0x01, // bcdUSB 1.00
	0x03, // bDeviceClass
	0x00, // bDeviceSubClass
	0x00, // bDeviceProtocol
	0x08, // bMaxPacketSize0 8
	0x00, 0x00, // idVendor  0x0000
	0x00, 0x00, // idProduct 0x0000
	0x01, 0x00, // bcdDevice 0.01
	0x01, // iManufacturer (String Index)
	0x02, // iProduct (String Index)
	0x03, // iSerialNumber (String Index)
	0x01, // bNumConfigurations 1
};

/* Configuration Descriptor */
const uint8_t MyCfgDescr[] = {
	// ---------- 配置描述符 ----------
	0x09, // bLength: 9
	0x02, // bDescriptorType: CONFIGURATION
	0x29, 0x00, // wTotalLength: 41 (0x0029)
	0x01, // bNumInterfaces: 1
	0x01, // bConfigurationValue: 1
	0x00, // iConfiguration: 0 (无字符串)
	0x80, // bmAttributes: 总线供电，无远程唤醒
	0x32, // bMaxPower: 100mA (50 * 2mA)

	// ---------- 接口描述符 ----------
	0x09, // bLength: 9
	0x04, // bDescriptorType: INTERFACE
	0x00, // bInterfaceNumber: 0
	0x00, // bAlternateSetting: 0
	0x02, // bNumEndpoints: 2 (EP4 IN, EP1 OUT)
	0x03, // bInterfaceClass: HID
	0x00, // bInterfaceSubClass: 0 (无引导接口)
	0x00, // bInterfaceProtocol: 0
	0x00, // iInterface: 0

	// ---------- HID 描述符 ----------
	0x09, // bLength: 9
	0x21, // bDescriptorType: HID
	0x10, 0x01, // bcdHID: 1.11
	0x00, // bCountryCode: 未本地化
	0x01, // bNumDescriptors: 1
	0x22, // bDescriptorType: Report
	0x28, 0x00, // wDescriptorLength: 40 (0x0028) 字节的报告描述符

	// ---------- 端点描述符: EP4 IN ----------
	0x07, // bLength: 7
	0x05, // bDescriptorType: ENDPOINT
	0x84, // bEndpointAddress: 0x84 (IN, 端点4)
	0x03, // bmAttributes: 中断传输
	0x08, 0x00, // wMaxPacketSize: 8 字节
	0x01, // bInterval

	// ---------- 端点描述符: EP1 OUT ----------
	0x07, // bLength: 7
	0x05, // bDescriptorType: ENDPOINT
	0x01, // bEndpointAddress: 0x01 (OUT, 端点1)
	0x03, // bmAttributes: 中断传输
	0x08, 0x00, // wMaxPacketSize: 8 字节
	0x01, // bInterval
};

// HID 报告描述符（大小 40 字节）
const uint8_t MyHIDReportDesc[] = {
	0x06, 0x00, 0xFF, // USAGE_PAGE (Vendor Defined)
	0x09, 0x01, // USAGE (Vendor Usage 1)
	0xA1, 0x01, // COLLECTION (Application)

	0x09, 0x01, //   USAGE (Vendor Usage 1)
	0x15, 0x00, //   LOGICAL_MINIMUM (0)
	0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
	0x75, 0x08, //   REPORT_SIZE (8 bits)
	0x95, 0x08, //   REPORT_COUNT 8
	0x81, 0x02, //   INPUT (Data, Variable, Absolute)

	0xC0, // END_COLLECTION

	0x06, 0x00, 0xFF, // USAGE_PAGE (Vendor Defined)
	0x09, 0x02, // USAGE (Vendor Usage 2)
	0xA1, 0x01, // COLLECTION (Application)

	0x09, 0x02, //   USAGE (Vendor Usage 2)
	0x15, 0x00, //   LOGICAL_MINIMUM (0)
	0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
	0x75, 0x08, //   REPORT_SIZE (8 bits)
	0x95, 0x08, //   REPORT_COUNT
	0x91, 0x02, //   OUTPUT (Data, Variable, Absolute)

	0xC0 // END_COLLECTION
};

// clang-format off

/* Language Descriptor */
const uint8_t MyLangDescr[] = { 0x04, 0x03, 0x09, 0x04 };

/* Manufacturer Descriptor */
const uint8_t MyManuInfo[] = {
	0x0E, 0x03,
	'W', 0,
	'C', 0,
	'H', 0,
	'.',  0,
	'C', 0,
	'N', 0,
};

/* Product Information */
const uint8_t MyProdInfo[] = {
	46, 0x03,
	'C', 0,
	'H', 0,
	'3', 0,
	'2', 0,
	'V',  0,
	'3', 0,
	'0', 0,
	'7', 0,
	' ', 0,
	'K', 0,
	'E', 0,
	'R', 0,
	'M', 0,
	'I', 0,
	'T', 0,
	' ', 0,
	'L', 0,
	'O', 0,
	'A', 0,
	'D', 0,
	'E', 0,
	'R', 0,
};


/* Serial Number Information */
const uint8_t MySerNumInfo[] = {
	22, 0x03,
	'0', 0,
	'1', 0,
	'2', 0,
	'3',  0,
	'4', 0,
	'5', 0,
	'6', 0,
	'7',  0,
	'8', 0,
	'9', 0,
};
// clang-format on
