#include <cassert>
#include <cstring>

#include <memory>

#include "sane.h"

std::function<sane::callback_type>
    sane::_callback = [](const std::string &, std::string &, std::string &) { };
std::mutex sane::sane_instance_mutex;

// Implementation of sane_version
unsigned short sane_version::major_version(){
    return m_major_version;
}


sane_version::sane_version(unsigned short maj, unsigned short min, unsigned short bld):
        m_major_version(maj),
        m_minor_version(min),
        m_built_version(blt){
}

sane_version::sane_version(SANE_Int version_code)
        : m_major_version(SANE_VERSION_MAJOR(version_code)),
          m_minor_version(SANE_VERSION_MINOR(version_code)),
          m_built_version(SANE_VERSION_BUILT(version_code)){
}


bool operator<=(const sane_version &lhs, const sane_version &rhs){
   if (lhs.m_major_version<rhs.m_major_version)
       return true;
    if (lhs.m_major_version==rhs.m_major_version){
        if (lhs.m_minor_version<rhs.m_minor_version)
            return true;
        if (lhs.m_minor_version==rhs.m_minor_version){
            if (lhs.m_built_version<=rhs.m_built_version)
                return true;
        }
    }
    return false;
}

bool operator>(const sane_version &lhs, const sane_version &rhs){
    if (lhs<=rhs)
        return false;
    return true;
}

bool operator==(const sane_version &lhs, const sane_version &rhs){
    if (lhs.m_major_version==rhs.m_major_version&&
            lhs.m_minor_version==rhs.m_minor_version&&
            lhs.m_built_version==rhs.m_built_version)
        return true;
    return false;
}

bool operator!=(const sane_version &lhs, const sane_version &rhs){
    if (lhs==rhs)
        return false;
    return true;
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
    std::lock_guard<std::mutex>{sane_instance_mutex};
    static sane instance(sane::callback_wrapper);

    return instance;
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
