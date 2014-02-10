#pragma once

typedef unsigned long long Hash;

class HashCode
{
public:
	HashCode(const char* string) : hash(calculateCRC64(string)) {};
	~HashCode() {};

	const Hash hash;
private:
	class CRCBuilder
	{
	public:
		CRCBuilder();

		void AddByte(char data);
		void AddBytes(const char* data, unsigned int dataSize);
		Hash GetCRC64() const;
	private:
		Hash m_CRC;
	};

	static Hash calculateCRC64(const char* string);
};
