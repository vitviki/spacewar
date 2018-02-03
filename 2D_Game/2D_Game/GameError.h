#ifndef GAME_ERROR_H_
#define GAME_ERROR_H_

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <exception>

namespace GameErrorNS
{
	// Error Codes
	// Negative numbers are fatal errors that may require the game to shutdown
	// Positive numbers are warnings that do no require the game to shutdown
	const int FATAL_ERROR = -1;
	const int WARNING = 1;
}

class GameError : public std::exception
{
private:

	int m_iErrorCode;
	std::string m_Message;

public:

	// Default Constructor.
	GameError() throw()
		: m_iErrorCode(GameErrorNS::FATAL_ERROR)
		, m_Message("Undefined Error!")
	{

	}

	// Copy Constructor.
	GameError(const GameError& e) throw()
		: std::exception(e)
		, m_iErrorCode(e.m_iErrorCode)
		, m_Message(e.m_Message)
	{

	}

	// Constructor with arguments.
	GameError(int iCode, const std::string& s) throw()
		: m_iErrorCode(iCode)
		, m_Message(s)
	{

	}

	// Assignment operator.
	GameError& operator=(const GameError& rhs) throw()
	{
		std::exception::operator =(rhs);
		this->m_iErrorCode = rhs.m_iErrorCode;
		this->m_Message = rhs.m_Message;
	}

	// Destructor.
	virtual ~GameError() throw()
	{

	}

	// Override what from base class.
	virtual const char* what(void) const throw()
	{
		return this->GetMessage();
	}

	const char* GetMessage(void) const throw()
	{
		return m_Message.c_str();
	}

	int GetErrorCode(void) const throw()
	{
		return m_iErrorCode;
	}
};

#endif