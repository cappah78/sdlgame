Blocks.GravelBlock = {
	type = "defaultblock",	--normal, unanimated block with just 6 textured faces.

	texture = {
		top = "Assets\\Textures\\MinecraftBlocks\\gravel.png",
		bottom = "Assets\\Textures\\MinecraftBlocks\\gravel.png",
		front = "Assets\\Textures\\MinecraftBlocks\\gravel.png",
		back = "Assets\\Textures\\MinecraftBlocks\\gravel.png",
		left = "Assets\\Textures\\MinecraftBlocks\\gravel.png",
		right = "Assets\\Textures\\MinecraftBlocks\\gravel.png"
	},

	events = {
		{
			trigger = "tickCount == 5",	--run every 5 ticks
			event = function (block)
				if World.getBlock(block.x, block.y - 1, block.z) == Blocks.AirBlock.id -- if block below is air
				then	
					World.setBlock(0, block.x, block.y, block.z)
					World.setBlock(Blocks.GravelBlock.id, block.x, block.y - 1, block.z)
				end
			end
		},
		{
			trigger = "y <= -5",
			event = function (block)
				World.setBlock(0, block.x, block.y, block.z)
				World.setBlock(Blocks.GravelBlock.id, block.x, 25, block.z)
			end
		}
	}
}