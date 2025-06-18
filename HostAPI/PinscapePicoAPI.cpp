#include "PinscapePicoAPI.h"
#include <libusb.h>
#include "../OpenPinballDevice/OpenPinballDeviceLib/hidapi/hidapi.h"
#include <filesystem>
#include <fstream>

using namespace std;

namespace PinscapePico {

VendorInterface::~VendorInterface()
{
    if (handle)
        libusb_close(handle);
    if (ctx)
        libusb_exit(ctx);
}

int VendorInterface::EnumerateDevices(list<VendorInterfaceDesc>& list)
{
    list.clear();
    libusb_context *ctx = nullptr;
    if (libusb_init(&ctx) != 0)
        return -1;

    libusb_device **devs = nullptr;
    ssize_t cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        libusb_exit(ctx);
        return -1;
    }

    int idx = 0;
    for (ssize_t i = 0; i < cnt; ++i) {
        libusb_device *dev = devs[i];
        libusb_device_descriptor desc;
        if (libusb_get_device_descriptor(dev, &desc) == 0) {
            if (desc.idVendor == 0x1209 && desc.idProduct == 0xEAEB) {
                char path[32];
                snprintf(path, sizeof(path), "%03u:%03u", libusb_get_bus_number(dev), libusb_get_device_address(dev));
                list.push_back({ path, idx++ });
            }
        }
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
    return static_cast<int>(list.size());
}

int VendorInterface::Open(VendorInterface*& out, int unit)
{
    list<VendorInterfaceDesc> devices;
    if (EnumerateDevices(devices) <= unit)
        return -1;

    libusb_context *ctx = nullptr;
    if (libusb_init(&ctx) != 0)
        return -1;

    libusb_device **devs = nullptr;
    ssize_t cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        libusb_exit(ctx);
        return -1;
    }

    int idx = 0;
    libusb_device_handle *h = nullptr;
    for (ssize_t i = 0; i < cnt && !h; ++i) {
        libusb_device *dev = devs[i];
        libusb_device_descriptor desc;
        if (libusb_get_device_descriptor(dev, &desc) == 0) {
            if (desc.idVendor == 0x1209 && desc.idProduct == 0xEAEB) {
                if (idx == unit) {
                    if (libusb_open(dev, &h) != 0)
                        h = nullptr;
                }
                idx++;
            }
        }
    }

    libusb_free_device_list(devs, 1);
    if (!h) {
        libusb_exit(ctx);
        return -1;
    }

    out = new VendorInterface(ctx, h);
    return 0;
}

FeedbackControllerInterface::~FeedbackControllerInterface()
{
    if (dev)
        hid_close(dev);
    hid_exit();
}

int FeedbackControllerInterface::EnumerateDevices(list<HIDDeviceDesc>& list)
{
    list.clear();
    if (hid_init() != 0)
        return -1;
    struct hid_device_info *info = hid_enumerate(0x1209, 0xEAEB);
    for (auto *cur = info; cur; cur = cur->next) {
        list.push_back({ cur->path });
    }
    hid_free_enumeration(info);
    hid_exit();
    return static_cast<int>(list.size());
}

int FeedbackControllerInterface::Open(FeedbackControllerInterface*& ifc, const string& path)
{
    if (hid_init() != 0)
        return -1;
    hid_device *h = hid_open_path(path.c_str());
    if (!h) {
        hid_exit();
        return -1;
    }
    ifc = new FeedbackControllerInterface(h);
    return 0;
}

int FeedbackControllerInterface::Read(uint8_t *data, size_t len)
{
    if (!dev)
        return -1;
    return hid_read(dev, data, len);
}

int FeedbackControllerInterface::Write(const uint8_t *data, size_t len)
{
    if (!dev)
        return -1;
    return hid_write(dev, data, len);
}

int RP2BootDevice::Enumerate(list<string>& paths)
{
    paths.clear();
    vector<string> roots;
    const char *user = getenv("USER");
    if (user) {
        roots.push_back(string("/media/") + user);
        roots.push_back(string("/run/media/") + user);
    }
    roots.push_back("/media");
    roots.push_back("/Volumes");

    for (const auto &r : roots) {
        if (!filesystem::exists(r))
            continue;
        for (const auto &entry : filesystem::directory_iterator(r)) {
            if (!entry.is_directory())
                continue;
            auto info = entry.path() / "INFO_UF2.TXT";
            if (filesystem::exists(info))
                paths.push_back(entry.path());
        }
    }
    return static_cast<int>(paths.size());
}

} // namespace PinscapePico

