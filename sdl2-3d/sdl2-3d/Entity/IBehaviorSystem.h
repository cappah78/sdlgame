#ifndef IBEHAVIOR_SYSTEM_H_
#define IBEHAVIOR_SYSTEM_H_

#include<vector>

typedef unsigned int SystemID;

template <typename COMPONENT>
class IBehaviorSystem
{
public:
	IBehaviorSystem();
	virtual ~IBehaviorSystem();
private:

	void setEnabled(bool enabled);
	void update(float deltaSec);

	bool isEnabled;
	std::vector<COMPONENT> m_componentList;
};

template <typename COMPONENT1, typename COMPONENT2>
class IBehavior2System
{
public:
	IBehavior2System();
	virtual ~IBehavior2System();

	struct ComponentPair
	{
		COMPONENT1 component1;
		COMPONENT2 component2;
	};
private:

	void setEnabled(bool enabled);
	void update(float deltaSec);

	bool isEnabled;
	std::vector<ComponentPair> m_componentList;

};

#endif //IBEHAVIOR_SYSTEM_H_