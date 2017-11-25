#pragma once

#include <optional>
#include <string>
#include <vector>

#include <sane/sane.h>

#include "sane_options.h"

namespace sanepp {
    class Device final {
       public:
        Device(const Device &) = delete;
        Device(Device &&);
        ~Device();

        Device &operator=(const Device &) = delete;
        Device &operator=(Device &&);

        const std::vector<Option> &options() const;
        std::optional<Option> find_option(const OptionInfo &info) const;

        explicit operator bool() const;

       private:
        Device(const std::string &device_name);
        void load_options();

        SANE_Handle m_device_handle;
        SANE_Status m_device_status;

        std::vector<Option> m_options;

        friend class DeviceInfo;
    };

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

    bool operator==(const DeviceInfo &lhs, const DeviceInfo &rhs);
    bool operator!=(const DeviceInfo &lhs, const DeviceInfo &rhs);
}  // namespace sanepp
