require "lfs"	--LuaFileSystem

Blocks = {
	AirBlock = {
		id = 0	--should always be 0, hardcoded in c++ for performance.
	}
}

TicksPerSec = 20					--The number of times per second process will be called on every block that has it.
TickDuration = 1000 / TicksPerSec
TickDurationSec = 1 / TicksPerSec

Init = {
	registerBlocks = function(folder) -- iterates every file in the given folder and calls World.registerBlockType(path)
		for file in lfs.dir(folder) do
			if file ~= "." and file ~= ".." then	-- removes . and .. directory things
				World.registerBlockType(file)
			end
		end
	end
}



print("Initializing, TicksPerSec: ".. TicksPerSec .. ", TickDuration: " .. TickDuration .. ", TickDurationSec: " .. TickDurationSec)

