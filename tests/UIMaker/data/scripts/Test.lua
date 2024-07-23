-- MAIN TODO: create a dynamic panel that have a random color every time we run.
-- also add a button that changes the color to the specified in the text box and a output box
-- giving the history of all the color changes

-- CURRENT TODO: create a working button

local interface = {
    myButton = nil
}

interface.myButton = Meatball.UI.button()

-- Using 800x600 as base
interface.myButton.rect.width = Game.viewport.x * 0.06 -- 48px
interface.myButton.rect.height = Game.viewport.y * 0.04 -- 24px

interface.myButton.rect.x = (Game.viewport.x - interface.myButton.rect.width) * 0.5
interface.myButton.rect.y = (Game.viewport.y - interface.myButton.rect.height) * 0.5
interface.myButton.onRelease = function() print("onRelease called!") end
interface.myButton.onHover = function() print("onHover called!") end

interface.myButton.config.color = Meatball.color(20, 20, 20, 255)
interface.myButton.config.hoveredColor = Meatball.color(40, 40, 40, 255)

return Meatball.UI.LayoutTypes.NONE, interface
--[[
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
end]]