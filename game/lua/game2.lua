
local tris = nil
local function onInit()
  tris = create(0,0)
end

local time = 0
local function onFrame()
  time = time + 0.016
  tris['x'] = math.sin(time)
  tris['y'] = 0.5*math.cos(time)
end


return {
  init = onInit, 
  frame = onFrame
}
