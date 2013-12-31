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

	perBlock = {			--data unique to every block
		health = "int(5)",	--name = "type(initialvalue)"
		derp = "bool(false)",
		someFloat = "float(2.051)"
	},

	process = function (block)
		print(block.someFloat)
	end
}