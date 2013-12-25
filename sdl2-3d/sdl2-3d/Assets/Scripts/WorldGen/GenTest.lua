
print("gentest")
print(Blocks.DirtBlock.id)

for x = 0, 150 do
	for z = 0, 150 do
		if x % 2 == 0 then
			World.setBlock(Blocks.StoneBlock.id, x, 0, z)
		else
			World.setBlock(Blocks.DirtBlock.id, x, 0, z)
		end
	end
end