#pragma once
#include <string>
#include <iostream>
class Serial
{
private:
	int speedBPS;
public:
	Serial();
	~Serial();

	void begin(int speedBPS) {
		this->speedBPS = speedBPS;
	}

	void println(std::string line) {
		std::cout << line << std::endl;
	}
};

