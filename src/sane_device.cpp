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
std::experimental::optional<sane_device> sane_device_info::open_this_device() const {
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
    load_options();
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

void sane_device::load_options() {
    if (m_device_status == SANE_STATUS_GOOD) {
        SANE_Status current_status;

        SANE_Int number_of_options;
        SANE_Int option_id = 1;
        const SANE_Option_Descriptor *current_option = nullptr;

        current_status = sane_control_option(m_device_handle,
                0, SANE_ACTION_GET_VALUE, &number_of_options, nullptr);

        for (SANE_Int i = 1; i < number_of_options; ++option_id) {
            if ((current_option = sane_get_option_descriptor(m_device_handle,
                        option_id)) != nullptr) {
                std::unique_ptr<sane_option_value> option;

                // TODO maybe read values right here for the first time
                // so that the options have valid options or set
                // the values right after all the options are known
                switch(current_option->type) {
                    case SANE_TYPE_BOOL :
                        option = std::make_unique<sane_option_value_bool>(false);
                        break;
                    case SANE_TYPE_INT :
                        option = std::make_unique<sane_option_value_int>(0);
                        break;
                    case SANE_TYPE_FIXED :
                        option = std::make_unique<sane_option_value_fixed>(0);
                        break;
                    case SANE_TYPE_STRING :
                        option = std::make_unique<sane_option_value_string>(nullptr);
                        break;
                    case SANE_TYPE_BUTTON :
                        option = std::make_unique<sane_option_value_button>();
                        break;
                    case SANE_TYPE_GROUP :
                        option = std::make_unique<sane_option_value_group>();
                        break;
                    default:
                        // TODO Should never happen
                        break;
                }

                m_options[option_id] = std::move(option);

                ++i;

            }
        }
    }
}
