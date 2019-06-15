#pragma once

#include <filesystem>
#include <string>
#include <type_traits>

#include <sol3/sol.hpp>

namespace d20tempest::components
{
    // the partial specialization of A is enabled via a template parameter
    template<class T, class Enable = void>
    class Characteristic {}; // primary template

    template<typename T>
    class Characteristic<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
    {
    private:
        T m_characteristicValue = 0;
        mutable T m_modifierCache = 0;
        mutable bool m_cacheInvalidated = true;

        std::string m_name;
        std::string m_shortName;
        std::string m_scriptPath;

        sol::state m_luaState;
        sol::function m_getModifier;
        sol::function m_checkValueBeforeSet;
    public:
        explicit Characteristic(const std::string& scriptPath) : m_scriptPath(scriptPath)
        {
            m_luaState.script_file(scriptPath);
            m_getModifier = m_luaState["getModifier"];
            m_checkValueBeforeSet = m_luaState["checkValueBeforeSet"];

            m_name = m_luaState["name"];
            m_shortName = m_luaState["shortName"];
            m_luaState.set_function(m_shortName, [this]{ return Modifier(); });
            m_luaState.set_function(m_name, [this]{ return Value(); });
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
            return m_characteristicValue;
        }

        virtual bool Value(T value)
        {
            if(m_checkValueBeforeSet.valid() && m_checkValueBeforeSet(value))
            {
                m_characteristicValue = value;
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
    };
}