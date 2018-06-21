#if !defined(UUID_BC9B5745_6DEA_4BBC_AD5A_22F28C716F3E)
#define UUID_BC9B5745_6DEA_4BBC_AD5A_22F28C716F3E

#include <boost/type_index.hpp>

template <typename T>
auto T_name()
  noexcept(noexcept(boost::typeindex::type_id_with_cvr<T>().pretty_name()))
{
  return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}

template <typename T>
auto T_name(T)
  noexcept(noexcept(T_name<T>()))
{
  return T_name<T>();
}

#endif /* if not defined 'UUID_BC9B5745_6DEA_4BBC_AD5A_22F28C716F3E' */
