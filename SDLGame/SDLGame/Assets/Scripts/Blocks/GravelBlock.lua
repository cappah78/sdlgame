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

	perBlock = {						--data unique to every block: name = "type(initialvalue)"
		fallIntervalTicks = "int(5)"
	},

	process = function (block)
		block.fallIntervalTicks = block.fallIntervalTicks - 1;

		if block.y <= -10 
		then
			World.setBlock(0, block.x, block.y, block.z)
			World.setBlock(Blocks.GravelBlock.id, block.x, 20, block.z)
		elseif block.fallIntervalTicks <= 0 
		then
			if World.getBlock(block.x, block.y - 1, block.z) == Blocks.AirBlock.id -- if block below is air
			then	
				World.setBlock(0, block.x, block.y, block.z)
				World.setBlock(Blocks.GravelBlock.id, block.x, block.y - 1, block.z)
			end
		end
	end,

	events = {
		{
			trigger = "y < 0",
			event = function (block)
				
			end
		},
		{
			trigger = "tickCount == 100",
			event = function (block)

			end
		}
	}
}