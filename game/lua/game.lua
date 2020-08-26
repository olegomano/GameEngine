

local renderable = nil;

local items = {}
local xMin  = -3.14*1
local xMax  =  3.14*1
local count = 1000
local step  = (xMax - xMin)/count 
local function onInit()
  print("Lua Init")
  for i=0,count do
    --print(i)
    r = create({ 
      y=math.sin(xMin + step*i,0),
      x=-1 + i*2/count, 
      scale=math.sin(i/100) / 12
    })
    --r = create(-1 + 1*2/count,-1+1*2/count,0)
    items[i] = r
  end
end

local time = 0
local function onFrame() 
  time = time + 0.032
  for x,drawable in ipairs(items) do
    drawable['y'] = math.sin(time + xMin + step*x + math.cos(time))*math.cos(time)*math.sin(time) 
  end
end

return {
  init  = onInit,
  frame = onFrame
}
