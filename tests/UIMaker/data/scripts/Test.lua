-- MAIN TODO: add a button that changes the color to the specified in the text box and a output box
-- giving the history of all the color changes

--[[
The way this works uses too much memory and probably cpu when we could make it use less.
TODO: Look for better possibilities and change how this works.

For now I'm thinking that having the event functions are going to take too much.
Because they have a logic defined by their name, like onResize we know it will be used
when resizing the panel, what we could do is: event of type onResize takes a table of names
and a lua string, whose would be get on the C-side and every object that contains that name
will be updated with that lua string that all it does is get that object and pass to the lua
string with a fixed name and what the string returns is the new value of the object position.

Makes sense? I think I wrote some nonsense but I'm too tired to do anything now.

HOWEVER, WE WILL NOT GIVE UP ON LUA! It's a fun idea to have a fully customizable interface
and even the event functions being able to be changed if the user wants to add/remove something
]]

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
    changeColorButton = nil,
    kysButton = nil,
    panel = nil,
}

-- Using 800x600 as base
interface.panel = Meatball.UI.dynamicPanel()
interface.panel.rect = Meatball.rect(0, 0, Game.viewport.x, Game.viewport.y)
interface.panel.config.color = randomColor(Meatball.color(0,0,0,255))

interface.changeColorButton = Meatball.UI.button()
interface.changeColorButton.config.color = Meatball.color(20, 20, 20, 255)
interface.changeColorButton.config.hoveredColor = Meatball.color(40, 40, 40, 255)

interface.changeColorButton.onRelease = function()
    interface.panel.config.color = randomColor(interface.panel.config.color)
    --interface.panel.config.color = stringToColor(interface.colorInputBox.getText())

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

    interface.changeColorButton.rect.width = interface.panel.rect.width * 0.06 -- 48px
    interface.changeColorButton.rect.height = interface.panel.rect.height * 0.02 -- 12px

    interface.kysButton.rect.width = interface.changeColorButton.rect.width
    interface.kysButton.rect.height = interface.changeColorButton.rect.height

    interface.panel.onMove()

    interface.panel.config.minSize.x = -- get the furthest x position in the panel
        interface.changeColorButton.rect.x+
        interface.changeColorButton.rect.width

    interface.panel.config.minSize.y = -- get the furthest y position in the panel
        interface.kysButton.rect.y+
        interface.kysButton.rect.height

    return interface
end

interface.panel.onMove = function()
    interface.changeColorButton.rect.x = interface.panel.rect.x + (interface.panel.rect.width - interface.changeColorButton.rect.width) * 0.5
    interface.changeColorButton.rect.y = interface.panel.rect.y + (interface.panel.rect.height - interface.changeColorButton.rect.height) * 0.5

    interface.kysButton.rect.x = interface.changeColorButton.rect.x
    interface.kysButton.rect.y = interface.changeColorButton.rect.y + interface.changeColorButton.rect.height + 1

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