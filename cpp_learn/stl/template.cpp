#include <vector>


template<typename T>
void template_reference_parameter_test(T& value)
{

}

template<typename T>
void template_universal_reference_parameter_test(T&& value)
{
}

template<typename T>
void template_pointer_parameter_test(const T* value)
{
}