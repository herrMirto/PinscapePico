# Pinscape Pico Host API

This directory contains a cross-platform variant of the device host API.  It can
be built on Linux and macOS using **libusb** and **hidapi**.  The code mirrors
the Windows API where possible and now provides a minimal working
implementation to enumerate devices and open connections without relying on the
Windows USB stack.
