#pragma once

#include <filesystem>
#include <string>
#include <sstream>
#include <type_traits>

#include <sol3/sol.hpp>

namespace d20tempest::components
{
    // the partial specialization of A is enabled via a template parameter
    template<class T, class Enable = void>
    class Ability {}; // primary template

    template<typename T>
    class Ability<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
    {
    private:
        T m_abilityValue = 0;
        T m_tempBonus = 0;
        mutable T m_modifierCache = 0;
        mutable bool m_cacheInvalidated = true;

        std::string m_name;
        std::string m_shortName;
        std::string m_scriptPath;

        sol::state m_luaState;
        sol::function m_getModifier;
        sol::function m_checkValueBeforeSet;
    public:
        explicit Ability(const std::string& scriptPath) : m_scriptPath(scriptPath)
        {
            m_luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::count);
            m_luaState.script_file(scriptPath);
            m_getModifier = m_luaState["getModifier"];
            m_checkValueBeforeSet = m_luaState["checkValueBeforeSet"];

            m_name = m_luaState["name"];
            m_shortName = m_luaState["shortName"];
            m_luaState.set_function(m_shortName, [this]{ return Modifier(); });
            m_luaState.set_function(m_name, [this]{ return Value(); });

            std::stringstream sstream;
            sstream << "Temp" << m_shortName << "Bonus";
            m_luaState.set_function(sstream.str(), [this]{ return Temp(); });
            sstream.str("");
            sstream << "SetTemp" << m_shortName << "Bonus";
            m_luaState.set_function(sstream.str(), [this](T value){ Temp(value); });
        }

        virtual T Modifier() const
        {
            if(m_cacheInvalidated)
            {
                m_modifierCache = m_getModifier();
                m_cacheInvalidated = false;
            }

            return m_modifierCache;
        }
        
        virtual T Value() const
        {
            return m_abilityValue;
        }

        virtual bool Value(T value)
        {
            if(m_checkValueBeforeSet.valid() && m_checkValueBeforeSet(value))
            {
                m_abilityValue = value;
                m_cacheInvalidated = true;
                return true;
            }

            return false;
        }

        virtual std::string Name() const
        {
            return m_name;
        }

        virtual std::string ShortName() const
        {
            return m_shortName;
        }

        virtual T Temp() const
        {
            return m_tempBonus;
        }

        virtual void Temp(T val)
        {
            m_tempBonus += val;
            m_cacheInvalidated = true;
        }

        virtual void ResetTemp()
        {
            m_tempBonus = 0;
            m_cacheInvalidated = true;
        }
    };
}