#include "HashCode.h"

const Hash s_AllCRCBitsSet = 0xFFFFFFFFFFFFFFFFLL;

/** http://pages.pathcom.com/~vadco/crc.html */
const Hash s_CRC64Table[256] =
{
	0x0000000000000000LL, 0x42F0E1EBA9EA3693LL,
	0x85E1C3D753D46D26LL, 0xC711223CFA3E5BB5LL,
	0x493366450E42ECDFLL, 0x0BC387AEA7A8DA4CLL,
	0xCCD2A5925D9681F9LL, 0x8E224479F47CB76ALL,
	0x9266CC8A1C85D9BELL, 0xD0962D61B56FEF2DLL,
	0x17870F5D4F51B498LL, 0x5577EEB6E6BB820BLL,
	0xDB55AACF12C73561LL, 0x99A54B24BB2D03F2LL,
	0x5EB4691841135847LL, 0x1C4488F3E8F96ED4LL,
	0x663D78FF90E185EFLL, 0x24CD9914390BB37CLL,
	0xE3DCBB28C335E8C9LL, 0xA12C5AC36ADFDE5ALL,
	0x2F0E1EBA9EA36930LL, 0x6DFEFF5137495FA3LL,
	0xAAEFDD6DCD770416LL, 0xE81F3C86649D3285LL,
	0xF45BB4758C645C51LL, 0xB6AB559E258E6AC2LL,
	0x71BA77A2DFB03177LL, 0x334A9649765A07E4LL,
	0xBD68D2308226B08ELL, 0xFF9833DB2BCC861DLL,
	0x388911E7D1F2DDA8LL, 0x7A79F00C7818EB3BLL,
	0xCC7AF1FF21C30BDELL, 0x8E8A101488293D4DLL,
	0x499B3228721766F8LL, 0x0B6BD3C3DBFD506BLL,
	0x854997BA2F81E701LL, 0xC7B97651866BD192LL,
	0x00A8546D7C558A27LL, 0x4258B586D5BFBCB4LL,
	0x5E1C3D753D46D260LL, 0x1CECDC9E94ACE4F3LL,
	0xDBFDFEA26E92BF46LL, 0x990D1F49C77889D5LL,
	0x172F5B3033043EBFLL, 0x55DFBADB9AEE082CLL,
	0x92CE98E760D05399LL, 0xD03E790CC93A650ALL,
	0xAA478900B1228E31LL, 0xE8B768EB18C8B8A2LL,
	0x2FA64AD7E2F6E317LL, 0x6D56AB3C4B1CD584LL,
	0xE374EF45BF6062EELL, 0xA1840EAE168A547DLL,
	0x66952C92ECB40FC8LL, 0x2465CD79455E395BLL,
	0x3821458AADA7578FLL, 0x7AD1A461044D611CLL,
	0xBDC0865DFE733AA9LL, 0xFF3067B657990C3ALL,
	0x711223CFA3E5BB50LL, 0x33E2C2240A0F8DC3LL,
	0xF4F3E018F031D676LL, 0xB60301F359DBE0E5LL,
	0xDA050215EA6C212FLL, 0x98F5E3FE438617BCLL,
	0x5FE4C1C2B9B84C09LL, 0x1D14202910527A9ALL,
	0x93366450E42ECDF0LL, 0xD1C685BB4DC4FB63LL,
	0x16D7A787B7FAA0D6LL, 0x5427466C1E109645LL,
	0x4863CE9FF6E9F891LL, 0x0A932F745F03CE02LL,
	0xCD820D48A53D95B7LL, 0x8F72ECA30CD7A324LL,
	0x0150A8DAF8AB144ELL, 0x43A04931514122DDLL,
	0x84B16B0DAB7F7968LL, 0xC6418AE602954FFBLL,
	0xBC387AEA7A8DA4C0LL, 0xFEC89B01D3679253LL,
	0x39D9B93D2959C9E6LL, 0x7B2958D680B3FF75LL,
	0xF50B1CAF74CF481FLL, 0xB7FBFD44DD257E8CLL,
	0x70EADF78271B2539LL, 0x321A3E938EF113AALL,
	0x2E5EB66066087D7ELL, 0x6CAE578BCFE24BEDLL,
	0xABBF75B735DC1058LL, 0xE94F945C9C3626CBLL,
	0x676DD025684A91A1LL, 0x259D31CEC1A0A732LL,
	0xE28C13F23B9EFC87LL, 0xA07CF2199274CA14LL,
	0x167FF3EACBAF2AF1LL, 0x548F120162451C62LL,
	0x939E303D987B47D7LL, 0xD16ED1D631917144LL,
	0x5F4C95AFC5EDC62ELL, 0x1DBC74446C07F0BDLL,
	0xDAAD56789639AB08LL, 0x985DB7933FD39D9BLL,
	0x84193F60D72AF34FLL, 0xC6E9DE8B7EC0C5DCLL,
	0x01F8FCB784FE9E69LL, 0x43081D5C2D14A8FALL,
	0xCD2A5925D9681F90LL, 0x8FDAB8CE70822903LL,
	0x48CB9AF28ABC72B6LL, 0x0A3B7B1923564425LL,
	0x70428B155B4EAF1ELL, 0x32B26AFEF2A4998DLL,
	0xF5A348C2089AC238LL, 0xB753A929A170F4ABLL,
	0x3971ED50550C43C1LL, 0x7B810CBBFCE67552LL,
	0xBC902E8706D82EE7LL, 0xFE60CF6CAF321874LL,
	0xE224479F47CB76A0LL, 0xA0D4A674EE214033LL,
	0x67C58448141F1B86LL, 0x253565A3BDF52D15LL,
	0xAB1721DA49899A7FLL, 0xE9E7C031E063ACECLL,
	0x2EF6E20D1A5DF759LL, 0x6C0603E6B3B7C1CALL,
	0xF6FAE5C07D3274CDLL, 0xB40A042BD4D8425ELL,
	0x731B26172EE619EBLL, 0x31EBC7FC870C2F78LL,
	0xBFC9838573709812LL, 0xFD39626EDA9AAE81LL,
	0x3A28405220A4F534LL, 0x78D8A1B9894EC3A7LL,
	0x649C294A61B7AD73LL, 0x266CC8A1C85D9BE0LL,
	0xE17DEA9D3263C055LL, 0xA38D0B769B89F6C6LL,
	0x2DAF4F0F6FF541ACLL, 0x6F5FAEE4C61F773FLL,
	0xA84E8CD83C212C8ALL, 0xEABE6D3395CB1A19LL,
	0x90C79D3FEDD3F122LL, 0xD2377CD44439C7B1LL,
	0x15265EE8BE079C04LL, 0x57D6BF0317EDAA97LL,
	0xD9F4FB7AE3911DFDLL, 0x9B041A914A7B2B6ELL,
	0x5C1538ADB04570DBLL, 0x1EE5D94619AF4648LL,
	0x02A151B5F156289CLL, 0x4051B05E58BC1E0FLL,
	0x87409262A28245BALL, 0xC5B073890B687329LL,
	0x4B9237F0FF14C443LL, 0x0962D61B56FEF2D0LL,
	0xCE73F427ACC0A965LL, 0x8C8315CC052A9FF6LL,
	0x3A80143F5CF17F13LL, 0x7870F5D4F51B4980LL,
	0xBF61D7E80F251235LL, 0xFD913603A6CF24A6LL,
	0x73B3727A52B393CCLL, 0x31439391FB59A55FLL,
	0xF652B1AD0167FEEALL, 0xB4A25046A88DC879LL,
	0xA8E6D8B54074A6ADLL, 0xEA16395EE99E903ELL,
	0x2D071B6213A0CB8BLL, 0x6FF7FA89BA4AFD18LL,
	0xE1D5BEF04E364A72LL, 0xA3255F1BE7DC7CE1LL,
	0x64347D271DE22754LL, 0x26C49CCCB40811C7LL,
	0x5CBD6CC0CC10FAFCLL, 0x1E4D8D2B65FACC6FLL,
	0xD95CAF179FC497DALL, 0x9BAC4EFC362EA149LL,
	0x158E0A85C2521623LL, 0x577EEB6E6BB820B0LL,
	0x906FC95291867B05LL, 0xD29F28B9386C4D96LL,
	0xCEDBA04AD0952342LL, 0x8C2B41A1797F15D1LL,
	0x4B3A639D83414E64LL, 0x09CA82762AAB78F7LL,
	0x87E8C60FDED7CF9DLL, 0xC51827E4773DF90ELL,
	0x020905D88D03A2BBLL, 0x40F9E43324E99428LL,
	0x2CFFE7D5975E55E2LL, 0x6E0F063E3EB46371LL,
	0xA91E2402C48A38C4LL, 0xEBEEC5E96D600E57LL,
	0x65CC8190991CB93DLL, 0x273C607B30F68FAELL,
	0xE02D4247CAC8D41BLL, 0xA2DDA3AC6322E288LL,
	0xBE992B5F8BDB8C5CLL, 0xFC69CAB42231BACFLL,
	0x3B78E888D80FE17ALL, 0x7988096371E5D7E9LL,
	0xF7AA4D1A85996083LL, 0xB55AACF12C735610LL,
	0x724B8ECDD64D0DA5LL, 0x30BB6F267FA73B36LL,
	0x4AC29F2A07BFD00DLL, 0x08327EC1AE55E69ELL,
	0xCF235CFD546BBD2BLL, 0x8DD3BD16FD818BB8LL,
	0x03F1F96F09FD3CD2LL, 0x41011884A0170A41LL,
	0x86103AB85A2951F4LL, 0xC4E0DB53F3C36767LL,
	0xD8A453A01B3A09B3LL, 0x9A54B24BB2D03F20LL,
	0x5D45907748EE6495LL, 0x1FB5719CE1045206LL,
	0x919735E51578E56CLL, 0xD367D40EBC92D3FFLL,
	0x1476F63246AC884ALL, 0x568617D9EF46BED9LL,
	0xE085162AB69D5E3CLL, 0xA275F7C11F7768AFLL,
	0x6564D5FDE549331ALL, 0x279434164CA30589LL,
	0xA9B6706FB8DFB2E3LL, 0xEB46918411358470LL,
	0x2C57B3B8EB0BDFC5LL, 0x6EA7525342E1E956LL,
	0x72E3DAA0AA188782LL, 0x30133B4B03F2B111LL,
	0xF7021977F9CCEAA4LL, 0xB5F2F89C5026DC37LL,
	0x3BD0BCE5A45A6B5DLL, 0x79205D0E0DB05DCELL,
	0xBE317F32F78E067BLL, 0xFCC19ED95E6430E8LL,
	0x86B86ED5267CDBD3LL, 0xC4488F3E8F96ED40LL,
	0x0359AD0275A8B6F5LL, 0x41A94CE9DC428066LL,
	0xCF8B0890283E370CLL, 0x8D7BE97B81D4019FLL,
	0x4A6ACB477BEA5A2ALL, 0x089A2AACD2006CB9LL,
	0x14DEA25F3AF9026DLL, 0x562E43B4931334FELL,
	0x913F6188692D6F4BLL, 0xD3CF8063C0C759D8LL,
	0x5DEDC41A34BBEEB2LL, 0x1F1D25F19D51D821LL,
	0xD80C07CD676F8394LL, 0x9AFCE626CE85B507LL
};

inline unsigned long long CalculateCRCValue(char data, Hash crc)
{
	return s_CRC64Table[(static_cast<Hash>(crc) >> 56) ^ static_cast<unsigned char>(data)] ^ (crc << 8);
}

HashCode::CRCBuilder::CRCBuilder() :
	m_CRC(s_AllCRCBitsSet)
{

}

void HashCode::CRCBuilder::AddByte(char data)
{
	m_CRC = CalculateCRCValue(data, m_CRC);
}

void HashCode::CRCBuilder::AddBytes(const char* data, unsigned int dataSize)
{
	for (unsigned int pos = 0; pos != dataSize; ++pos)
	{
		m_CRC = CalculateCRCValue(data[pos], m_CRC);
	}
}

Hash HashCode::CRCBuilder::GetCRC64() const
{
	return m_CRC ^ s_AllCRCBitsSet;
}

Hash HashCode::calculateCRC64(const char* string)
{
	HashCode::CRCBuilder builder;
	for (unsigned int pos = 0; string[pos]; ++pos)
	{
		builder.AddByte(string[pos]);
	}
	return builder.GetCRC64();
}
