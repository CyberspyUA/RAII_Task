#ifndef MY_WEAK_PTR_H_
#define MY_WEAK_PTR_H_

template <typename Resource>
class SharedPtr;

#include <memory>
template  <typename Resource>
class WeakPtr
{
private:
	Resource* resource;
	long* referenceCounter;
public:
	friend class SharedPtr<Resource>;
	WeakPtr(): resource(nullptr), referenceCounter(nullptr)
	{
	}
	WeakPtr(const WeakPtr<Resource>& rhs): resource(rhs.resource), referenceCounter(rhs.referenceCounter)
	{
	}
	WeakPtr(const SharedPtr<Resource>& rhs) : resource(rhs.Get()), referenceCounter(new long(rhs.UseCount()))
	{
	}
	WeakPtr<Resource>& operator=(const WeakPtr<Resource>& rhs)
    {
		if(this != &rhs)
		{
			resource = rhs.resource;
			referenceCounter = rhs.UseCount();
		}
		return *this;
	}

	WeakPtr<Resource>& operator=(const SharedPtr<Resource>& rhs)
    {
		resource = rhs.Get();
		referenceCounter = rhs.UseCount();
		return *this;
	}
	~WeakPtr() = default;

	void Reset()
    {
		resource = nullptr;
		referenceCounter = nullptr;
	}
	void Swap(WeakPtr<Resource>& rhs)
    {
		std::swap(resource, rhs.resource);
		std::swap(referenceCounter, rhs.referenceCounter);
	}
	long UseCount() const
	{
		return referenceCounter ? *referenceCounter : 0;
	}
	bool Expired() const
	{
		return referenceCounter ? *referenceCounter == 0 : true;
	}
	SharedPtr<Resource> Lock() const
	{
		if(!resource || Expired())
		{
			return SharedPtr<Resource>();
		}
		return SharedPtr<Resource>(*this);
	}
};
#endif