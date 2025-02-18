/**
 * --------------------------------------
 * 
 * TI-84+ CE Gamepad Studio Source Code - defines.h
 * By TIny_Hacker
 * Copyright 2024 - 2025
 * License: GPL-3.0
 * 
 * --------------------------------------
**/

#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Check a specified bit in a number.
 * 
 */
#define bit(n, bit) ((n) & (1U << (bit)))

/**
 * @brief Toggles a specified bit in a number.
 * 
 */
#define toggle(n, bit) (n ^= (1U << bit))

#define JOYSTICK_SENSITIVITY            8

#define DEFAULT_LANGID                  0x0409

#define USB_HID_REPORT_MAIN             0 << 2
#define USB_HID_REPORT_GLOBAL           1 << 2
#define USB_HID_REPORT_LOCAL            2 << 2

#define USB_HID_REPORT_USAGE            0x00 << 4
#define USB_HID_REPORT_USAGE_PAGE       0x00 << 4
#define USB_HID_REPORT_LOGICAL_MIN      0x01 << 4
#define USB_HID_REPORT_USAGE_MIN        0x01 << 4
#define USB_HID_REPORT_LOGICAL_MAX      0x02 << 4
#define USB_HID_REPORT_USAGE_MAX        0x02 << 4
#define USB_HID_REPORT_PHYSICAL_MIN     0x03 << 4
#define USB_HID_REPORT_PHYSICAL_MAX     0x04 << 4
#define USB_HID_REPORT_UNIT             0x06 << 4
#define USB_HID_REPORT_REPORT_SIZE      0x07 << 4
#define USB_HID_REPORT_INPUT            0x08 << 4
#define USB_HID_REPORT_REPORT_COUNT     0x09 << 4
#define USB_HID_REPORT_REPORT_OUTPUT    0x09 << 4
#define USB_HID_REPORT_COLLECTION       0x0A << 4
#define USB_HID_REPORT_FEATURE          0x0B << 4
#define USB_HID_REPORT_END_COLLECTION   0x0C << 4

enum {
    USB_HID_REPORT_COLLECTION_PHYSICAL,
    USB_HID_REPORT_COLLECTION_APPLICATION,
    USB_HID_REPORT_COLLECTION_LOGICAL,
    USB_HID_REPORT_COLLECTION_REPORT,
    USB_HID_REPORT_COLLECTION_NAMED_ARRAY,
    USB_HID_REPORT_COLLECTION_USAGE_SWITCH,
    USB_HID_REPORT_COLLECTION_USAGE_MODIFIER,
};

enum {
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8,
    BUTTON_9,
    BUTTON_10,
    BUTTON_11,
    BUTTON_12,
    BUTTON_13,
    BUTTON_14,
    BUTTON_15,
    BUTTON_16,
};

// Custom struct definitions

typedef struct usb_hid_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdHID;
    uint8_t bCountryCode;
    uint8_t bNumDescriptors;
    uint8_t bDescriptorType2;
    uint16_t wDescriptorLength;
} usb_hid_descriptor_t;

typedef struct usb_hid_report_item0 {
    uint8_t header;
} usb_hid_report_item0_t;

typedef struct usb_hid_report_item1 {
    uint8_t header;
    uint8_t item;
} usb_hid_report_item1_t;

typedef struct usb_hid_descriptor_report {
    usb_hid_report_item1_t usagePage;
    usb_hid_report_item1_t usage;
    struct collection1 {
        usb_hid_report_item1_t type;
        struct collection2 {
            usb_hid_report_item1_t type;
            usb_hid_report_item1_t usagePage1;
            usb_hid_report_item1_t usageMin1;
            usb_hid_report_item1_t usageMax1;
            usb_hid_report_item1_t logicalMin1;
            usb_hid_report_item1_t logicalMax1;
            usb_hid_report_item1_t reportSize1;
            usb_hid_report_item1_t reportCount1;
            usb_hid_report_item1_t input1;
            usb_hid_report_item1_t usagePage2;
            usb_hid_report_item1_t usage1;
            usb_hid_report_item1_t usage2;
            usb_hid_report_item1_t usage3;
            usb_hid_report_item1_t usage4;
            usb_hid_report_item1_t logicalMin2;
            usb_hid_report_item1_t logicalMax2;
            usb_hid_report_item1_t reportSize2;
            usb_hid_report_item1_t reportCount2;
            usb_hid_report_item1_t input2;
            usb_hid_report_item0_t endCollection;
        } collection2;
        usb_hid_report_item0_t endCollection;
    } collection1;
} usb_hid_descriptor_report_t;

typedef struct usb_hid_gamepad_data {
    uint16_t buttons;
    int8_t leftX;
    int8_t leftY;
    int8_t rightX;
    int8_t rightY;
} usb_hid_gamepad_data_t;

#ifdef __cplusplus
}
#endif

#endif