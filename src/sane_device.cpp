#include "sane.h"

namespace sanepp {
    DeviceInfo::DeviceInfo(const SANE_Device *device) : m_device(device) {}

    std::string_view DeviceInfo::name() const {
        if (m_device == nullptr) return "";
        return m_device->name;
    }

    std::string_view DeviceInfo::vendor() const {
        if (m_device == nullptr) return "";
        return m_device->vendor;
    }

    std::string_view DeviceInfo::model() const {
        if (m_device == nullptr) return "";
        return m_device->model;
    }

    std::string_view DeviceInfo::type() const {
        if (m_device == nullptr) return "";
        return m_device->type;
    }

    // also add mutex here for thread-safety
    std::optional<Device> DeviceInfo::open() const {
        if (m_device == nullptr) return std::optional<Device>{};

        return std::optional<Device>(Device(m_device->name));
    }

    // TODO do a different check (compare name, vendor, model, type)
    bool operator==(const DeviceInfo &lhs, const DeviceInfo &rhs) { return lhs.m_device == rhs.m_device; }

    bool operator!=(const DeviceInfo &lhs, const DeviceInfo &rhs) { return !(lhs == rhs); }

    Device::Device(const char *device_name) {
        m_device_status = sane_open(device_name, &m_device_handle);
        load_options();
    }

    Device::Device(Device &&device) : m_device_handle(device.m_device_handle), m_options(std::move(device.m_options)) {
        device.m_device_handle = nullptr;
    }

    Device::~Device() {
        if (m_device_handle) sane_close(m_device_handle);
    }

    Device &Device::operator=(Device &&device) {
        m_device_handle = device.m_device_handle;
        m_options = std::move(device.m_options);
        device.m_device_handle = nullptr;

        return *this;
    }

    const std::vector<Option> &Device::options() const { return m_options; }

    Device::operator bool() const { return m_device_status == SANE_STATUS_GOOD; }

    void Device::load_options() {
        if (m_device_status == SANE_STATUS_GOOD) {
            SANE_Status current_status;

            SANE_Int number_of_options = 0;
            SANE_Int option_id = 1;
            const SANE_Option_Descriptor *current_option = nullptr;

            current_status =
                sane_control_option(m_device_handle, 0, SANE_ACTION_GET_VALUE, &number_of_options, nullptr);

            if (current_status != SANE_STATUS_GOOD) return;

            for (SANE_Int i = 1; i < number_of_options; ++option_id) {
                if ((current_option = sane_get_option_descriptor(m_device_handle, option_id)) != nullptr) {
                    OptionInfo info(option_id);
                    if (current_option->name && SANE_OPTION_IS_ACTIVE(current_option->cap)) {
                        info.name(current_option->name).size(current_option->size);

                        if (current_option->title) {
                            info.title(current_option->title);
                        }
                        if (current_option->desc) {
                            info.description(current_option->desc);
                        }

                        switch (current_option->type) {
                            case SANE_TYPE_BOOL:
                                if (current_option->size == sizeof(SANE_Bool)) {
                                    m_options.emplace_back(m_device_handle, false, info);
                                }
                                break;
                            case SANE_TYPE_INT:
                                if (current_option->size == sizeof(SANE_Int)) {
                                    m_options.emplace_back(m_device_handle, 0, info);
                                }
                                break;
                            case SANE_TYPE_FIXED:
                                if (current_option->size == sizeof(SANE_Fixed)) {
                                    m_options.emplace_back(m_device_handle, 0.0f, info);
                                }
                                break;
                            case SANE_TYPE_STRING:
                                m_options.emplace_back(m_device_handle, std::string(""), info);
                                break;
                            case SANE_TYPE_BUTTON:
                                m_options.emplace_back(m_device_handle, Button{}, info);
                                break;
                            case SANE_TYPE_GROUP:
                                m_options.emplace_back(m_device_handle, Group{}, info);
                                break;
                            default:
                                break;
                        }
                    }
                    ++i;
                }
            }
        }
    }
}  // namespace sanepp
