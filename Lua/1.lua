function factory1(_ENV)
	return function()
				return a
			end
end

f1 = factory1{a = 6}
f2 = factory1{a = 7}
print(f1())
print(f2())