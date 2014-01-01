#ifndef VOID_DATA_LIST_H_
#define VOID_DATA_LIST_H_

#include <vector>

class VoidDataList
{
public:
	VoidDataList(unsigned int initialSize = 0);
	~VoidDataList();

	/** copy the data to the list, returning its start index */
	unsigned int add(void* data, unsigned int size);

	/** Return the data at position, make sure to static_cast to avoid byte endianness errors */
	inline void* const get(unsigned int position) const
	{
#ifdef _DEBUG
		return (void*) &m_data.at(position);
#else
		return (void*) &m_data[position];
#endif //_DEBUG
	};

	/** 
	remove size bytes from the list at position, moving all subsequent data to the left by size, 
	!-- READ: all values returned by add must have size substracted to keep the position matched up --!
	*/
	void remove(unsigned int position, unsigned int size);

	inline unsigned int getSize() const
	{
		return m_data.size();
	}

private:
	std::vector<unsigned char> m_data;
};

#endif //VOID_DATA_LIST_H_
