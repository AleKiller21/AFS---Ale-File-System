#pragma once
class ErrorHandler
{
public:
	ErrorHandler();
	static std::string handleError(int errorCode);

	~ErrorHandler();
};

