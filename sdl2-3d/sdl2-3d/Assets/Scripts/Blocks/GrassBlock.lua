Blocks.GrassBlock = {
	type = "defaultblock",
	texture = {
		top = "Assets\\Textures\\MinecraftBlocks\\grasstop.png",
		bottom = "Assets\\Textures\\MinecraftBlocks\\dirt.png",
		front = "Assets\\Textures\\MinecraftBlocks\\grassside.png",
		back = "Assets\\Textures\\MinecraftBlocks\\grassside.png",
		left = "Assets\\Textures\\MinecraftBlocks\\grassside.png",
		right = "Assets\\Textures\\MinecraftBlocks\\grassside.png"
	},

	blockUpdate = function (block)
		print("grassblock update: " .. block.x .. ":" .. block.y .. ":".. block.z)
	end,
}