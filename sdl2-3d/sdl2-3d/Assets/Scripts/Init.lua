require "lfs"	--LuaFileSystem

Blocks = {}
Init = {}

-- iterates every file in the given folder and calls World.registerBlockType(path)
Init.registerBlocks = function(folder)
	for file in lfs.dir(folder) do
		if file ~= "." and file ~= ".." then	-- removes . and .. directory things
			World.registerBlockType(file)
		end
	end
end