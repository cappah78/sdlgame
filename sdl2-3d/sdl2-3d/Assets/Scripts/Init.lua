require "lfs"	--LuaFileSystem

Blocks = {}
Init = {}

Init.registerBlocks = function(folder)
	for file in lfs.dir(folder) do
		if file ~= "." and file ~= ".." then	-- TODO: fix, for some reason there is a "." and ".." listed
			World.registerBlockType(file)
		end
	end
end