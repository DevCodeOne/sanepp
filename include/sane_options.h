#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>

#include <sane/sane.h>

namespace sanepp {

    class Button {
        int tmp;
    };
    class Group {
        int tmp;
    };

    template<typename T>
    struct SaneType {};

    template<>
    struct SaneType<int> {
        using type = SANE_Int;
    };

    template<>
    struct SaneType<float> {
        using type = SANE_Fixed;
    };

    template<>
    struct SaneType<bool> {
        using type = SANE_Bool;
    };

    template<>
    struct SaneType<std::string> {
        using type = SANE_String;
    };

    template<>
    struct SaneType<Group> {
        using type = Group;
    };

    template<>
    struct SaneType<Button> {
        using type = Button;
    };

    class OptionInfo final {
       public:
        OptionInfo(SANE_Int id);
        OptionInfo &name(const std::string &name);
        OptionInfo &title(const std::string &title);
        OptionInfo &description(const std::string &description);
        OptionInfo &size(SANE_Int size);

        const std::string &name() const;
        const std::string &title() const;
        const std::string &description() const;
        SANE_Int size() const;
        SANE_Int id() const;

       private:
        const SANE_Int m_id;
        SANE_Int m_size;
        std::string m_name = "";
        std::string m_title = "";
        std::string m_description = "";
    };

    class Option final {
       public:
        typedef std::variant<int, bool, float, std::string, Button, Group> value_type;

        template<typename T>
        Option(SANE_Handle device_handle, const T &value, const OptionInfo &description);
        template<typename T>
        std::optional<T> value() const;
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
    std::optional<T> Option::value() const {
        // TODO handle SANE_Fixed correctly
        std::visit(
            [this](auto &variable) {
                using current_type = std::decay_t<decltype(variable)>;

                if constexpr (!(std::is_same_v<Button, current_type> || std::is_same_v<Group, current_type> ||
                                std::is_same_v<std::string, current_type>)) {
                    typename SaneType<current_type>::type destination{};
                    SANE_Status sane_status = sane_control_option(m_device_handle, m_option_description.id(),
                                                                  SANE_ACTION_GET_VALUE, (void *)&destination, nullptr);
                    if (sane_status) {
                        variable = destination;
                    }
                }
                if constexpr (std::is_same_v<std::string, current_type>) {
                    auto destination = std::make_unique<char[]>(info().size() + 1);
                    SANE_Status sane_status =
                        sane_control_option(m_device_handle, m_option_description.id(), SANE_ACTION_GET_VALUE,
                                            (void *)destination.get(), nullptr);
                    if (sane_status == SANE_STATUS_GOOD) {
                        variable = std::string(destination.get());
                    }
                }
            },
            m_value);

        if (std::holds_alternative<T>(m_value)) {
            return std::get<T>(m_value);
        }
        return std::optional<T>();
    }  // namespace sanepp

}  // namespace sanepp
