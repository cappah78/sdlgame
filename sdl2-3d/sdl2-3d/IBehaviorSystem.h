#ifndef IBEHAVIOR_SYSTEM_H_
#define IBEHAVIOR_SYSTEM_H_

class IBehaviorSystem
{
public:
	IBehaviorSystem();
	virtual ~IBehaviorSystem();
private:

	void setEnabled(bool enabled);
	void update(float deltaSec);



	bool isEnabled;
};

#endif //IBEHAVIOR_SYSTEM_H_