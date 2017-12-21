require "externals.vector2"

v = Vector2:new(2.0, 2.0)
r = v:add(Vector2:new(2.0, 2.0))
l = lerp(v, r, 0.5)
print(l.x)