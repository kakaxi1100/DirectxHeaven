wk = setmetatable({}, {__mode = "k"})
wv = setmetatable({},{__mode = "v"})

o = {}
wv[1] = o; wk[o] = 10

setmetatable(o, {__gc=function ( o )
	print(wk[o], wv[1])
end})

o = nil;
collectgarbage();-->10  nil