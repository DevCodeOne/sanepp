#include <cassert>
#include <cstring>

#include <memory>

#include "sane.h"

std::function<sane::callback_type>
sane::_callback = [](const std::string &, std::string &, std::string &) { };
std::mutex sane::sane_instance_mutex;
unsigned int sane::_instance_count = 0;
bool sane::_initialized = false;
sane_version sane::_version;

// Implementation of sane_version
unsigned char sane_version::major_version() {
    return m_major_version;
}


sane_version::sane_version(unsigned char major_version, unsigned char minor_version,
        unsigned short build_version):
    m_major_version(major_version),
    m_minor_version(minor_version),
    m_build_version(build_version) {
}

sane_version::sane_version(SANE_Int version_code)
    : m_major_version(SANE_VERSION_MAJOR(version_code)),
    m_minor_version(SANE_VERSION_MINOR(version_code)),
    m_build_version(SANE_VERSION_BUILD(version_code)) {
}

bool operator<=(const sane_version &lhs, const sane_version &rhs) {
    return !(lhs > rhs);
}

bool operator<(const sane_version &lhs, const sane_version &rhs) {
    return !(lhs >= rhs);
}

bool operator>=(const sane_version &lhs, const sane_version &rhs) {
    return lhs > rhs || lhs == rhs;
}

bool operator>(const sane_version &lhs, const sane_version &rhs) {
    if (lhs.m_major_version > rhs.m_major_version)
        return true;

    if (lhs.m_major_version < rhs.m_major_version)
        return false;

    if (lhs.m_minor_version > rhs.m_minor_version)
        return true;

    if (lhs.m_minor_version < rhs.m_minor_version)
        return false;

    if (lhs.m_build_version > rhs.m_build_version)
        return true;

    return false;
}

bool operator==(const sane_version &lhs, const sane_version &rhs) {
    return (lhs.m_major_version == rhs.m_major_version &&
            lhs.m_minor_version == rhs.m_minor_version &&
            lhs.m_build_version == rhs.m_build_version);
}

bool operator!=(const sane_version &lhs, const sane_version &rhs) {
    return !(lhs == rhs);
}

// Implementation of sane
// TODO Handle unsuccessfull call to sane_init
sane::sane() {
    std::lock_guard<std::mutex>{sane_instance_mutex};

    if (_instance_count == 0) {
        SANE_Int version_code;
        SANE_Status sane_status = sane_init(&version_code, sane::callback_wrapper);

        _version = sane_version(version_code);

        if (sane_status == SANE_STATUS_GOOD)
            _initialized = true;

    }

    ++_instance_count;
}

void sane::authorization_callback(const std::function<callback_type> &callback) {
    std::lock_guard<std::mutex>{sane_instance_mutex};
    _callback = callback;
}

void sane::callback_wrapper(SANE_String_Const resource, SANE_Char *name, SANE_Char *password) {
    std::string name_destination;
    std::string password_destination;

    _callback(std::string(resource), name_destination, password_destination);

    assert(name_destination.size() < SANE_MAX_USERNAME_LEN);
    assert(password_destination.size() < SANE_MAX_PASSWORD_LEN);

    strncpy(name, name_destination.c_str(), name_destination.size());
    strncpy(password, password_destination.c_str(), password_destination.size());
}

sane::~sane() {
    std::lock_guard<std::mutex>{sane_instance_mutex};

    --_instance_count;
    if (_instance_count == 0) {
        sane_exit();
        _initialized = false;
    }
}

const sane_version &sane::version() const {
    return _version;
}

sane::operator bool() const {
    return _initialized;
}

std::vector<sane_device_info> sane::devices(bool local_devices_only) const {
    if (!_initialized)
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
