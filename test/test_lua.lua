--system funtions creed by c++ appending them to the table 
--createObject('path') -> handle/object 
--createPrimitive(Tris|Cube|Plane|Sphere) -> handle/object

--figure out how to return objects into lua



local function onInit(context)

end

local function onFrame(context)

end

local function onDestroy(context)

end

local function onKeyboard(context,keystate)

end


local function tableTest(table)
    print(table["int"])
    table["int"] = table["int"] + table["int"]
end


local function test2(a,b,c,d)
    print(a)
    print(b)
    print(c)
    print(d)

    r = a + b + c + d
    print(r*2*10)
end

return {
    init      = onInit,
    onFrame   = onFrame,
    onKey     = onKeyboard,
    destroy   = onDestroy,
    test      = tableTest,
    int2      = "integer",
    kkk       = test2,
    scale     = 0.5,
    width     = 1200,
    height    = 800
}