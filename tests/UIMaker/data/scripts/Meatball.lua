--[[
THIS FILE SHOULD NOT BE INCLUDED IN YOUR LUA CODE!
All it does is give definitions for lua intellisense.
]]

Meatball = {
    Input = {},
    Console = {},
    UI = {
        LayoutTypes = {
            CONSOLE = 0,
            NONE = -1, -- NONE = #LayoutTypes
        },
        Types = {
            BUTTON = 1
        }
    }
}

Rect = {x=0, y=0, width=0, height=0}
Vec2 = {x=0, y=0}
Color = {r=0, g=0, b=0, a=0}

function Meatball.rect(x, y, width, height) return Rect end
function Meatball.vec2(x, y) return Vec2 end
function Meatball.color(r, g, b, a) return Color end

function Meatball.Input.bind(name, callback) end
function Meatball.Input.unbind(name) end

function Meatball.Console.run(input) end

function Meatball.UI.button() return {rect=Rect, config={color=Color,hoveredColor=Color}, onHover=nil, onRelease=nil} end
function Meatball.UI.dynamicPanel() end
function Meatball.UI.inputTextBox() end
function Meatball.UI.scrollBar() end
function Meatball.UI.scrollTextBox() end
function Meatball.UI.textButton() end