Blocks.WaterBlock = {
	type = "defaultblock",
	texture = {
		top = "Assets\\Textures\\MinecraftBlocks\\water.png",
		bottom = "Assets\\Textures\\MinecraftBlocks\\water.png",
		front = "Assets\\Textures\\MinecraftBlocks\\water.png",
		back = "Assets\\Textures\\MinecraftBlocks\\water.png",
		left = "Assets\\Textures\\MinecraftBlocks\\water.png",
		right = "Assets\\Textures\\MinecraftBlocks\\water.png"
	},

	blockUpdate = function (block)
		print("waterblock update: " .. block.x .. ":" .. block.y .. ":".. block.z)
	end,
}