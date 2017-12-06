#pragma once

#include <optional>
#include <string>
#include <vector>

#include <sane/sane.h>

#include "sane_options.h"

namespace sanepp {
    class Device;

    class DeviceInfo final {
       public:
        const std::string &name() const;
        const std::string &vendor() const;
        const std::string &model() const;
        const std::string &type() const;
        std::optional<Device> open() const;

       private:
        DeviceInfo(const SANE_Device *device);

        std::string m_name = "";
        std::string m_vendor = "";
        std::string m_model = "";
        std::string m_type = "";

        friend class Sane;
    };

    class Device final {
       public:
        Device(const Device &other) = delete;
        Device(Device &&other);
        ~Device();

        Device &operator=(const Device &other) = delete;
        Device &operator=(Device &&other);

        const std::vector<Option> &options() const;
        const DeviceInfo &info() const;
        std::optional<Option> find_option(const OptionInfo &info) const;

        explicit operator bool() const;

       private:
        Device(const DeviceInfo &info);
        void load_options();

        SANE_Handle m_device_handle;
        SANE_Status m_device_status;
        DeviceInfo m_device_info;

        std::vector<Option> m_options;

        friend class DeviceInfo;
    };

    bool operator==(const DeviceInfo &lhs, const DeviceInfo &rhs);
    bool operator!=(const DeviceInfo &lhs, const DeviceInfo &rhs);
}  // namespace sanepp
