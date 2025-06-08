#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <type_traits>
#include <list>

using namespace std;

// (based on: https://stackoverflow.com/questions/35293470/checking-if-a-type-is-a-map)
// (based on: https://devblogs.microsoft.com/oldnewthing/20190619-00/?p=102599 )

// Helper to determine if container is STL map
namespace detail {
    // Needed for some older versions of GCC
    template<typename...>
    struct voider { using type = void; };

    // std::void_t will be part of C++17, but until then define it ourselves:
    template<typename... T>
    using void_t = typename voider<T...>::type;

    template<typename T, typename U = void>
    struct is_mappish_impl : std::false_type {};

    template<typename T>
    struct is_mappish_impl<T, void_t<typename T::key_type,
        typename T::mapped_type,
        decltype(std::declval<T&>()[std::declval<const typename T::key_type&>()])>>
        : std::true_type { };
}

template<typename T>
struct is_mappish : detail::is_mappish_impl<T>::type {};


// 1) Takes any STL container by reference, and uses a range - based loop to display all its elements.
template <typename T>
auto rangeBasedLoop(const T& container, true_type)
{
    for (auto mapItem : container)
    {
        auto& key = mapItem.first;
        auto& value = mapItem.second;

        cout << "{ " << key << ", " << value << " }" << "; ";
    }
}

template <typename T>
auto rangeBasedLoop(const T& container, false_type)
{
    for (const auto it : container) {
        cout << it << "; ";
    }
}

template<typename C, typename T = typename C::value_type>
auto rangeBasedLoop(const C& container)
{
    cout << "\nContainer content type name: " << typeid(T).name();
    cout << "\nIs map: " << (is_mappish<C>{} ? "true" : "false");
    cout << "\nData: ";

    return rangeBasedLoop(container, is_mappish<C>{});
}


// 2) Takes any STL container by reference, and uses iterators to display all its elements, the iterator type is displayed via auto.
template <typename T>
auto iteratorsBasedLoop(const T& container, true_type)
{
    for (auto it = container.begin(); it != container.end(); ++it) {
        auto& key = it->first;
        auto& value = it->second;

        cout << "{ " << key << ", " << value << " }" << "; ";
    }
}

template <typename T>
auto iteratorsBasedLoop(const T& container, false_type)
{
    for (auto it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << "; ";
    }
}

template<typename C, typename T = typename C::value_type>
auto iteratorsBasedLoop(const C& container)
{
    cout << "\nContainer content type name: " << typeid(T).name();
    cout << "\nIs map: " << (is_mappish<C>{} ? "true" : "false");
    cout << "\nData: ";

    return iteratorsBasedLoop(container, is_mappish<C>{});
}


// Demo
int main() {

    // Data
    map<int, int> m = { {1,1}, {2,2}, {3,3}, {4,4}, {5,5} };
    vector<string> v = { "Volvo", "BMW", "Ford", "Mazda" };
    list<int> l = { 1, 3, 2, 8, 9 };

    cout << endl << endl << "Range based loop" << endl;
    rangeBasedLoop<vector<string>>(v);
    rangeBasedLoop<map<int, int>>(m);
    rangeBasedLoop<list<int>>(l);

    cout << endl << endl << "Iterator based loop" << endl;
    iteratorsBasedLoop<vector<string>>(v);
    iteratorsBasedLoop<map<int, int>>(m);
    iteratorsBasedLoop<list<int>>(l);

    cout << endl << endl;
    return 0;
}
