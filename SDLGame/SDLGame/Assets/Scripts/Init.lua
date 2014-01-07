require "lfs"	--LuaFileSystem
dofile ".\\Assets\\Scripts\\Bindings.lua"	--setup lua-cpp bindings

TicksPerSec = 20					--The number of times per second process will be called on every block that has it.
TickDuration = 1000 / TicksPerSec
TickDurationSec = 1 / TicksPerSec

Blocks = {
	AirBlock = {
		id = 0	--should always be 0, hardcoded in c++ for performance.
	}
}

local registerBlocks = function (folder) -- iterates every file in the given folder and calls World.registerBlockType(path)
	for file in lfs.dir(folder) do
		if file ~= "." and file ~= ".." then
			dofile(folder..file)				--run the block script so vars get initialized
			World.registerBlockType(file)		--register block c++ side
		end
	end
end

registerBlocks(".\\Assets\\Scripts\\Blocks\\")

