#include <memory>

#include "sane.h"

std::unique_ptr<sane> sane::_instance;

// Implementation of sane

// TODO Handle unsuccessfull call to sane_init
sane::sane(sane_authorization_callback callback) {
    SANE_Status sane_status = sane_init(&m_version_code, callback);

    if (sane_status == SANE_STATUS_GOOD)
        m_initialized = true;
}

// TODO add mutex for thread-safety this mutex is shared with instance
void sane::create_instance(sane_authorization_callback callback) {
    if (_instance != nullptr)
        return;

    _instance = std::unique_ptr<sane>(new sane(callback));
}

// TODO add mutex
const sane &sane::instance() {
    return *_instance.get();
}

sane::~sane() {
    sane_exit();
}

SANE_Int sane::version() const {
    return m_version_code;
}

sane::operator bool() const {
    return m_initialized;
}

std::vector<sane_device_info> sane::devices(bool local_devices_only) const {
    if (!m_initialized)
        return {};

    const SANE_Device **raw_device_info_list;
    const SANE_Device *current_device;
    size_t current_device_index = 0;
    std::vector<sane_device_info> device_info_list;

    sane_get_devices(&raw_device_info_list, local_devices_only);

    while ((current_device = raw_device_info_list[current_device_index]) != nullptr) {
        device_info_list.push_back(current_device);

        ++current_device_index;
    }

    return device_info_list;
}
