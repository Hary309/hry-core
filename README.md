# hry-core

hry-core is utility library which provides many useful tools for creating plugins for **Euro Truck Simulator 2** and **American Truck Simulator**

## Motivation

The idea was to create a simple tool (only for me) which provides simple API for rendering and inputs. In the process of developing it, I was feeling that I can do better so I started adding more and more features. Now I am in a point where I'm almost satisfied with the API, and I'm ready to publish first alpha.

Unfortunately, the documentation isn't there yet so you have to learn from example plugins. Check out the [hry-sic](https://github.com/Hary309/hry-sic)

## Features

- UI and rendering based on Dear ImGui

- Easy API for creating settings and keybinds

- SCS telemetry wrapped with modern C++

  - Access to channels for truck, multiple trailer and job

  - Gameplay events

  - Configuation callback for truck, multiple trailers and more

- Logger

- Easy access to mouse, keyboard and joystick (but I suggest using keybinds instead)

- API for creating detour and hooks

- And much more!

## This isn't binding of game

This mod isn't like FiveM or MTA:SA, you still have to decode and hook the game on your own, to help you with that, **hry-core** provides some useful tools (Detour, Pattern scanning, vtable hooks)
