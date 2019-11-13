#pragma once

#include <iostream>
#include <string>
#include <cstdint>


enum class Error
{
		NoError,
		CorruptedArchive
};


class Serializer
{
	std::ostream& out_;
	static const char separator = ' ';

public:
	Serializer(std::ostream& out)
		: out_ (out)
	{}


	template <class... ArgsT>
	Error operator()(const ArgsT&... args)
	{
		return process<ArgsT...>(args...);
	}


	template <class T>
	Error save(const T& data)
	{
		return data.serialize(*this);
	}


private:
	template<class T>
	Error process(const T& last)
	{
		return save<T>(last);
	}

	template <class T, class... ArgsT>
	Error process(const T& next, const ArgsT&... args)
	{
		if (save(next) != Error::NoError) return Error::CorruptedArchive;

		return process<ArgsT...>(args...);
	}
};



class Deserializer
{
	std::istream& in_;

public:
	Deserializer(std::istream& in)
		: in_ (in)
	{}

	template <class... ArgsT>
	Error operator()(ArgsT&... args)
	{
		return process<ArgsT...>(args...);
	}

	template <class T>
	Error load(T& data)
	{
		return data.deserialize(*this);
	}


private:
	template<class T>
	Error process(T& last)
	{
		return load<T>(last);
	}

	template <class T, class... ArgsT>
	Error process(T& next, ArgsT&... args)
	{
		if (load<T>(next) != Error::NoError) return Error::CorruptedArchive;

		return process<ArgsT...>(args...);
	}
};



template <>
Error Serializer::save<uint64_t>(const uint64_t& data)
{
	out_ << data << separator;
	return Error::NoError;
}

template <>
Error Serializer::save<bool>(const bool& data)
{
	if (data) out_ << "true";
	else out_ << "false";
	out_ << separator;
	return Error::NoError;
}



template <>
Error Deserializer::load<uint64_t>(uint64_t& data)
{
    in_ >> data;
    if (in_.fail()) return Error::CorruptedArchive;
	return Error::NoError;
}

template <>
Error Deserializer::load<bool>(bool& data)
{
    std::string str;
    in_ >> str;

    if (str == "false") data = false;
    else if (str == "true") data = true;
    else return Error::CorruptedArchive;

    return Error::NoError;
}
