#include "sane.h"

// Implementation of sane_device_info
sane_device_info::sane_device_info(const SANE_Device *device)
    : m_device(device) {
}

std::string_view sane_device_info::name() const {
    if (m_device == nullptr)
        return "";
    return m_device->name;
}

std::string_view sane_device_info::vendor() const {
    if (m_device == nullptr)
        return "";
    return m_device->vendor;
}

std::string_view sane_device_info::model() const {
    if (m_device == nullptr)
        return "";
    return m_device->model;
}

std::string_view sane_device_info::type() const {
    if (m_device == nullptr)
        return "";
    return m_device->type;
}

// also add mutex here for thread-safety
std::optional<sane_device> sane_device_info::open() const {
    // TODO return something else
    if (m_device == nullptr)
        return std::optional<sane_device>{};

    return std::optional<sane_device>(sane_device(m_device->name));
}

// TODO do a different check (compare name, vendor, model, type)
bool operator==(const sane_device_info &lhs, const sane_device_info &rhs) {
    return lhs.m_device == rhs.m_device;
}

bool operator!=(const sane_device_info &lhs, const sane_device_info &rhs) {
    return !(lhs == rhs);
}

// Implementation of sane_device
sane_device::sane_device(const char *device_name) {
    m_device_status = sane_open(device_name, &m_device_handle);
    load_options();
}

sane_device::sane_device(sane_device &&device)
    : m_device_handle(device.m_device_handle),
    m_options(std::move(device.m_options)) {
    device.m_device_handle = nullptr;
}

sane_device::~sane_device() {
    if (m_device_handle)
        sane_close(m_device_handle);
}

sane_device &sane_device::operator=(sane_device &&device) {
    m_device_handle = device.m_device_handle;
    m_options = std::move(device.m_options);
    device.m_device_handle = nullptr;

    return *this;
}


const std::vector<sane_option> &sane_device::options() const {
    return m_options;
}

sane_device::operator bool() const {
    return m_device_status == SANE_STATUS_GOOD;
}

void sane_device::load_options() {
    if (m_device_status == SANE_STATUS_GOOD) {
        SANE_Status current_status;

        SANE_Int number_of_options = 0;
        SANE_Int option_id = 1;
        const SANE_Option_Descriptor *current_option = nullptr;

        current_status = sane_control_option(m_device_handle,
                0, SANE_ACTION_GET_VALUE, &number_of_options, nullptr);

        if (current_status != SANE_STATUS_GOOD)
            return;

        for (SANE_Int i = 1; i < number_of_options; ++option_id) {
            if ((current_option = sane_get_option_descriptor(m_device_handle, option_id)) != nullptr) {

                sane_option_description description(option_id);
                if (current_option->name && SANE_OPTION_IS_ACTIVE(current_option->cap)) {
                    description.name(current_option->name);
                    if (current_option->title)
                        description.title(current_option->title);
                    if (current_option->desc)
                        description.description(current_option->desc);

                    switch(current_option->type) {
                        case SANE_TYPE_BOOL :
                            m_options.emplace_back(m_device_handle, sane_bool(), description);
                            break;
                        case SANE_TYPE_INT :
                            m_options.emplace_back(m_device_handle, sane_int(), description);
                            break;
                        case SANE_TYPE_FIXED :
                            m_options.emplace_back(m_device_handle, sane_fixed(), description);
                            break;
                        case SANE_TYPE_STRING :
                            m_options.emplace_back(m_device_handle, sane_string(), description);
                            break;
                        case SANE_TYPE_BUTTON :
                            m_options.emplace_back(m_device_handle, sane_button(), description);
                            break;
                        case SANE_TYPE_GROUP :
                            m_options.emplace_back(m_device_handle, sane_group(), description);
                            break;
                        default:
                            // Should never happen
                            break;
                    }
                }
                ++i;
            }
        }
    }
}
