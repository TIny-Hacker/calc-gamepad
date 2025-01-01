/**
 * --------------------------------------
 * 
 * TI-84+ CE Gamepad - main.c
 * By TIny_Hacker
 * Copyright 2024 - 2025
 * License: GPL-3.0
 * Last Build: January 1, 2025
 * Version: 0.0.1
 * 
 * --------------------------------------
**/

#include "defines.h"

#include <graphx.h>
#include <keypadc.h>
#include <usbdrvce.h>

static const usb_string_descriptor_t product_name = {
    .bLength = sizeof(product_name) + 18, // 18 includes length of .bString
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"CEGamepad",
};

static const usb_string_descriptor_t manufacturer = {
    .bLength = sizeof(manufacturer) + 22,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"TIny_Hacker",
};

static const usb_string_descriptor_t *strings[] = { &product_name, &manufacturer };

static const usb_string_descriptor_t langids = {
    // .bLength = sizeof(langids),
    .bLength = 4,
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = {
        [0] = DEFAULT_LANGID,
    },
};

static usb_hid_descriptor_report_t report = {
    .usagePage = {
        .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_USAGE_PAGE,
        .item = 0x01, // Generic Desktop Controls
    },
    .usage = {
        .header = sizeof(uint8_t) | USB_HID_REPORT_LOCAL | USB_HID_REPORT_USAGE,
        // .item = 0x04, // Joystick
        .item = 0x05, // Gamepad
    },
    .collection1 = {
        .type = {
            .header = sizeof(uint8_t) | USB_HID_REPORT_MAIN | USB_HID_REPORT_COLLECTION,
            .item = USB_HID_REPORT_COLLECTION_APPLICATION,
        },
        .collection2 = {
            .type = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_MAIN | USB_HID_REPORT_COLLECTION,
                .item = USB_HID_REPORT_COLLECTION_PHYSICAL,
            },
            .usagePage1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_USAGE_PAGE,
                .item = 0x09, // Button
            },
            .usageMin1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_LOCAL | USB_HID_REPORT_USAGE_MIN,
                .item = 0x01, // Button 1
            },
            .usageMax1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_LOCAL | USB_HID_REPORT_USAGE_MAX,
                .item = 0x10, // Button 16
            },
            .logicalMin1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_LOGICAL_MIN,
                .item = 0x00,
            },
            .logicalMax1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_LOGICAL_MAX,
                .item = 0x01,
            },
            .reportSize1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_REPORT_SIZE,
                .item = 0x01,
            },
            .reportCount1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_REPORT_COUNT,
                .item = 0x10,
            },
            .input1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_MAIN | USB_HID_REPORT_INPUT,
                .item = 0x02, // Data, Var, Abs
            },
            .usagePage2 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_USAGE_PAGE,
                .item = 0x01, // Generic Desktop Controls
            },
            .usage1 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_LOCAL | USB_HID_REPORT_USAGE,
                .item = 0x30, // X
            },
            .usage2 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_LOCAL | USB_HID_REPORT_USAGE,
                .item = 0x31, // Y
            },
            .usage3 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_LOCAL | USB_HID_REPORT_USAGE,
                .item = 0x32, // Z
            },
            .usage4 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_LOCAL | USB_HID_REPORT_USAGE,
                .item = 0x33, // Rz
            },
            .logicalMin2 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_LOGICAL_MIN,
                .item = 0x81, // -127
            },
            .logicalMax2 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_LOGICAL_MAX,
                .item = 0x7F, // 127
            },
            .reportSize2 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_REPORT_SIZE,
                .item = 0x08,
            },
            .reportCount2 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_GLOBAL | USB_HID_REPORT_REPORT_COUNT,
                .item = 0x04,
            },
            .input2 = {
                .header = sizeof(uint8_t) | USB_HID_REPORT_MAIN | USB_HID_REPORT_INPUT,
                .item = 0x02, // Data, Var, Abs
            },
            .endCollection = {
                .header = USB_HID_REPORT_END_COLLECTION,
            },
        },
        .endCollection = {
            .header = USB_HID_REPORT_END_COLLECTION,
        },
    },
};

static struct {
    usb_configuration_descriptor_t configuration;
    usb_interface_descriptor_t interface;
    usb_hid_descriptor_t hid;
    usb_endpoint_descriptor_t endpoint;
} configuration1 = {
    .configuration = {
        .bLength = sizeof(usb_configuration_descriptor_t),
        .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
        .wTotalLength = sizeof(configuration1),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes =  (1 << 7) | USB_BUS_POWERED | USB_NO_REMOTE_WAKEUP,
        .bMaxPower = 500 / 2,
    },
    .interface = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 1,
        .bInterfaceClass = USB_HID_CLASS,
        .bInterfaceSubClass = 0x00, // No subclass
        .bInterfaceProtocol = 0x00,
        .iInterface = 0,
    },
    .hid = {
        .bLength = sizeof(usb_hid_descriptor_t),
        .bDescriptorType = 0x21, // HID
        .bcdHID = 0x0110,
        .bCountryCode = 0x00, // Not supported
        .bNumDescriptors = 1,
        .bDescriptorType2 = 0x22, // HID Report
        .wDescriptorLength = sizeof(report),
    },
    .endpoint = {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = USB_DEVICE_TO_HOST | 1, // OUT
        .bmAttributes = USB_INTERRUPT_TRANSFER,
        .wMaxPacketSize = 0x0008,
        .bInterval = 10,
    },
};

static const usb_configuration_descriptor_t *configurations[] = {
    &configuration1.configuration,
};

