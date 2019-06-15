#pragma once

#include <string>
#include <type_traits>

namespace d20tempest::components
{
    template<typename T, 
        typename std::enable_if_t<std::is_trivially_constructible<T>::type || 
                                  std::is_arithmetic<T>::type> = 0>
    class Characteristic
    {
    private:
        T m_charac;
        std::string m_name;
        std::string m_shortName;
    
    public:
        virtual T Modifier() const = 0;
        
        virtual T Value() const = 0;
        virtual void Value(T value) = 0;

        virtual std::string Name() = 0;
        virtual std::string ShortName() = 0;
    };
}