#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>

#include <sane/sane.h>

namespace sanepp {

    class Button {};
    class Group {};

    // TODO add specialization for fixed
    template<typename T>
    class Value {
       public:
        template<typename O>
        Value(const O &o);

        template<typename O>
        Value &operator=(O rhs);

        operator T() const { return m_value; }

        const T &value() const;

       private:
        T m_value;
    };

    template<typename T>
    template<typename O>
    Value<T>::Value(const O &value) : m_value(value) {}

    template<typename T>
    const T &Value<T>::value() const {
        return m_value;
    }

    template<typename T>
    template<typename O>
    Value<T> &Value<T>::operator=(O rhs) {
        m_value = std::move(rhs);

        return *this;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &os, const Value<T> &value) {
        os << value.value();
        return os;
    }

    template<typename T>
    struct SaneType {};

    template<>
    struct SaneType<Value<int>> {
        using type = SANE_Int;
    };

    template<>
    struct SaneType<Value<float>> {
        using type = SANE_Fixed;
    };

    template<>
    struct SaneType<Value<bool>> {
        using type = SANE_Bool;
    };

    template<>
    struct SaneType<Value<std::string>> {
        using type = SANE_String;
    };

    template<>
    struct SaneType<Value<Group>> {
        using type = Group;
    };

    template<>
    struct SaneType<Value<Button>> {
        using type = Button;
    };

    class OptionInfo final {
       public:
        OptionInfo(SANE_Int id);
        OptionInfo &name(const std::string &name);
        OptionInfo &title(const std::string &title);
        OptionInfo &description(const std::string &description);

        const std::string &name() const;
        const std::string &title() const;
        const std::string &description() const;
        SANE_Int id() const;

       private:
        const SANE_Int m_id;
        std::string m_name = "";
        std::string m_title = "";
        std::string m_description = "";
    };

    class Option final {
       public:
        typedef std::variant<Value<int>, Value<bool>, Value<float>, Value<std::string>, Value<Button>, Value<Group>>
            value_type;

        template<typename T>
        Option(SANE_Handle device_handle, const T &value, const OptionInfo &description);
        template<typename T>
        std::optional<Value<T>> value() const;
        const OptionInfo &info() const;

       private:
        SANE_Handle m_device_handle;
        OptionInfo m_option_description;
        mutable value_type m_value;
    };

    template<typename T>
    Option::Option(SANE_Handle device_handle, const T &value, const OptionInfo &description)
        : m_device_handle(device_handle), m_option_description(description), m_value(value) {}

    template<typename T>
    std::optional<Value<T>> Option::value() const {
        // TODO handle SANE_String correctly
        std::visit(
            [this](auto &variable) {
                using current_type = decltype(variable);

                if constexpr (!(std::is_same_v<Value<Button>, current_type> ||
                                std::is_same_v<Value<Group>, current_type> ||
                                std::is_same_v<Value<std::string>, current_type>)) {
                    typename SaneType<std::decay_t<decltype(variable)>>::type destination{};

                    SANE_Status sane_status = sane_control_option(m_device_handle, m_option_description.id(),
                                                                  SANE_ACTION_GET_VALUE, (void *)&destination, nullptr);

                    if (sane_status == SANE_STATUS_GOOD) {
                        variable = destination;
                    }
                }
            },
            m_value);

        if (std::holds_alternative<Value<T>>(m_value)) {
            return std::get<Value<T>>(m_value);
        }
        return std::optional<Value<T>>();
    }

}  // namespace sanepp