static const usb_device_descriptor_t device = {
    .bLength = sizeof(usb_device_descriptor_t),
    .bDescriptorType = USB_DEVICE_DESCRIPTOR,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00, // Device
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 0x40,
    .idVendor = 0x0451, // Texas Instruments
    .idProduct = 0xBEEF,
    .bcdDevice = 0x0100,
    .iManufacturer = 2,
    .iProduct = 1,
    .iSerialNumber = 0,
    .bNumConfigurations = sizeof(configurations) / sizeof(*configurations),
};

static const usb_standard_descriptors_t standard = {
    .device = &device,
    .configurations = configurations,
    .langids = &langids,
    .numStrings = sizeof(strings) / sizeof(*strings),
    .strings = strings,
};

static usb_hid_gamepad_data_t state = {
    .leftX = 0,
    .leftY = 0,
    .rightX = 0,
    .rightY = 0,
    .buttons = 0,
};

static usb_error_t handleUsbEvent(usb_event_t event, void *eventData, usb_callback_data_t *callbackData) {
    usb_error_t error = USB_SUCCESS;

    usb_device_t activeDevice = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);

    if (event == USB_DEFAULT_SETUP_EVENT) {
        if (((usb_control_setup_t *)eventData)->bmRequestType == USB_DEVICE_TO_HOST &&
            ((usb_control_setup_t *)eventData)->bRequest == USB_GET_DESCRIPTOR_REQUEST &&
            ((usb_control_setup_t *)eventData)->wValue == 0x0200 &&
            ((usb_control_setup_t *)eventData)->wLength == sizeof(configuration1)) {

            usb_Transfer(usb_GetDeviceEndpoint(activeDevice, 0), &configuration1, sizeof(configuration1), 5, NULL);
            error = USB_IGNORE;
        } else if (((usb_control_setup_t *)eventData)->bmRequestType == (USB_DEVICE_TO_HOST | USB_RECIPIENT_INTERFACE) &&
            ((usb_control_setup_t *)eventData)->bRequest == USB_GET_DESCRIPTOR_REQUEST &&
            ((usb_control_setup_t *)eventData)->wValue == 0x2200 &&
            ((usb_control_setup_t *)eventData)->wLength == sizeof(report) + 0x40) {

            usb_Transfer(usb_GetDeviceEndpoint(activeDevice, 0), &report, sizeof(report), 5, NULL);
            error = USB_IGNORE;
        }
    }

    return error;
}

static bool updateJoystickAxis(kb_lkey_t dec, kb_lkey_t inc, int8_t *value) {
    if (kb_IsDown(dec)) {
        if (*value > -127 + JOYSTICK_SENSITIVITY) {
            *value -= JOYSTICK_SENSITIVITY;
            return true;
        }
    } else if (kb_IsDown(inc)) {
        if (*value < 127 - JOYSTICK_SENSITIVITY) {
            *value += JOYSTICK_SENSITIVITY;
            return true;
        }
    } else if (*value) {
        *value = 0;
        return true;
    }

    return false;
}

static bool updateButton(kb_lkey_t key, uint8_t button) {
    if (kb_IsDown(key) && !bit(state.buttons, button)) {
        toggle(state.buttons, button);
        return true;
    } else if (!kb_IsDown(key) && bit(state.buttons, button)) {
        toggle(state.buttons, button);
        return true;
    }

    return false;
}

int main(void) {
    usb_error_t error;
    bool needUpdate = false;

    gfx_Begin();
    gfx_ZeroScreen();

    if ((error = usb_Init(handleUsbEvent, NULL, &standard, USB_DEFAULT_INIT_FLAGS)) == USB_SUCCESS) {
        while (kb_AnyKey());
        while (!kb_IsDown(kb_KeyClear)) {
            kb_Scan();
            usb_HandleEvents();

            if (needUpdate && USB_SUCCESS == usb_ScheduleInterruptTransfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), USB_DEVICE_TO_HOST | 1), &state, sizeof(state), NULL, NULL)) {
                needUpdate = false;
            } else {
                // Left joystick
                needUpdate |= updateJoystickAxis(kb_Key4, kb_Key6, &(state.leftX));
                needUpdate |= updateJoystickAxis(kb_Key8, kb_Key2, &(state.leftY));

                // Right joystick
                needUpdate |= updateJoystickAxis(kb_KeyLeft, kb_KeyRight, &(state.rightX));
                needUpdate |= updateJoystickAxis(kb_KeyUp, kb_KeyDown, &(state.rightY));

                // Standard(?) buttons
                needUpdate |= updateButton(kb_Key2nd, BUTTON_1);
                needUpdate |= updateButton(kb_KeyAlpha, BUTTON_2);
                needUpdate |= updateButton(kb_KeyGraphVar, BUTTON_3);
                needUpdate |= updateButton(kb_KeyMode, BUTTON_4);

                // Trigger buttons
                needUpdate |= updateButton(kb_KeyYequ, BUTTON_5);
                needUpdate |= updateButton(kb_KeyWindow, BUTTON_7);
                needUpdate |= updateButton(kb_KeyGraph, BUTTON_6);
                needUpdate |= updateButton(kb_KeyTrace, BUTTON_8);

                // Other
                needUpdate |= updateButton(kb_KeyDel, BUTTON_9);
                needUpdate |= updateButton(kb_KeyStat, BUTTON_10);

                // Joystick buttons
                needUpdate |= updateButton(kb_Key5, BUTTON_11);
                needUpdate |= updateButton(kb_KeyVars, BUTTON_12);

                // Hat switch
                needUpdate |= updateButton(kb_KeyCos, BUTTON_13);
                needUpdate |= updateButton(kb_KeyLParen, BUTTON_14);
                needUpdate |= updateButton(kb_KeyComma, BUTTON_15);
                needUpdate |= updateButton(kb_KeyRParen, BUTTON_16);
            }
        }
    }

    usb_Cleanup();
    gfx_End();

    return 0;
}
