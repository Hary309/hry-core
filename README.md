# hry-core

hry-core is a small framework that provides many useful tools for creating plugins for **Euro Truck Simulator 2** and **American Truck Simulator**

## Features

- UI and custom rendering based on Dear ImGui (supports DirectX 11 and OpenGL renderer)

- Easy API for creating settings and keybinds

- SCS telemetry wrapped with modern C++

  - Access to channels for truck, multiple trailer and job

  - Gameplay events

  - Configuration callback for truck, multiple trailers and more

- Logger

- Easy access to mouse, keyboard and joystick (but I suggest using keybinds instead)

- API for creating detour and hooks

- And much more!


## Motivation

The idea was to create a simple tool (only for me) which provides simple API for rendering and inputs. In the process of developing it, I was feeling that I can do better so I keep adding more and more features. Now I am in a point where I'm almost satisfied with the API, and I'm ready to publish first alpha.

Unfortunately, the documentation isn't there yet so you have to learn from examples. Check out the [example](example) and [hry-sic](https://github.com/Hary309/hry-sic)

## This isn't binding of game

This mod isn't like FiveM or MTA:SA, you still have to reverse engineer and hook the game on your own, to help you with that, **hry-core** provides some useful tools (Detour, Pattern scanning, vtable hooks)

## How to use it

(This section isn't complete)

hry-core publicly available is compiled with clang-cl (Clang 11 rc5 and MSVC 19.27), so you can link with it

## TODO List

- support for loading images
- Support for XInput
- API for controller's axis (easy binding and usage like keybinds)

