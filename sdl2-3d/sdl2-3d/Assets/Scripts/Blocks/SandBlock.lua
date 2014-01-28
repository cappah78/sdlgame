Blocks.SandBlock = {
	type = "defaultblock",
	texture = {
		top = "Assets\\Textures\\MinecraftBlocks\\sand.png",
		bottom = "Assets\\Textures\\MinecraftBlocks\\sand.png",
		front = "Assets\\Textures\\MinecraftBlocks\\sand.png",
		back = "Assets\\Textures\\MinecraftBlocks\\sand.png",
		left = "Assets\\Textures\\MinecraftBlocks\\sand.png",
		right = "Assets\\Textures\\MinecraftBlocks\\sand.png"
	},

	perBlock = {
		thingy = "int(5)"
	},

	blockUpdate = function (block)
		print("sandblock update: " .. block.x .. ":" .. block.y .. ":".. block.z .. " thingy: " .. block.thingy)
		block.thingy = block.thingy - 1
	end,
}