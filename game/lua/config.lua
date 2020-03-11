

local function Init()
  createWindow("Window",400,400)
  createWindow("Window 2",400,400)
  createWindow("Window 3",400,400)
  loadScript("/home/oleg/Documents/projects/galacoli/game/lua/game.lua")
  loadScript("/home/oleg/Documents/projects/galacoli/game/lua/game2.lua")
end

local function Frame()
end

return {
  init = Init,
  frame = Frame
}
