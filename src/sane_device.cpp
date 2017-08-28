#include <iostream>

#include "sane_instance.h"

// Implementation of sane_device_info
sane_device_info::sane_device_info(const SANE_Device **device)
    : m_device(device) {
}

const char *sane_device_info::name() const {
    if (m_device == nullptr)
        return "";
    return (*m_device)->name;
}

const char *sane_device_info::vendor() const {
    if (m_device == nullptr)
        return "";
    return (*m_device)->vendor;
}

const char *sane_device_info::model() const {
    if (m_device == nullptr)
        return "";
    return (*m_device)->model;
}

const char *sane_device_info::type() const {
    if (m_device == nullptr)
        return "";
    return (*m_device)->type;
}

// TODO find better alternative
std::experimental::optional<sane_device> sane_device_info::open_device() const {
    // TODO return something else
    if (m_device == nullptr)
        return std::experimental::optional<sane_device>{};

    return std::experimental::optional<sane_device>(
            sane_device((*m_device)->name));
}

// TODO do a different check (compare name, vendor, model, type)
bool operator==(const sane_device_info &lhs,
        const sane_device_info &rhs) {
    return lhs.m_device == rhs.m_device;
}

bool operator!=(const sane_device_info &lhs,
        const sane_device_info &rhs) {
    return !(lhs == rhs);
}

// Implementation of sane_device
sane_device::sane_device(const char *device_name) {
    m_device_status = sane_open(device_name, &m_device_handle);
}

sane_device::sane_device(sane_device &&device)
    : m_device_handle(device.m_device_handle) {
    device.m_device_handle = nullptr;
}

sane_device::~sane_device() {
    if (m_device_handle)
        sane_close(m_device_handle);
}

sane_device &sane_device::operator=(sane_device &&device) {
    m_device_handle = device.m_device_handle;
    device.m_device_handle = nullptr;

    return *this;
}

sane_device::operator bool() const {
    return m_device_status == SANE_STATUS_GOOD;
}
