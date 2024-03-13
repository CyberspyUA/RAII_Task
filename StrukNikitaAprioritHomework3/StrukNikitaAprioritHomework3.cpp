/**
 * IDE: Microsoft Visual Studio 2022
 * C++ Standard: 2017 ISO
 * Compiler: MSVC C++
 */
#include "Win32FileHandleGuard.hpp"
#include "FunctorSockedDeleter.hpp"
#include <functional>
#include <winsock.h>
#include <stdexcept>
#include <memory>

#include "SharedPtr.h"
#include "WeakPtr.h"

int main()
{
    const LPTSTR filename = LPTSTR(L"testfile.txt");

    try
    {
        /**
         * Завдання 1 (Task 1)
         */
        Win32FileHandleGuard fileHandleGuard(filename,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (fileHandleGuard)
        {
            DWORD bytesWritten;
            const char* dataToWrite = "Lorem Ipsum is simply dummy text of the printing and typesetting industry."
                " Lorem Ipsum has been the industry's standard dummy text ever since the 1500s.\n";
            /**
             * Виконуємо запис тексту до файлу (Write text to a file)
             */
            if (WriteFile(fileHandleGuard, dataToWrite, static_cast<DWORD>(strlen(dataToWrite)), &bytesWritten, nullptr))
            {
                std::cout << bytesWritten << " bytes is written to the file.\n"
                    << "Data written to the file:\n"
                    << dataToWrite << "\n";
            }
            else
            {
                throw std::runtime_error("Failed to write to the file.");
            }
             /**
              * Повертаємо вказівник файлу на початок(Reset the file pointer to the beginning)
              */
        	LARGE_INTEGER move;
        	move.QuadPart = 0;

        	if (SetFilePointerEx(fileHandleGuard, move, nullptr, FILE_BEGIN))
        	{
        		std::cout << "File pointer reset to the beginning." << std::endl;
        	}
        	else
        	{
        		throw std::runtime_error("Failed to reset the file pointer.");
        	}

        	/**
        	 * Читаємо текст із файлу (Read text from the file)
        	 */
        	DWORD bytesRead;
        	const DWORD bufferSize = 1024;
        	char buffer[bufferSize];

        	if (ReadFile(fileHandleGuard, buffer, bufferSize, &bytesRead, nullptr))
        	{
        		std::cout << bytesRead << " bytes read from the file.\n"
							  <<"Data read from the file:\n"
                			  << dataToWrite << "\n";
        	}
        	else
        	{
        		throw std::runtime_error("Failed to read from the file.\n");
        	}
            /**
             * Завдання 2(Task 2)
             */

            /**
             * Унікальний вказівник із функцією видалення. (A unique pointer with delete function).
             */
            {
	            std::function<void(SOCKET*)> FunctionSocketDeleter = [](const SOCKET* socket)
            	{
					closesocket(*socket);
                    std::cout << "Socket is closed.\n";
					delete socket;
                    std::cout << "Unique pointer is deleted.\n";
				};
				std::unique_ptr<SOCKET, decltype(FunctionSocketDeleter)> socket_pointer_01(new SOCKET, FunctionSocketDeleter);
            }
            /**
             * Унікальний вказівник із функтором видаленння. (A unique pointer with delete functor).
             */
            {
	            std::unique_ptr<SOCKET, FunctorSockedDeleter> socket_pointer_01(new SOCKET);
            }

            /**
             * Унікальний вказівник із функцією-лямбдою видаленння. (A unique pointer with delete lambda function).
             */
            {
	            auto lambdaDeleter = [](const SOCKET* socket)
	            {
		            closesocket(*socket);
                    std::cout << "Socket is closed.\n";
					delete socket;
					std::cout << "Unique pointer is deleted.\n";
	            };
                std::unique_ptr<SOCKET, decltype(lambdaDeleter)> socket_pointer_01(new SOCKET, lambdaDeleter);
            }
            /**
             * Завдання 3(Task 3)
             */
            int* number = new int(100);
            SharedPtr sharedPointer1(number);
            std::cout << "Shared pointer value: " << sharedPointer1.Get() << "\n"
        			  << "Shared pointer reference counter: " << sharedPointer1.UseCount() << "\n";
            WeakPtr weakPointer1(sharedPointer1);
			std::cout << "Weak pointer reference counter: " << weakPointer1.UseCount() << "\n";
            if(!weakPointer1.Expired())
            {
	            SharedPtr<int> lockedSharedPointer1 = weakPointer1.Lock();
                std::cout << "Value managed by weak pointer after locking: " << *lockedSharedPointer1 << "\n";

            }
            else
            {
	            std::cout << "Weak pointer expired. Lock cannot be applied.\n";
            }
            sharedPointer1.Reset();
            weakPointer1.Reset();
            double* floatNumber1 = new double(100.00);
            double* floatNumber2 = new double(200.00);
            SharedPtr sharedPointer2(floatNumber1);
            SharedPtr sharedPointer3(floatNumber2);
            std::cout << "Shared pointer 2 address: " << sharedPointer2.Get() << "\n"
					  << "Shared pointer 3 address: " << sharedPointer3.Get() << "\n";
            sharedPointer2.Swap(sharedPointer3);
            std::cout << "Shared pointer 2 address: " << sharedPointer2.Get() << "\n"
					  << "Shared pointer 3 address: " << sharedPointer3.Get() << "\n";
        }
    }
    catch (std::exception& exceptionName)
    {
        std::cerr << "Error: " << exceptionName.what();
    }

    return 0;
}

