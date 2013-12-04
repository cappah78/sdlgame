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
	toolType = { "shovel", "fist" },
	strength = 10,

	onCreate = function(x, y, z)
		print("onCreate: " .. id .. ":" .. x .. ":" .. y .. ":" .. z)
	end
}