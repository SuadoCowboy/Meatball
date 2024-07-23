-- MAIN TODO: create a dynamic panel that have a random color every time we run.
-- also add a button that changes the color to the specified in the text box and a output box
-- giving the history of all the color changes

-- CURRENT TODO: create a working button

local interface = {
    myButton1 = nil,
    myButton2 = nil
}

interface.myButton1 = Meatball.UI.button()

-- Using 800x600 as base
interface.myButton1.rect.width = Game.viewport.x * 0.06 -- 48px
interface.myButton1.rect.height = Game.viewport.y * 0.02 -- 12px

interface.myButton1.rect.x = (Game.viewport.x - interface.myButton1.rect.width) * 0.5
interface.myButton1.rect.y = (Game.viewport.y - interface.myButton1.rect.height) * 0.5
interface.myButton1.onRelease = function() print("onRelease called on myButton1!") end
interface.myButton1.onHover = function() print("onHover called on myButton1!") end

interface.myButton1.config.color = Meatball.color(20, 20, 20, 255)
interface.myButton1.config.hoveredColor = Meatball.color(40, 40, 40, 255)

interface.myButton1.rect.width = Game.viewport.x * 0.06 -- 48px
interface.myButton1.rect.height = Game.viewport.y * 0.04 -- 24px

interface.myButton2 = Meatball.UI.button()

interface.myButton2.rect.width = interface.myButton1.rect.width
interface.myButton2.rect.height = interface.myButton1.rect.height

interface.myButton2.rect.x = interface.myButton1.rect.x
interface.myButton2.rect.y = interface.myButton1.rect.y + interface.myButton1.rect.height + 1

interface.myButton2.onRelease = function() print("onRelease called on myButton2!") end
interface.myButton2.onHover = function() print("onHover called on myButton2!") end

interface.myButton2.config.color = Meatball.color(255, 0, 0, 255)
interface.myButton2.config.hoveredColor = Meatball.color(0, 255, 0, 255)

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