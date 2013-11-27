#include "DirtBlock.h"

#include <stdio.h>
#include <iostream>

void DirtBlock::onCreate()
{
	std::cout << "onCreate" << std::endl;
}
void DirtBlock::onDestroy(int blockX, int blockY, int blockZ)
{
	std::cout << "onDestroy" << std::endl;
}
void DirtBlock::onBlockUpdate()
{
	std::cout << "onBlockUpdate" << std::endl;
}