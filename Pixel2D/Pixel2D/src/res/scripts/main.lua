require "externals.vector2"

prevPos = Vector2:new(0.0, 0.0)
movementSpeed = 250.0

-- Called once
function onStart()
  obj = Object.new()
  obj:get("Circle")
  currPos = Vector2:new(obj:getPosX(), obj:getPosY())
  prevPos:copy(currPos)
end

-- Called when timestep update is required
function onInput(dt)
  prevPos:copy(currPos)
  
  -- Move circle
  if (keyboard.isKeyPressed("W")) then
    currPos.y = currPos.y + movementSpeed * dt
  end
  if (keyboard.isKeyPressed("S")) then
    currPos.y = currPos.y - movementSpeed * dt
  end
  if (keyboard.isKeyPressed("A")) then
    currPos.x = currPos.x - movementSpeed * dt
  end
  if (keyboard.isKeyPressed("D")) then
    currPos.x = currPos.x + movementSpeed * dt
  end
end

-- Called every frame
function onUpdate(alpha)
  local target = lerp(prevPos, currPos, alpha)
  obj:setPosition(target.x, target.y)
end


