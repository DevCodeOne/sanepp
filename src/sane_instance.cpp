#include <memory>

#include "sane_instance.h"

static std::unique_ptr<sane_instance> instance;

// Implementation of sane_instance
sane_instance::sane_instance(authorization_callback callback) {
    SANE_Status sane_status = sane_init(&m_version_code, callback);

    if (sane_status == SANE_STATUS_GOOD)
        m_initialized = true;
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

void create_sane_instance(authorization_callback callback) {
    if (instance != nullptr)
        return;

    instance = std::unique_ptr<sane_instance>(new sane_instance(callback));
}

sane_instance *get_sane_instance() {
    return instance.get();
}

// Implementation of sane_device_info_list
sane_device_info_list::sane_device_info_list() : m_device_list(nullptr) {
}

sane_device_info_list::sane_device_info_list(const SANE_Device **device_list)
    : m_device_list(device_list) {
}

sane_device_info_list_iterator sane_device_info_list::begin() {
    return sane_device_info_list_iterator(m_device_list);
}

sane_device_info_list_iterator sane_device_info_list::end() {
    auto device_list_ptr = m_device_list;
    while (*device_list_ptr != nullptr) {
        ++device_list_ptr;
    }

    return sane_device_info_list_iterator(device_list_ptr);
}

// Implementation of sane_device_info_list_iterator
sane_device_info_list_iterator::sane_device_info_list_iterator()
    : m_device_info(nullptr) {
}

sane_device_info_list_iterator::sane_device_info_list_iterator(
        const SANE_Device **device_ptr) : m_device_info(device_ptr) {
}

sane_device_info_list_iterator &sane_device_info_list_iterator::operator=(
        sane_device_info_list_iterator iterator) {
    swap(iterator);

    return *this;
}

sane_device_info_list_iterator &sane_device_info_list_iterator::operator++() {
    ++m_device_info.m_device;

    return *this;
}

sane_device_info_list_iterator sane_device_info_list_iterator::operator++(int) {
    auto tmp(*this);

    ++*this;

    return tmp;
}

sane_device_info &sane_device_info_list_iterator::operator*() {
    return m_device_info;
}

sane_device_info *sane_device_info_list_iterator::operator->() {
    return &m_device_info;
}

const sane_device_info &sane_device_info_list_iterator::operator*() const {
    return m_device_info;
}

const sane_device_info *sane_device_info_list_iterator::operator->() const {
    return &m_device_info;
}

void sane_device_info_list_iterator::swap(sane_device_info_list_iterator &iterator) {
    using std::swap;

    swap(m_device_info, iterator.m_device_info);
}

bool operator==(const sane_device_info_list_iterator &lhs,
        const sane_device_info_list_iterator &rhs) {
    return *lhs == *rhs;
}

bool operator!=(const sane_device_info_list_iterator &lhs,
        const sane_device_info_list_iterator &rhs) {
    return !(lhs == rhs);
}
