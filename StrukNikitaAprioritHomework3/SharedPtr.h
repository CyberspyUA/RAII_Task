#ifndef MY_SHARED_PTR_
#define MY_SHARED_PTR_
#include <memory>
template  <typename Resource>
class WeakPtr;
template <typename Resource>
class SharedPtr
{
private:
	Resource* resource;
	long* referenceCounter;
public:
	
	SharedPtr() : resource(nullptr), referenceCounter(nullptr)
	{
	}
	explicit SharedPtr(Resource* res) : resource(res), referenceCounter(new long(1))
	{
	}
	SharedPtr(const SharedPtr<Resource>& rhs) : resource(rhs.resource), referenceCounter(rhs.referenceCounter)
	{
		if(referenceCounter)
		{
			++*referenceCounter;
		}
	}
	SharedPtr(const WeakPtr<Resource>& rhs) : resource(rhs.resource), referenceCounter(new long(rhs.UseCount()))
	{
		if(referenceCounter)
		{
			++*referenceCounter;
		}
	}
	SharedPtr<Resource>& operator=(const SharedPtr<Resource>& rhs)
	{
		if (this != &rhs) 
		{
            Reset();
            resource = rhs.resource;
            referenceCounter = rhs.referenceCounter;
            if (referenceCounter)
            {
	            ++(*referenceCounter);
            }
        }
        return *this;
	}
	~SharedPtr()
	{
		Reset();
	}

	void Reset()
	{
		if(referenceCounter)
		{
			if(--*referenceCounter == 0)
			{
				delete resource;
				delete referenceCounter;
			}
			resource = nullptr;
			referenceCounter = nullptr;
		}
	}
	void Reset(Resource* res)
	{
		Reset();
		resource = res;
		referenceCounter = new long(1);
	}
	void Swap(SharedPtr<Resource>& rhs)
	{
		std::swap(resource, rhs.resource);
		std::swap(referenceCounter, rhs.referenceCounter);
	}

	Resource* Get() const
	{
		return resource;
	}
	Resource& operator*() const
	{
		return *resource;
	}
	Resource* operator->() const
	{
		return resource;
	}
	long UseCount() const
	{
		return referenceCounter ? *referenceCounter : 0;
	}
};
#endif