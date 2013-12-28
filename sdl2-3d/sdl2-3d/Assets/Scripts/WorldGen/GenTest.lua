print("GenTest.lua")

local ids = { Blocks.StoneBlock.id, Blocks.DirtBlock.id, Blocks.GravelBlock.id, 
			Blocks.CobbleStoneBlock.id, Blocks.CoalOreBlock.id, Blocks.IronOreBlock.id, 
			Blocks.GoldOreBlock.id, Blocks.DiamondOreBlock.id }


for i = 0, 70000 do
		local idx = math.random(1, table.getn(ids))
		World.setBlock(ids[idx], math.random(0, 50), math.random(0, 50), math.random(0, 50))
end


for x = -10, 0 do
	for z = -6, 0 do
		local idx = math.random(1, table.getn(ids))
		World.setBlock(ids[idx], x, 0, z)
	end
end

World.setBlock(1, -3, 1, -3);
World.setBlock(1, -3, -1, -3);

World.setBlock(1, -4, 1, -3);
World.setBlock(1, -4, 1, -4);
World.setBlock(1, -4, 1, -2);
World.setBlock(1, -4, 1, -5);
World.setBlock(1, -5, 1, -3);
World.setBlock(1, -6, 3, -6);
World.setBlock(1, -7, 3, -6);
World.setBlock(1, -7, 2, -6);
World.setBlock(1, -8, 1, -3);
World.setBlock(1, -8, 1, -2);
World.setBlock(1, -9, 1, -2);
