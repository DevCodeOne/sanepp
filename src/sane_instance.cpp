#include <memory>

#include "sane_instance.h"


std::unique_ptr<sane_instance> sane_instance::_instance;

// Implementation of sane_instance
sane_instance::sane_instance(sane_authorization_callback callback) {
    SANE_Status sane_status = sane_init(&m_version_code, callback);

    if (sane_status == SANE_STATUS_GOOD)
        m_initialized = true;
}

// TODO add mutex for thread-safety this mutex is shared with instance
void sane_instance::create_instance(sane_authorization_callback callback) {
    if (_instance != nullptr)
        return;

    _instance = std::unique_ptr<sane_instance>(new sane_instance(callback));
}

// TODO add mutex
sane_instance *sane_instance::instance() {
    return _instance.get();
}

sane_instance::~sane_instance() {
    sane_exit();
}

// TODO Handle unsuccessfull call to sane_init
SANE_Int sane_instance::version_code() const {
    return m_version_code;
}

sane_device_info_list sane_instance::get_devices(bool local_devices_only) const {
    if (!m_initialized)
        return sane_device_info_list{};

    const SANE_Device **device_list;
    sane_get_devices(&device_list, local_devices_only);

    return sane_device_info_list(device_list);
}
