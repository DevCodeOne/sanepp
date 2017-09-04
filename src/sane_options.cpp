#include "sane_options.h"

// Implementation of sane_option_value
sane_option_value::sane_option_value(SANE_Value_Type type)
    : m_value_type(type) {
}

SANE_Value_Type sane_option_value::value_type() const {
    return m_value_type;
}

// Implementation of sane_option_value_bool
sane_option_value_bool::sane_option_value_bool(SANE_Bool value)
    : sane_option_value(SANE_TYPE_BOOL), m_value(value) {
}

sane_option_value *sane_option_value_bool::copy() const {
    return new sane_option_value_bool(m_value);
}

void sane_option_value_bool::value(const sane_option_value_bool &value) {
        m_value = value.m_value;
}

// Implementation of sane_operation_type_int
sane_option_value_int::sane_option_value_int(SANE_Int value)
    : sane_option_value(SANE_TYPE_INT), m_value(value) {
}

void sane_option_value_int::value(const sane_option_value_int &value) {
    m_value = value.m_value;
}

sane_option_value *sane_option_value_int::copy() const {
    return new sane_option_value_int(m_value);
}

// Implementation of sane_option_value_fixed
sane_option_value_fixed::sane_option_value_fixed(SANE_Fixed value)
    : sane_option_value(SANE_TYPE_FIXED), m_value(value) {
}

void sane_option_value_fixed::value(const sane_option_value_fixed &value) {
    m_value = value.m_value;
}

sane_option_value *sane_option_value_fixed::copy() const {
    return new sane_option_value_fixed(m_value);
}

// Implementation of sane_option_value_string
sane_option_value_string::sane_option_value_string(SANE_String value)
    : sane_option_value(SANE_TYPE_STRING), m_value(value) {
}

void sane_option_value_string::value(const sane_option_value_string &value) {
    m_value = value.m_value;
}

sane_option_value *sane_option_value_string::copy() const {
    return new sane_option_value_string(m_value);
}

// Implementation of sane_option_value_button
sane_option_value_button::sane_option_value_button()
    : sane_option_value(SANE_TYPE_BUTTON) {
}

sane_option_value *sane_option_value_button::copy() const {
    return new sane_option_value_button();
}

// TODO Implement groups better as groups of values
// or ignore them altogether
// Implementation of sane_option_value_group
sane_option_value_group::sane_option_value_group()
    : sane_option_value(SANE_TYPE_GROUP) {
}

sane_option_value *sane_option_value_group::copy() const {
    return new sane_option_value_button();
}

option_description::option_description(SANE_Int id) : m_id(id) {
}

// Implementation of option_description
option_description &option_description::name(const std::string &name) {
    m_name = name;

    return *this;
}

// Implementation of option_description
option_description &option_description::title(const std::string &title) {
    m_title = title;

    return *this;
}

// Implementation of option_description
option_description &option_description::description(const std::string &desc) {
    m_description = desc;

    return *this;
}

const std::string &option_description::name() const {
    return m_name;
}

const std::string &option_description::title() const {
    return m_title;
}

const std::string &option_description::description() const {
    return m_description;
}

SANE_Int option_description::id() const {
    return m_id;
}

// Implementation of option
option::option(SANE_Handle device_handle,
        const sane_option_value *value,
        const option_description &description)
    : m_device_handle(device_handle),
    m_value(value->copy()), m_option_description(description) {
}

// TODO handle SANE_String correctly
// maybe friend this method so it has direct access to the classes member
const sane_option_value &option::value() const {
    // std::unique_ptr needs a custom deleter for void *
    // TODO correct this later
    std::shared_ptr<void> destination = nullptr;
    switch (m_value->value_type()) {
        case SANE_TYPE_BOOL:
            destination = std::make_shared<SANE_Bool>();
            break;
        case SANE_TYPE_INT:
            destination = std::make_shared<SANE_Int>();
            break;
        case SANE_TYPE_FIXED:
            destination = std::make_shared<SANE_Fixed>();
            break;
        case SANE_TYPE_STRING:
            break;
        default:
            break;
    }

    if (!destination)
        return *m_value.get();

    SANE_Status sane_status = sane_control_option(m_device_handle,
            m_option_description.id(), SANE_ACTION_GET_VALUE,
            destination.get(), nullptr);

    if (sane_status == SANE_STATUS_GOOD) {
        std::unique_ptr<sane_option_value> new_value;
        switch (m_value->value_type()) {
            case SANE_TYPE_BOOL:
                m_value->value_bool(*((SANE_Bool *)destination.get()));
                break;
            case SANE_TYPE_INT:
                m_value->value_int(*((SANE_Int *)destination.get()));
                break;
            case SANE_TYPE_FIXED:
                m_value->value_fixed(*((SANE_Fixed *)destination.get()));
                break;
            case SANE_TYPE_STRING:
                break;
            default:
                break;
        }
    }

    return *m_value.get();
}

const option_description &option::description() const {
    return m_option_description;
}
