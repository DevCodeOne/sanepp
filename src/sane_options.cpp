#include "sane_options.h"

namespace sanepp {

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

    const std::string &OptionInfo::name() const { return m_name; }

    const std::string &OptionInfo::title() const { return m_title; }

    const std::string &OptionInfo::description() const { return m_description; }

    SANE_Int OptionInfo::id() const { return m_id; }

    const OptionInfo &Option::info() const { return m_option_description; }
}  // namespace sanepp
