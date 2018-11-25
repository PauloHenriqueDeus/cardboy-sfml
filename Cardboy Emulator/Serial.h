#pragma once
#include <string>
#include <iostream>
class Serial_
{
private:
	int speedBPS;
public:
	Serial_();
	~Serial_();

	void begin(int speedBPS) {
		this->speedBPS = speedBPS;
	}

	void println(std::string line) {
		std::cout << line << std::endl;
	}
};

