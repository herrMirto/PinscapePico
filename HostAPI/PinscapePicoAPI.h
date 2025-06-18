#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

struct libusb_context;
struct libusb_device_handle;
struct hid_device;

namespace PinscapePico {

struct VendorInterfaceDesc {
    std::string path;     // bus:device path string
    int unitNumber = 0;   // enumeration index
};

class VendorInterface {
public:
    VendorInterface(libusb_context *ctx, libusb_device_handle *h) : ctx(ctx), handle(h) {}
    ~VendorInterface();

    static int EnumerateDevices(std::list<VendorInterfaceDesc>& list);
    static int Open(VendorInterface*& vi, int unit = 0);

private:
    libusb_context *ctx{nullptr};
    libusb_device_handle *handle{nullptr};
};

struct HIDDeviceDesc {
    std::string path;
};

class FeedbackControllerInterface {
public:
    FeedbackControllerInterface(hid_device *d) : dev(d) {}
    ~FeedbackControllerInterface();

    static int EnumerateDevices(std::list<HIDDeviceDesc>& list);
    static int Open(FeedbackControllerInterface*& ifc, const std::string& path);
    int Read(uint8_t *data, size_t len);
    int Write(const uint8_t *data, size_t len);

private:
    hid_device *dev{nullptr};
};

class RP2BootDevice {
public:
    static int Enumerate(std::list<std::string>& paths);
};

} // namespace PinscapePico
