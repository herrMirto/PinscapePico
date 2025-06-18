// Pinscape Pico - Device API includer
// Copyright 2024, 2025 Michael J Roberts / BSD-3-Clause license / NO WARRANTY
//
// Includes the device API files, and brings the classes into our global
// namespace.

#pragma once
#ifdef _WIN32
#include "../WinAPI/PinscapeVendorInterface.h"
#include "../WinAPI/FeedbackControllerInterface.h"
#include "../WinAPI/RP2BootLoaderInterface.h"
#else
#include "../HostAPI/PinscapePicoAPI.h"
#endif

// bring the main interfaces into our global namespace
using VendorInterfaceDesc = PinscapePico::VendorInterfaceDesc;
using FeedbackControllerInterface = PinscapePico::FeedbackControllerInterface;
using RP2BootDevice = PinscapePico::RP2BootDevice;
