--[[
THIS FILE SHOULD NOT BE INCLUDED IN YOUR LUA CODE!
All it does is give definitions for lua intellisense.
]]

Meatball = {
    Input = {},
    Console = {},
    Utils = {}
}

Game = {
    viewport = {x=0, y=0}
}

function Meatball.rect(x, y, width, height) return {x=x, y=y, width=width, height=height} end
function Meatball.vec2(x, y) return {x=x, y=y} end
function Meatball.color(r, g, b, a) return {r=r, g=g, b=b, a=a} end

function Meatball.Input.bind(name, callback) end
function Meatball.Input.unbind(name) end

function Meatball.Console.run(input) end

function Meatball.Utils.drawRect(rect, color) end

function Meatball.Utils.drawX(rect, color) end

function Meatball.Utils.drawText(font, height, text, x, y, color) end