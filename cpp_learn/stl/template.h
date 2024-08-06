#pragma once
template<typename T>
void template_reference_parameter_test(T& value);

template<typename T>
void template_universal_reference_parameter_test(T&& value);
