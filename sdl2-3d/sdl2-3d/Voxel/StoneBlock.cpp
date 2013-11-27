#include "StoneBlock.h"

#include <stdio.h>
#include <iostream>

/*
void StoneBlock::onCreate()
{
	std::cout << "onCreate: " << std::endl;
}
*/
void StoneBlock::onPlace()
{
	std::cout << "onPlace" << std::endl;
}
void StoneBlock::onDestroy(int blockX, int blockY, int blockZ)
{
	std::cout << "onDestroy" << std::endl;
}
void StoneBlock::onBlockUpdate()
{
	std::cout << "onBlockUpdate" << std::endl;
}