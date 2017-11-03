#include <cassert>
#include <cstring>

#include <memory>

#include "sane.h"

namespace sanepp {
    std::function<Sane::callback_type> Sane::_callback = [](const std::string &, std::string &, std::string &) {};
    std::mutex Sane::_instance_mutex;
    unsigned int Sane::_instance_count = 0;
    bool Sane::_initialized = false;
    Version Sane::_version;

    unsigned char Version::major_version() { return m_major_version; }

    Version::Version(unsigned char major_version, unsigned char minor_version, unsigned short build_version)
        : m_major_version(major_version), m_minor_version(minor_version), m_build_version(build_version) {}

    Version::Version(SANE_Int version_code)
        : m_major_version(SANE_VERSION_MAJOR(version_code)),
          m_minor_version(SANE_VERSION_MINOR(version_code)),
          m_build_version(SANE_VERSION_BUILD(version_code)) {}

    bool operator<=(const Version &lhs, const Version &rhs) { return !(lhs > rhs); }

    bool operator<(const Version &lhs, const Version &rhs) { return !(lhs >= rhs); }

    bool operator>=(const Version &lhs, const Version &rhs) { return lhs > rhs || lhs == rhs; }

    bool operator>(const Version &lhs, const Version &rhs) {
        if (lhs.m_major_version > rhs.m_major_version) return true;

        if (lhs.m_major_version < rhs.m_major_version) return false;

        if (lhs.m_minor_version > rhs.m_minor_version) return true;

        if (lhs.m_minor_version < rhs.m_minor_version) return false;

        if (lhs.m_build_version > rhs.m_build_version) return true;

        return false;
    }

    bool operator==(const Version &lhs, const Version &rhs) {
        return (lhs.m_major_version == rhs.m_major_version && lhs.m_minor_version == rhs.m_minor_version &&
                lhs.m_build_version == rhs.m_build_version);
    }

    bool operator!=(const Version &lhs, const Version &rhs) { return !(lhs == rhs); }

    Sane::Sane() {
        std::lock_guard<std::mutex> instance_guard{_instance_mutex};

        if (_instance_count == 0) {
            SANE_Int version_code;
            SANE_Status sane_status = sane_init(&version_code, Sane::callback_wrapper);

            _version = Version(version_code);

            if (sane_status == SANE_STATUS_GOOD) {
                _initialized = true;
            }
        }

        ++_instance_count;
    }

    Sane::Sane(const Sane &) : Sane() {}

    Sane::Sane(Sane &&) : Sane() {}

    void Sane::authorization_callback(const std::function<callback_type> &callback) {
        std::lock_guard<std::mutex> instance_guard{_instance_mutex};
        _callback = callback;
    }

    void Sane::callback_wrapper(SANE_String_Const resource, SANE_Char *name, SANE_Char *password) {
        std::string name_destination;
        std::string password_destination;

        _callback(std::string(resource), name_destination, password_destination);

        assert(name_destination.size() < SANE_MAX_USERNAME_LEN);
        assert(password_destination.size() < SANE_MAX_PASSWORD_LEN);

        strncpy(name, name_destination.c_str(), name_destination.size());
        strncpy(password, password_destination.c_str(), password_destination.size());
    }

    Sane::~Sane() {
        std::lock_guard<std::mutex> instance_guard{_instance_mutex};

        --_instance_count;
        if (_instance_count == 0) {
            sane_exit();
            _initialized = false;
        }
    }

    const Version &Sane::version() const { return _version; }

    Sane::operator bool() const { return _initialized; }

    std::vector<DeviceInfo> Sane::devices(bool local_devices_only) const {
        if (!_initialized) return {};

        const SANE_Device **raw_device_info_list;
        const SANE_Device *current_device;
        size_t current_device_index = 0;
        std::vector<DeviceInfo> device_info_list;

        sane_get_devices(&raw_device_info_list, local_devices_only);

        while ((current_device = raw_device_info_list[current_device_index]) != nullptr) {
            device_info_list.push_back(current_device);

            ++current_device_index;
        }

        return device_info_list;
    }
}  // namespace sanepp
