package main

import (
	rl "github.com/gen2brain/raylib-go/raylib"
)

var (
	SHOW_PLAYER_TARGET = false
	CREATIVE_MODE      = false
)

func handleInput() {
	handleMovementInput()

	if rl.IsKeyPressed(rl.KeyF11) {
		rl.ToggleFullscreen()
	}
	if rl.IsKeyPressed(rl.KeyF10) {
		if rl.IsCursorHidden() {
			rl.EnableCursor()
		} else {
			rl.DisableCursor()
		}
	}
	if rl.IsKeyPressed(rl.KeyF9) {
		SHOW_PLAYER_TARGET = !SHOW_PLAYER_TARGET
	}
	if rl.IsKeyPressed(rl.KeyF8) {
		CREATIVE_MODE = !CREATIVE_MODE
	}
	if rl.IsMouseButtonPressed(rl.MouseButtonLeft) {
		breakTargetBlock()
	}
}
