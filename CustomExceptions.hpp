#pragma once
#include <exception>

class EOFException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Reached EOF";
	}
};