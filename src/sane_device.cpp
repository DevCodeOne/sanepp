#include <iostream>

#include "sane.h"

// Implementation of sane_device_info
sane_device_info::sane_device_info(const SANE_Device *device)
    : m_device(device) {
}

const char *sane_device_info::name() const {
    if (m_device == nullptr)
        return "";
    return m_device->name;
}

const char *sane_device_info::vendor() const {
    if (m_device == nullptr)
        return "";
    return m_device->vendor;
}

const char *sane_device_info::model() const {
    if (m_device == nullptr)
        return "";
    return m_device->model;
}

const char *sane_device_info::type() const {
    if (m_device == nullptr)
        return "";
    return m_device->type;
}

// TODO find better alternative for optional
// also add mutex here for thread-safety
std::experimental::optional<sane_device> sane_device_info::open() const {
    // TODO return something else
    if (m_device == nullptr)
        return std::experimental::optional<sane_device>{};

    return std::experimental::optional<sane_device>(sane_device(m_device->name));
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


const std::vector<std::unique_ptr<option>> &sane_device::options() const {
    return m_options;
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

        if (current_status != SANE_STATUS_GOOD)
            return;

        for (SANE_Int i = 1; i < number_of_options; ++option_id) {
            if ((current_option = sane_get_option_descriptor(m_device_handle,
                        option_id)) != nullptr) {
                std::unique_ptr<sane_option_value> option_inst;

                // TODO maybe add helper function that will automaticly create
                // the correct object for a specific type
                switch(current_option->type) {
                    case SANE_TYPE_BOOL :
                        option_inst = std::make_unique<sane_option_value_bool>(false);
                        break;
                    case SANE_TYPE_INT :
                        option_inst = std::make_unique<sane_option_value_int>(0);
                        break;
                    case SANE_TYPE_FIXED :
                        option_inst = std::make_unique<sane_option_value_fixed>(0);
                        break;
                    case SANE_TYPE_STRING :
                        option_inst = std::make_unique<sane_option_value_string>(nullptr);
                        break;
                    case SANE_TYPE_BUTTON :
                        option_inst = std::make_unique<sane_option_value_button>();
                        break;
                    case SANE_TYPE_GROUP :
                        option_inst = std::make_unique<sane_option_value_group>();
                        break;
                    default:
                        // TODO Should never happen
                        break;
                }

                option_description description(option_id);

                if (current_option->name != nullptr)
                    description.name(current_option->name);

                if (current_option->title != nullptr)
                    description.name(current_option->title);

                if (current_option->desc != nullptr)
                    description.description(current_option->desc);

                m_options.emplace_back(m_device_handle, option_inst.get(), description);

                ++i;
            }
        }
    }
}
