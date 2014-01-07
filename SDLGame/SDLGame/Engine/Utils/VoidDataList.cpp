#include "VoidDataList.h"

#include <memory>
#include <assert.h>

VoidDataList::VoidDataList(unsigned int initialSize)
	: m_data(initialSize)
{

}

VoidDataList::~VoidDataList()
{

}

unsigned int VoidDataList::add(void* data, unsigned int size)
{
	unsigned int position = m_data.size();
	m_data.insert(m_data.end(), (unsigned char*) data, (unsigned char*) data + size);

	return position;
}

void VoidDataList::remove(unsigned int position, unsigned int size)
{
	//! shifts all elements after position to the left with size (intentional)
	m_data.erase(m_data.begin() + position, m_data.begin() + position + size);
}
