#include "sane_options.h"

namespace sanepp {

    Fixed::Fixed(SANE_Fixed value) : m_value(value) {}

    Fixed &Fixed::operator=(SANE_Fixed value) {
        m_value = value;
        return *this;
    }

    SANE_Fixed Fixed::value() const { return m_value; }

    OptionInfo::OptionInfo(SANE_Int id) : m_id(id) {}

    OptionInfo &OptionInfo::name(const std::string &name) {
        m_name = name;

        return *this;
    }

    OptionInfo &OptionInfo::title(const std::string &title) {
        m_title = title;

        return *this;
    }

    OptionInfo &OptionInfo::description(const std::string &desc) {
        m_description = desc;

        return *this;
    }

    OptionInfo &OptionInfo::size(SANE_Int size) {
        m_size = size;

        return *this;
    }

    const std::string &OptionInfo::name() const { return m_name; }

    const std::string &OptionInfo::title() const { return m_title; }

    const std::string &OptionInfo::description() const { return m_description; }

    SANE_Int OptionInfo::id() const { return m_id; }

    SANE_Int OptionInfo::size() const { return m_size; }

    const OptionInfo &Option::info() const { return m_option_description; }

    std::optional<Option::value_type> Option::value_as_variant() const {
        std::visit([this](const auto &value) { this->value<std::decay_t<decltype(value)>>(); }, m_value);

        return m_value;
    }

    bool operator==(const Option &lhs, const Option &rhs) {
        return (lhs.m_device_handle == rhs.m_device_handle) && (lhs.info().id() == rhs.info().id());
    }

    bool operator!=(const Option &lhs, const Option &rhs) { return !(lhs == rhs); }

    bool operator==(const Fixed &lhs, const Fixed &rhs) { return (lhs.m_value == rhs.m_value); }

    bool operator!=(const Fixed &lhs, const Fixed &rhs) { return !(lhs == rhs); }

}  // namespace sanepp
