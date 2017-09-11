#include <cstring>

#include <memory>

#include "sane.h"

std::function<sane::callback_type>
    sane::_callback = [](const std::string &, std::string &, std::string &) { };
std::mutex sane::sane_instance_mutex;

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
    std::lock_guard<std::mutex>(sane_instance_mutex);
    static sane instance(sane::callback_wrapper);

    return instance;
}

void sane::authorization_callback(const std::function<callback_type> &callback) {
    std::lock_guard<std::mutex>(sane_instance_mutex);
    _callback = callback;
}

void sane::callback_wrapper(SANE_String_Const resource, SANE_Char *name, SANE_Char *password) {
    std::string name_destination;
    std::string password_destination;

    _callback(std::string(resource), name_destination, password_destination);

    strncpy(name, name_destination.c_str(), std::min(name_destination.size(),
                (std::string::size_type) SANE_MAX_USERNAME_LEN));
    strncpy(password, password_destination.c_str(), std::min(password_destination.size(),
                (std::string::size_type) SANE_MAX_PASSWORD_LEN));
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
