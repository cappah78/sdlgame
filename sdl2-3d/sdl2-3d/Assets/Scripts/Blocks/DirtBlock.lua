Blocks.DirtBlock = {
	type = "defaultblock",
	texture = {
		top = "Assets\\Textures\\MinecraftBlocks\\dirt.png",
		bottom = "Assets\\Textures\\MinecraftBlocks\\dirt.png",
		front = "Assets\\Textures\\MinecraftBlocks\\dirt.png",
		back = "Assets\\Textures\\MinecraftBlocks\\dirt.png",
		left = "Assets\\Textures\\MinecraftBlocks\\dirt.png",
		right = "Assets\\Textures\\MinecraftBlocks\\dirt.png"
	},

	blockUpdate = function (block)
		print("dirtblock update: " .. block.x .. ":" .. block.y .. ":".. block.z)
	end,
}