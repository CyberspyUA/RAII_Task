#ifndef WIN32_FILE_HANDLE_GUARD_
#define WIN32_FILE_HANDLE_GUARD_
#include <iostream>
#include <memory>
#include <Windows.h>
class Win32FileHandleGuard final
{
private:
	/**
	 * \brief Користувальницький deleter
	 */
	struct HandleDeleter
	{
		void operator()(HANDLE handle) const
		{
			if(handle != nullptr && handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(handle);
			}
		}
	};
	std::unique_ptr<void, HandleDeleter> fileHandle_;
public:
	explicit Win32FileHandleGuard(const LPTSTR& filename,
								  DWORD desiredAccess,
								  DWORD shareMode,
								  LPSECURITY_ATTRIBUTES securityAttributes,
								  DWORD creationDisposition,
								  DWORD flagsAndAttributes,
								  HANDLE templateFile)
									  :fileHandle_(CreateFileW(filename, 
															   desiredAccess, 
															   shareMode, 
															   securityAttributes, 
															   creationDisposition,
															   flagsAndAttributes,
															   templateFile),
															   HandleDeleter{})
	{
		if(fileHandle_.get() == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("Failed to create Win32 file handle.");
		}
	}
	operator HANDLE() const
	{
		return static_cast<HANDLE>(fileHandle_.get());
	}
	~Win32FileHandleGuard()
	{
		std::cout << "Win32 file handle closed.";
	}
};
#endif