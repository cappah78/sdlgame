print("GenTest.lua")

local ids = { Blocks.StoneBlock.id, Blocks.DirtBlock.id, 
			Blocks.CobbleStoneBlock.id, Blocks.CoalOreBlock.id, Blocks.IronOreBlock.id, 
			Blocks.GoldOreBlock.id, Blocks.DiamondOreBlock.id }

math.randomseed(9)

for i = 0, 10000 do
		local idx = math.random(1, table.getn(ids))
		World.setBlock(ids[idx], math.random(0, 50), math.random(0, 10), math.random(0, 50))
end


for i = 0, 500 do
		World.setBlock(Blocks.GravelBlock.id, math.random(0, 50), math.random(20, 40), math.random(0, 50))
end
