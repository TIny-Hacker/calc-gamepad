/**
 * --------------------------------------
 * 
 * TI-84+ CE Gamepad - main.c
 * By TIny_Hacker
 * Copyright 2024
 * License: GPL-3.0
 * Last Build: May 3, 2024
 * Version: 0.0.1
 * 
 * --------------------------------------
**/

#include <graphx.h>
#include <keypadc.h>
#include <usbdrvce.h>

#define DEFAULT_LANGID 0x0409

static const usb_string_descriptor_t product_name = {
    .bLength = sizeof(product_name) + 12, // 12 includes length of .bString
    .bDescriptorType = USB_STRING_DESCRIPTOR,
    .bString = L"gameCE",
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

static struct {
    usb_configuration_descriptor_t configuration;
    usb_interface_descriptor_t interface;
    struct usb_hid_descriptor_t {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t bcdHID;
        uint8_t bCountryCode;
        uint8_t bNumDescriptors;
        uint8_t bDescriptorType2;
        uint16_t wDescriptorLength;
    } hid;
    usb_endpoint_descriptor_t endpointIN;
    usb_endpoint_descriptor_t endpointOUT;
} configuration1 = {
    .configuration = {
        .bLength = sizeof(usb_configuration_descriptor_t),
        .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
        .wTotalLength = sizeof(configuration1),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = USB_BUS_POWERED | USB_NO_REMOTE_WAKEUP,
        .bMaxPower = 500 / 2,
    },
    .interface = {
        .bLength = sizeof(usb_interface_descriptor_t),
        .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 2,
        .bInterfaceClass = USB_HID_CLASS,
        .bInterfaceSubClass = 0x00, // No subclass
        .bInterfaceProtocol = 0x00,
        .iInterface = 0,
    },
    .hid = {
        .bLength = sizeof(struct usb_hid_descriptor_t),
        .bDescriptorType = 0x21, // HID
        .bcdHID = 0x0110,
        .bCountryCode = 0x00, // Not supported
        .bNumDescriptors = 1,
        .bDescriptorType2 = 0x22, // HID Report
        .wDescriptorLength = 119,
    },
    .endpointIN = {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = 0x81, // IN
        .bmAttributes = 0x03,
        .wMaxPacketSize = 0x40,
        .bInterval = 10,
    },
    .endpointOUT = {
        .bLength = sizeof(usb_endpoint_descriptor_t),
        .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
        .bEndpointAddress = 0x01, // OUT
        .bmAttributes = 0x03,
        .wMaxPacketSize = 0x40,
        .bInterval = 10,
    },
};

static const usb_configuration_descriptor_t *configurations[] = {
    &configuration1.configuration,
};

static const usb_device_descriptor_t device = {
    .bLength = sizeof(usb_device_descriptor_t),
    .bDescriptorType = USB_DEVICE_DESCRIPTOR,
    .bcdUSB = 0x0110,
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

static usb_error_t handleUsbEvent(usb_event_t event, void *eventData, usb_callback_data_t *callbackData) {
    usb_error_t error = USB_SUCCESS;

    if (event == USB_DEFAULT_SETUP_EVENT) {
        if (((usb_control_setup_t *)eventData)->bRequest == USB_GET_DESCRIPTOR_REQUEST &&
            ((usb_control_setup_t *)eventData)->wValue == 0x0200 &&
            ((usb_control_setup_t *)eventData)->wLength == sizeof(configuration1)) {

            usb_Transfer(usb_GetDeviceEndpoint(usb_FindDevice(NULL, NULL, USB_SKIP_HUBS), 0), &configuration1, sizeof(configuration1), 5, NULL);
            error = USB_IGNORE;
        }
    }

    return error;
}

int main(void) {
    usb_error_t error;

    if ((error = usb_Init(handleUsbEvent, NULL, &standard, USB_DEFAULT_INIT_FLAGS)) == USB_SUCCESS) {
        while (kb_AnyKey());
        while (!kb_IsDown(kb_KeyClear)) {
            kb_Scan();
            usb_HandleEvents();
        }
    }

    usb_Cleanup();

    gfx_Begin();
    gfx_PrintStringXY("Error code: ", 0, 0);
    gfx_PrintUInt(error, 2);

    while (kb_AnyKey());
    while (!kb_AnyKey());

    gfx_End();

    return 0;
}
