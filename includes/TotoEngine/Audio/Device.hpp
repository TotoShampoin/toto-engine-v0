#pragma once

#include <AL/alc.h>
#include <format>
#include <map>
#include <stdexcept>
#include <string>

namespace TotoEngine {

namespace Audio {

class Device {
public:
    Device() {
        _device = alcOpenDevice(NULL);
        if(!_device) {
            throw std::runtime_error(std::format("ERROR::DEVICE::CANNOT_OPEN"));
        }
    }
    Device(const std::string& name) {
        _device = alcOpenDevice(name.c_str());
    }
    ~Device() {
        alcCloseDevice(_device);
    }

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    Device(Device&& other):
        _device(other._device)
    {
        other._device = 0;
    }
    Device&& operator=(Device&& other) {
        _device = other._device;
        other._device = 0;
        return std::move(*this);
    }

    ALCdevice* device() const { return _device; }
private:
    ALCdevice* _device;
};

class DeviceManager {
public:
    static Device& get(const std::string& name = "##DEFAULT") {
        auto& manager = instance();
        if(manager._devices.find(name) == manager._devices.end()) {
            throw std::runtime_error(std::format("ERROR::DEVICEMANAGER::NOT_FOUND ({})", name));
        }
        return manager._devices[name];
    }
    static Device& open(const std::string& name = "##DEFAULT") {
        auto& manager = instance();
        if(manager._devices.find(name) == manager._devices.end()) {
            if(name == "##DEFAULT") {
                manager._devices.emplace(name, Device());
            } else {
                manager._devices.emplace(name, Device(name));
            }
        }
        return get(name);
    }
    static void close(const std::string& name = "##DEFAULT") {
        auto& manager = instance();
        if(manager._devices.find(name) == manager._devices.end()) {
            return;
        }
        manager._devices.erase(name);
    }

    static DeviceManager& instance() {
        static auto manager = DeviceManager();
        return manager;
    }
    ~DeviceManager() = default;
    DeviceManager(const DeviceManager&) = delete;
    DeviceManager(const DeviceManager&&) = delete;
    DeviceManager& operator=(const DeviceManager&) = delete;
    DeviceManager&& operator=(const DeviceManager&&) = delete;
private:
    DeviceManager() = default;
    std::map<std::string, Device> _devices;
};

}

}
