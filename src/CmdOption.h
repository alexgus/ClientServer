/*
 * CmdOption.h
 *
 *  Created on: 5 juil. 2014
 *      Author: alexandre
 */

#ifndef CMDOPTION_H_
#define CMDOPTION_H_

#include <string>

using namespace std;

/**
 * Define an option to a command
 */
class CmdOption
{
public:

	/**
	 * Default constructor
	 */
	CmdOption();

	/**
	 * Destroyer of the class
	 */
	virtual ~CmdOption();

	/**
	 * Get the option string
	 * @return The option string
	 */
	const string& getOption() const
	{
		return option;
	}

	/**
	 * Set the option string
	 * @param option the option string
	 */
	void setOption(const string& option)
	{
		this->option = option;
	}

	/**
	 * Get the value string
	 * @return The value string
	 */
	const string& getValue() const
	{
		return value;
	}

	/**
	 * Set the value string
	 * @param value the value string
	 */
	void setValue(const string& value)
	{
		this->value = value;
	}

	/**
	 * Check if there is a value to the option
	 */
	bool valueExist();

private:

	/**
	 * Option string
	 */
	string option;

	/**
	 * Value of the option (can be NULL)
	 */
	string value;
};

#endif /* CMDOPTION_H_ */
