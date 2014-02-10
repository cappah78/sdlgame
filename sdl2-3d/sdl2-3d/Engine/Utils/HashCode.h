#pragma once

class HashCode
{
public:
	HashCode(const char* string) : hash(calculateCRC64(string)) {};
	~HashCode() {};

	const unsigned long hash;
private:

	class CRCBuilder
	{
	public:
		CRCBuilder();

		void AddByte(char data);
		void AddBytes(const char* data, unsigned int dataSize);
		unsigned long GetCRC64() const;
	private:
		unsigned long m_CRC;
	};

	static unsigned long calculateCRC64(const char* string);
};
