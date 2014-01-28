Blocks.StoneBlock = {
	type = "defaultblock",
	texture = {
		top = "Assets\\Textures\\MinecraftBlocks\\stone.png",
		bottom = "Assets\\Textures\\MinecraftBlocks\\stone.png",
		front = "Assets\\Textures\\MinecraftBlocks\\stone.png",
		back = "Assets\\Textures\\MinecraftBlocks\\stone.png",
		left = "Assets\\Textures\\MinecraftBlocks\\stone.png",
		right = "Assets\\Textures\\MinecraftBlocks\\stone.png"
	},

	blockUpdate = function (block)
		print("stoneblock update: " .. block.x .. ":" .. block.y .. ":".. block.z)
	end,
}