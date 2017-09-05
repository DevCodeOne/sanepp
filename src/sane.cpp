#include <memory>

#include "sane.h"

std::function<void(SANE_String_Const, SANE_Char *, SANE_Char *)>
    sane::_callback = [](SANE_String_Const, SANE_Char *, SANE_Char *) { };

// Implementation of sane_version
sane_version::sane_version(SANE_Int version_code)
    : m_version_code(version_code) {
}

void sane_version::version_code(SANE_Int version_code) {
    m_version_code = version_code;
}

// Implementation of sane
// TODO Handle unsuccessfull call to sane_init
sane::sane(sane_authorization_callback callback) {
    SANE_Int version_code;
    SANE_Status sane_status = sane_init(&version_code, callback);

    m_version.version_code(version_code);

    if (sane_status == SANE_STATUS_GOOD)
        m_initialized = true;
}

const sane &sane::instance() {
    static sane instance(sane::callback_wrapper);

    return instance;
}

void sane::authorization_callback(const std::function<callback_type> &callback) {
    _callback = callback;
}

void sane::callback_wrapper(SANE_String_Const resource, SANE_Char *name, SANE_Char *password) {
    _callback(resource, name, password);
}

sane::~sane() {
    sane_exit();
}

const sane_version &sane::version() const {
    return m_version;
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
