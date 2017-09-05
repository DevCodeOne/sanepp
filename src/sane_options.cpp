#include <memory>

#include "sane_options.h"

//Implementation of sane_int
sane_int::sane_int(SANE_Int value) : m_value(value) {
}

SANE_Int sane_int::value() const {
    return m_value;
}

std::ostream &operator<<(std::ostream &os, const sane_int &value) {
    os << value.value();
    return os;
}

//Implementation of sane_fixed
sane_fixed::sane_fixed(SANE_Fixed value) : m_value(value) {
}

// Implementation of sane_bool
sane_bool::sane_bool(SANE_Bool value) : m_value(value) {
}

// Implementation of sane_option_description
sane_option_description::sane_option_description(SANE_Int id) : m_id(id) {
}

sane_option_description &sane_option_description::name(const std::string &name) {
    m_name = name;

    return *this;
}

sane_option_description &sane_option_description::title(const std::string &title) {
    m_title = title;

    return *this;
}

sane_option_description &sane_option_description::description(const std::string &desc) {
    m_description = desc;

    return *this;
}

const std::string &sane_option_description::name() const {
    return m_name;
}

const std::string &sane_option_description::title() const {
    return m_title;
}

const std::string &sane_option_description::description() const {
    return m_description;
}

SANE_Int sane_option_description::id() const {
    return m_id;
}

// Implementation of option
// TODO handle SANE_String correctly
// maybe friend this method so it has direct access to the classes member
const sane_option::value_type &sane_option::value() const {
    // std::unique_ptr needs a custom deleter for void *
    // TODO correct this later

    auto get_option_value = [this](void *destination) -> SANE_Status {
        SANE_Status sane_status = sane_control_option(m_device_handle,
                m_option_description.id(), SANE_ACTION_GET_VALUE,
                destination, nullptr);

        return sane_status;
    };

    if (std::holds_alternative<sane_int>(m_value)) {
        SANE_Int destination;

        if (get_option_value(&destination) == SANE_STATUS_GOOD)
            m_value = sane_int(destination);
    } else if (std::holds_alternative<sane_fixed>(m_value)) {
        SANE_Fixed destination;

        if (get_option_value(&destination) == SANE_STATUS_GOOD)
            m_value = sane_fixed(destination);
    } else if (std::holds_alternative<sane_bool>(m_value)) {
        SANE_Bool destination;

        if (get_option_value(&destination) == SANE_STATUS_GOOD)
            m_value = sane_bool(destination);
    } else if (std::holds_alternative<sane_string>(m_value)) {
        // TODO handle correctly
    }

    return m_value;
}

const sane_option_description &sane_option::description() const {
    return m_option_description;
}
