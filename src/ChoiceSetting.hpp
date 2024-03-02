#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ChoiceSettingValue : public SettingValue {
    std::vector<std::string> m_choices;
};