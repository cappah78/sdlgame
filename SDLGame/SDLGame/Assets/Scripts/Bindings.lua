--[[
Script linking the c++ engine and the lua scripts together.
]]--

World = {
	registerBlockType = function (blockFilePath)
		CPP_World.registerBlockType(blockFilePath)
	end,

	setBlock = function (blockID, x, y, z, initialData)
		if initialData == nil then
			CPP_World.setBlock(blockID, x, y, z)
		else
			CPP_World.setBlockD(blockID, x, y, z, initialData)
		end
	end,

	getBlock = function (x, y, z)
		local blockID = CPP_World.getBlock(x, y, z)
		return blockID
	end,
}
