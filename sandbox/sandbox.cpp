// sandbox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <memory>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/variant.hpp>

#define VOLT_THROW_EXCEPTION(err)      \
	throw err                          \
		<< boost::throw_line(__LINE__) \
		<< boost::throw_file(__FILE__) \
		<< boost::throw_function(BOOST_CURRENT_FUNCTION)

template <typename T>
inline const char* typeOf(const T&)
{
	return typeid(T).name();
}

struct Null {};

using FieldValueType = boost::variant<
	Null,
	bool,
	char,
	int, 
	long long, 
	unsigned long long, 
	double, 
	std::string>;

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...))
}

template <typename Target>
struct ConvertParam
	: boost::static_visitor<Target>
{

public:

	ConvertParam(const std::string & funcName, int argX)
		: funcName_(funcName)
	, argX_(argX)
	{
	}

	template <typename Source>
	typename boost::enable_if<boost::is_convertible<Source, Target>, const Target>::type
	operator()(const Source& source) const
	{
		Target t = static_cast<Target>(source);
		return t;
	}

	template <typename Source>
	typename boost::disable_if<boost::is_convertible<Source, Target>, const Target>::type
	operator()(const Source& source) const
	{
		//typename boost::remove_reference<Target>::type target;
		//VOLT_THROW_EXCEPTION(WrongParameterType(funcName_, argX_, typeOf(source), typeOf(target)));
		return Target();
	}

	static Target apply(const FieldValueType& value, const std::string& funcName, int argX)
	{
		ConvertParam<Target> visitor(funcName, argX);
		return value.apply_visitor(visitor);
	}

private:
		
	const std::string & funcName_;
	const int argX_;
};

int _tmain(int argc, _TCHAR* argv[])
{

	//Container cont( A(1), C(2), D(3) );
	FieldValueType val = 25.f;
	std::string str("FuncName");

	//ConvertParam<std::string> visitor(str, 1);

	auto result = ConvertParam<int>::apply(val, std::string("FuncName"), 1);

	char c;
	std::cin >> c;
	return 0;
}


/*
class A
{

public:

	virtual void doMethod();
};

class B 
	: public A
{

public:

	void doMethod() override
	{
		std::cout << "class B::doMethod" << std::endl;
	}
};

class C 
	: public A
{

public:

	void doMethod() override
	{
		std::cout << "class C::doMethod" << std::endl;
	}
};

template<typename... Items>
class Container
{

public:

	Container(Items)
	{
	}

private:

	std::vector<std::make_unique<>>
};

class ServiceContainer
{

public:

    template<typename... Items>
    ServiceContainer( Items&&... items ) 
    : items_()
    {
		std::cout << "I'm here";

        std::unique_ptr<A> itemArr[] = 
		{
			std::make_unique<Items>( std::move(items))... 
		};

        items_ = std::vector< std::unique_ptr<A> > 
		{
			std::make_move_iterator(std::begin(itemArr)), 
				std::make_move_iterator(std::end(itemArr))
		};

		for ( auto & rItem : items_ )
			rItem->doMethod();
    }

	~ServiceContainer()
	{
		for ( auto & rItem : items_ )
			rItem->doMethod();
	}

private:

    std::vector<std::unique_ptr<A>> items_;
};

template <class ...Args>
void foo(Args... args)
{
    foo_imp(any(args)...);  //pass each arg to any constructor, and call foo_imp with resulting any objects
}

class Item
{
	virtual void DoMethod() = 0;
};


class Container
{
    template<typename... Items>
    Container( Items&&... items ) 
    : items_()
    {
        std::unique_ptr<Item> itemArr[] = 
		{
			std::make_unique<Items>(items)...
		};

        items_ = std::vector< std::unique_ptr<Item> > 
		{ 
			std::make_move_iterator(std::begin(itemArr)), std::make_move_iterator(std::end(itemArr))
		};

		for ( auto & rItem : items_ )
			rItem->DoMethod();
    }

	~Container()
	{
		for ( auto & rItem : items_ )
			rItem->DoMethod();	
	}

    std::vector< std::unique_ptr<Item> > items_;
};

struct A 
	: Item 
{ 
	A(float)
	{
	}

	virtual void DoMethod() override
	{
		std::cout << "A::DoMethod" << std::endl;
	}
};

struct B 
	: Item 
{  
	virtual void DoMethod() override
	{
		std::cout << "B::DoMethod" << std::endl;
	}
};

struct C 
	: B    
{ 
	C(float){} 

	virtual void DoMethod() override
	{
		std::cout << "C::DoMethod" << std::endl;
	}
};

struct D 
	: Item 
{ 
	D(float){} 

	virtual void DoMethod() override
	{
		std::cout << "D::DoMethod" << std::endl;
	}
};
*/
