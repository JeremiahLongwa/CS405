// Exceptions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdexcept> 
#include <exception>  

// ADDED: Custom exception derived from std::exception (via std::runtime_error)
class CustomApplicationException : public std::runtime_error
{
public:
  explicit CustomApplicationException(const char* msg)
    : std::runtime_error(msg)
  {}
};

bool do_even_more_custom_application_logic()
{
  // TODO: Throw any standard exception
  throw std::runtime_error("Standard exception thrown in do_even_more_custom_application_logic()");

  std::cout << "Running Even More Custom Application Logic." << std::endl;

  return true;
}

void do_custom_application_logic()
{
  // TODO: Wrap the call to do_even_more_custom_application_logic()
  //  with an exception handler that catches std::exception, displays
  //  a message and the exception.what(), then continues processing
  std::cout << "Running Custom Application Logic." << std::endl;

  try  // ADDED: try-block as requested
  {
    if(do_even_more_custom_application_logic())
    {
      std::cout << "Even More Custom Application Logic Succeeded." << std::endl;
    }
  }
  catch(const std::exception& ex) // ADDED: catch std::exception as requested
  {
    std::cout << "Caught std::exception in do_custom_application_logic(): " << ex.what() << std::endl;
    // continue processing
  }

  // TODO: Throw a custom exception derived from std::exception
  //  and catch it explictly in main
  throw CustomApplicationException("Custom exception thrown in do_custom_application_logic()");

  std::cout << "Leaving Custom Application Logic." << std::endl;

}

float divide(float num, float den)
{
  // TODO: Throw an exception to deal with divide by zero errors using
  //  a standard C++ defined exception
  if(den == 0.0f) // ADDED: divide-by-zero guard
  {
    throw std::invalid_argument("divide(): denominator is zero");
  }

  return (num / den);
}

void do_division() noexcept
{
  //  TODO: create an exception handler to capture ONLY the exception thrown
  //  by divide.

  float numerator = 10.0f;
  float denominator = 0;

  try  // ADDED: handler around divide call
  {
    auto result = divide(numerator, denominator);
    std::cout << "divide(" << numerator << ", " << denominator << ") = " << result << std::endl;
  }
  catch(const std::invalid_argument& ex) // ADDED: ONLY the exception thrown by divide()
  {
    std::cout << "Caught divide exception in do_division(): " << ex.what() << std::endl;
  }
}

int main()
{
  std::cout << "Exceptions Tests!" << std::endl;

  // TODO: Create exception handlers that catch (in this order):
  //  your custom exception
  //  std::exception
  //  uncaught exception
  //  that wraps the whole main function, and displays a message to the console.
  try // ADDED: wrap whole main body
  {
    do_division();
    do_custom_application_logic();
  }
  catch(const CustomApplicationException& ex) // ADDED: custom exception first
  {
    std::cout << "Caught CustomApplicationException in main(): " << ex.what() << std::endl;
  }
  catch(const std::exception& ex) // ADDED: std::exception next
  {
    std::cout << "Caught std::exception in main(): " << ex.what() << std::endl;
  }
  catch(...) // ADDED: catch-all last
  {
    std::cout << "Caught unknown (uncaught) exception in main()." << std::endl;
  }

  return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
