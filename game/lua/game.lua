

local renderable = nil;

local items = {}
local xMin  = -3.14*10
local xMax  =  3.14*10
local count = 100
local step  = (xMax - xMin)/count 
local function onInit()
  print("Lua Init")
  for i=0,count do
    --print(i)
    r = create(math.sin(xMin + step*i,0),-1 + i*2/count)
    --r = create(-1 + 1*2/count,-1+1*2/count,0)
    items[i] = r
  end
end

local time = 0
local function onFrame() 
  time = time + 0.0032
  for x,drawable in ipairs(items) do
    drawable['y'] = math.sin(time + xMin + step*x + math.cos(time))*math.cos(time)*math.sin(time) 
  end
end

return {
  init  = onInit,
  frame = onFrame
}
