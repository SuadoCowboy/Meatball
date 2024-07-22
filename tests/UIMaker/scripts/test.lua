Meatball.Input.bind("w", "echo da")

local rect = Meatball.rect(10, 10, 100, 100)
local dir = Meatball.vec2(1, 1)

local velocity = Meatball.vec2(300, 200)

local color = Meatball.color(255,0,0,255)

local function randomColor(colorBefore)
    color = Meatball.color(math.random(0, 1) * 255, math.random(0, 1) * 255, math.random(0, 1) * 255, 255)
    while color.r+color.g+color.b == 0 or
          color.r+color.g+color.b == 765 or
          (color.r == colorBefore.r and
          color.g == colorBefore.g and
          color.b == colorBefore.b) do
        color = Meatball.color(math.random(0, 1) * 255, math.random(0, 1) * 255, math.random(0, 1) * 255, 255)
    end

    return color
end

function OnResize(ratio)
    velocity.x = velocity.x * ratio.x
    velocity.y = velocity.y * ratio.y

    rect.x = rect.x * ratio.x
    rect.y = rect.y * ratio.y

    rect.width = rect.width * ratio.x
    rect.height = rect.height * ratio.y
end

function Update(dt)
    if dt > 0.1 then
        print(dt)
    end
    
    rect.x = rect.x + velocity.x * dir.x * dt
    rect.y = rect.y + velocity.y * dir.y * dt

    if rect.x+rect.width >= Game.viewport.x then
        dir.x = -1
        color = randomColor(color)
    elseif rect.x <= 0 then
        dir.x = 1
        color = randomColor(color)
    end

    if rect.y+rect.height >= Game.viewport.y then
        dir.y = -1
        color = randomColor(color)
    elseif rect.y <= 0 then
        dir.y = 1
        color = randomColor(color)
    end
end

function Draw()
    Meatball.Utils.drawRect(rect, color)
end