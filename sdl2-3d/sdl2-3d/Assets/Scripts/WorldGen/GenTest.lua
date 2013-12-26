print("GenTest.lua")

local ids = { Blocks.StoneBlock.id, Blocks.DirtBlock.id, Blocks.GravelBlock.id, 
			Blocks.CobbleStoneBlock.id, Blocks.CoalOreBlock.id, Blocks.IronOreBlock.id, 
			Blocks.GoldOreBlock.id, Blocks.DiamondOreBlock.id }

for i = 0, 100 do
	local randX = math.random(0, 10)
	local randY = math.random(0, 10)
	local randZ = math.random(0, 10)
	local idx = math.random(1, table.getn(ids))

	World.setBlock(ids[idx], randX, randY, randZ)
end