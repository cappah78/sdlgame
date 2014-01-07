@ECHO OFF
SETLOCAL
SET LUA_PATH=C:\Users\Enrico\Documents\GitHub\sdlgame\sdl2-3d\2.1\lua\?.lua;C:\Users\Enrico\Documents\GitHub\sdlgame\sdl2-3d\2.1\lua\?\init.lua;%LUA_PATH%
IF NOT "%LUA_PATH_5_2%"=="" (
   SET LUA_PATH_5_2=C:\Users\Enrico\Documents\GitHub\sdlgame\sdl2-3d\2.1\lua\?.lua;C:\Users\Enrico\Documents\GitHub\sdlgame\sdl2-3d\2.1\lua\?\init.lua;%LUA_PATH_5_2%
)
SET PATH=C:\Users\Enrico\Documents\GitHub\sdlgame\sdl2-3d\2.1\;%PATH%
"lua5.1" "C:\Users\Enrico\Documents\GitHub\sdlgame\sdl2-3d\2.1\luarocks-admin.lua" %*
ENDLOCAL
