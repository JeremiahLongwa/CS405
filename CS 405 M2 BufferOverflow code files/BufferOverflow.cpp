#include <iomanip>
#include <iostream>
#include <cstring>
#include <limits>

int main()
{
    std::cout << "Buffer Overflow Example" << std::endl;

    // The account number must remain unchanged and directly before the input buffer
    const std::string account_number = "CharlieBrown42";
    char user_input[20];

    std::cout << "Enter a value: ";

    // Safely read input with length checking
    std::cin.getline(user_input, sizeof(user_input));

    // Check if input was too long (buffer overflow attempt)
    if (std::cin.fail())
    {
        std::cin.clear(); // Clear error state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "ERROR: Input too long. Buffer overflow attempt prevented!"
                  << std::endl;
    }
    else
    {
        std::cout << "You entered: " << user_input << std::endl;
    }

    // Account number must never change
    std::cout << "Account Number = " << account_number << std::endl;

    return 0;
}