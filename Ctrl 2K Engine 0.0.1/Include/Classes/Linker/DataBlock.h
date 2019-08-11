#pragma once

class DataBlock
{
public:
	DataBlock(std::string &data);
	virtual ~DataBlock();

public:
	void updateDataType();

	void changeData(std::string &data);
	void clear();

	std::string m_data;
	char m_datatype = ' '; //f = float, i = int, u = unsigned int, k = keyword, n = in need of further processing, c = char, ' ' = null
};