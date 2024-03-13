#ifndef FUNCTOR_SOCKED_DELETER_
#define FUNCTOR_SOCKED_DELETER_
#include <Windows.h>

struct FunctorSockedDeleter
{
	void operator()(SOCKET* socket) const
	{
		closesocket(*socket);
		std::cout << "Socket is closed.\n";
		delete socket;
		std::cout << "Unique pointer is deleted.\n";
	}
};
#endif