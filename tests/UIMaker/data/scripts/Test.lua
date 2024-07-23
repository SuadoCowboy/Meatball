-- MAIN TODO: create a dynamic panel that have a random color every time we run.
-- also add a button that changes the color to the specified in the text box and a output box
-- giving the history of all the color changes

local function randomColor(colorBefore)
    local color = Meatball.color(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255)
    
    while color.r+color.g+color.b == 0 or
          color.r+color.g+color.b == 765 or
          (color.r == colorBefore.r and
          color.g == colorBefore.g and
          color.b == colorBefore.b) do
        color = Meatball.color(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255)
    end

    return color
end

local interface = {
    printHailoButton = nil,
    kysButton = nil,
    panel = nil,
}

-- Using 800x600 as base
interface.panel = Meatball.UI.dynamicPanel()
interface.panel.rect = Meatball.rect(0, 0, Game.viewport.x, Game.viewport.y)
interface.panel.config.color = Meatball.color(10, 10, 10, 255)

interface.printHailoButton = Meatball.UI.button()
interface.printHailoButton.config.color = Meatball.color(20, 20, 20, 255)
interface.printHailoButton.config.hoveredColor = Meatball.color(40, 40, 40, 255)

interface.printHailoButton.onRelease = function()
    Meatball.Console.run("echo Hailooo >.<")
    interface.panel.config.color = randomColor(interface.panel.config.color)

    return interface
end

interface.kysButton = Meatball.UI.button()
interface.kysButton.config.color = Meatball.color(255, 0, 0, 255)
interface.kysButton.config.hoveredColor = Meatball.color(0, 255, 0, 255)

interface.kysButton.onRelease = function()
    Meatball.Console.run("toggle_local_console 1; echo keep yourself safe!");
end

interface.panel.onResize = function()
    interface.panel.config.grabHeight = interface.panel.rect.height * 0.004
    if interface.panel.config.grabHeight < 0 then
        interface.panel.config.grabHeight = 1
    end

    interface.printHailoButton.rect.width = interface.panel.rect.width * 0.06 -- 48px
    interface.printHailoButton.rect.height = interface.panel.rect.height * 0.02 -- 12px

    interface.kysButton.rect.width = interface.printHailoButton.rect.width
    interface.kysButton.rect.height = interface.printHailoButton.rect.height

    interface.panel.onMove()

    interface.panel.config.minSize.x = -- get the furthest x position in the panel
        interface.printHailoButton.rect.x+
        interface.printHailoButton.rect.width

    interface.panel.config.minSize.y = -- get the furthest y position in the panel
        interface.kysButton.rect.y+
        interface.kysButton.rect.height

    return interface
end

interface.panel.onMove = function()
    interface.printHailoButton.rect.x = interface.panel.rect.x + (interface.panel.rect.width - interface.printHailoButton.rect.width) * 0.5
    interface.printHailoButton.rect.y = interface.panel.rect.y + (interface.panel.rect.height - interface.printHailoButton.rect.height) * 0.5

    interface.kysButton.rect.x = interface.printHailoButton.rect.x
    interface.kysButton.rect.y = interface.printHailoButton.rect.y + interface.printHailoButton.rect.height + 1

    return interface
end

interface.panel.onResize()

return Meatball.UI.LayoutTypes.NONE, interface
--[[
local rect = Meatball.rect(10, 10, 100, 100)
local dir = Meatball.vec2(1, 1)

local velocity = Meatball.vec2(300, 200)

local color = Meatball.color(255,0,0,255)

function OnResize(ratio)
    velocity.x = velocity.x * ratio.x
    velocity.y = velocity.y * ratio.y

    rect.x = rect.x * ratio.x
    rect.y = rect.y * ratio.y

    rect.width = rect.width * ratio.x
    rect.height = rect.height * ratio.y
end]]