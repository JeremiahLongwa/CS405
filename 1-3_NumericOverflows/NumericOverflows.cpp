#include <iostream>
#include <limits>
#include <type_traits>
#include <utility>
#include <string>
#include <typeinfo>

/// <summary>
/// Safely adds numbers and detects overflow.
/// Returns { success, result }
/// </summary>
template <typename T>
std::pair<bool, T> add_numbers(
    const T& start,
    const T& increment,
    const unsigned long int& steps)
{
    T result = start;

    for (unsigned long int i = 0; i < steps; ++i)
    {
        // Check for overflow/underflow before performing addition
        if (std::numeric_limits<T>::is_integer)
        {
            // Integer overflow detection for positive increments
            if (increment > 0)
            {
                // Check if result + increment would exceed max value
                if (result > std::numeric_limits<T>::max() - increment)
                {
                    return std::make_pair(false, result); // Overflow would occur
                }
            }
            // Integer underflow detection for negative increments (signed types only)
            else if (increment < 0 && std::numeric_limits<T>::is_signed)
            {
                // Check if result + increment would go below min value
                if (result < std::numeric_limits<T>::min() - increment)
                {
                    return std::make_pair(false, result); // Underflow would occur
                }
            }
            // If increment is 0, no overflow/underflow possible
        }
        else
        {
            // Floating-point overflow/underflow detection
            if (increment > 0)
            {
                // Check for positive overflow
                if (result > std::numeric_limits<T>::max() - increment)
                {
                    return std::make_pair(false, result); // Would overflow to INF
                }
            }
            else if (increment < 0)
            {
                // Check for negative overflow (toward -INF)
                // For floating-point, we check against negative max (not min)
                if (result < -std::numeric_limits<T>::max())
                {
                    // Already at or below -max, subtracting more would stay in range
                    // Actually we need to check if result + (-increment) would underflow
                    // But since increment is negative, result += increment could underflow
                    if (result < std::numeric_limits<T>::lowest() - increment)
                    {
                        return std::make_pair(false, result);
                    }
                }
            }
        }

        // Safe to perform the addition
        result += increment;
    }

    return std::make_pair(true, result);
}

/// <summary>
/// Safely subtracts numbers and detects underflow.
/// Returns { success, result }
/// </summary>
template <typename T>
std::pair<bool, T> subtract_numbers(
    const T& start,
    const T& decrement,
    const unsigned long int& steps)
{
    T result = start;

    for (unsigned long int i = 0; i < steps; ++i)
    {
        // Check for underflow/overflow before performing subtraction
        if (std::numeric_limits<T>::is_integer)
        {
            // For positive decrements (normal subtraction)
            if (decrement > 0)
            {
                // Check if result - decrement would go below min value
                if (result < std::numeric_limits<T>::min() + decrement)
                {
                    return std::make_pair(false, result); // Underflow would occur
                }
            }
            // For negative decrements (subtracting negative = adding)
            else if (decrement < 0 && std::numeric_limits<T>::is_signed)
            {
                // Check if result - (-decrement) would exceed max value
                // Since decrement is negative, result - decrement = result + |decrement|
                if (result > std::numeric_limits<T>::max() + decrement) // decrement is negative
                {
                    return std::make_pair(false, result); // Overflow would occur
                }
            }
            // If decrement is 0, no underflow/overflow possible
        }
        else
        {
            // Floating-point underflow/overflow detection
            if (decrement > 0)
            {
                // Check for underflow when subtracting positive value
                if (result < std::numeric_limits<T>::lowest() + decrement)
                {
                    return std::make_pair(false, result); // Would underflow
                }
            }
            else if (decrement < 0)
            {
                // Check for overflow when subtracting negative value (adding)
                if (result > std::numeric_limits<T>::max() + decrement) // decrement is negative
                {
                    return std::make_pair(false, result); // Would overflow to INF
                }
            }
        }

        // Safe to perform the subtraction
        result -= decrement;
    }

    return std::make_pair(true, result);
}

template <typename T>
void test_overflow()
{
    const unsigned long int steps = 5;
    const T increment = std::numeric_limits<T>::max() / steps;
    const T start = 0;

    std::cout << "Overflow Test of Type = " << typeid(T).name() << std::endl;

    std::cout << "\tAdding Numbers Without Overflow (" 
              << +start << ", " << +increment << ", " << steps << ") = ";
    std::pair<bool, T> result = add_numbers<T>(start, increment, steps);
    std::cout << +result.second
              << " | Overflow: "
              << std::boolalpha << !result.first
              << std::endl;

    std::cout << "\tAdding Numbers With Overflow (" 
              << +start << ", " << +increment << ", " << (steps + 1) << ") = ";
    result = add_numbers<T>(start, increment, steps + 1);
    std::cout << +result.second
              << " | Overflow: "
              << std::boolalpha << !result.first
              << std::endl;
}

template <typename T>
void test_underflow()
{
    const unsigned long int steps = 5;
    const T decrement = std::numeric_limits<T>::max() / steps;
    const T start = std::numeric_limits<T>::max();

    std::cout << "Underflow Test of Type = " << typeid(T).name() << std::endl;

    std::cout << "\tSubtracting Numbers Without Underflow (" 
              << +start << ", " << +decrement << ", " << steps << ") = ";
    std::pair<bool, T> result = subtract_numbers<T>(start, decrement, steps);
    std::cout << +result.second
              << " | Underflow: "
              << std::boolalpha << !result.first
              << std::endl;

    std::cout << "\tSubtracting Numbers With Underflow (" 
              << +start << ", " << +decrement << ", " << (steps + 1) << ") = ";
    result = subtract_numbers<T>(start, decrement, steps + 1);
    std::cout << +result.second
              << " | Underflow: "
              << std::boolalpha << !result.first
              << std::endl;
}

void do_overflow_tests(const std::string& star_line)
{
    std::cout << "\n" << star_line << std::endl;
    std::cout << "*** Running Overflow Tests ***" << std::endl;
    std::cout << star_line << std::endl;

    // Test signed integer types
    test_overflow<char>();
    test_overflow<wchar_t>();
    test_overflow<short>();
    test_overflow<int>();
    test_overflow<long>();
    test_overflow<long long>();

    // Test unsigned integer types
    test_overflow<unsigned char>();
    test_overflow<unsigned short>();
    test_overflow<unsigned int>();
    test_overflow<unsigned long>();
    test_overflow<unsigned long long>();

    // Test floating-point types
    test_overflow<float>();
    test_overflow<double>();
    test_overflow<long double>();
}

void do_underflow_tests(const std::string& star_line)
{
    std::cout << "\n" << star_line << std::endl;
    std::cout << "*** Running Underflow Tests ***" << std::endl;
    std::cout << star_line << std::endl;

    // Test signed integer types
    test_underflow<char>();
    test_underflow<wchar_t>();
    test_underflow<short>();
    test_underflow<int>();
    test_underflow<long>();
    test_underflow<long long>();

    // Test unsigned integer types
    test_underflow<unsigned char>();
    test_underflow<unsigned short>();
    test_underflow<unsigned int>();
    test_underflow<unsigned long>();
    test_underflow<unsigned long long>();

    // Test floating-point types
    test_underflow<float>();
    test_underflow<double>();
    test_underflow<long double>();
}

int main()
{
    const std::string star_line(50, '*');

    std::cout << "Starting Numeric Underflow / Overflow Tests!" << std::endl;

    do_overflow_tests(star_line);
    do_underflow_tests(star_line);

    std::cout << "\nAll Numeric Underflow / Overflow Tests Complete!" << std::endl;

    return 0;
}