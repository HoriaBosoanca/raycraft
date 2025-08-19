package main

import (
	rl "github.com/gen2brain/raylib-go/raylib"
)

func handleMovementInput() {
	deltaX := rl.Vector3Subtract(camera3D.Target, camera3D.Position)
	deltaX.Y = 0
	deltaX = rl.Vector3Normalize(deltaX)
	if CREATIVE_MODE {
		deltaX = rl.Vector3Scale(deltaX, CREATIVE_MOVE_SPEED)
	} else {
		deltaX = rl.Vector3Scale(deltaX, SURVIVAL_MOVE_SPEED)
	}

	deltaZ := rl.NewVector3(deltaX.Z, 0, -deltaX.X)

	deltaX = rl.Vector3Scale(deltaX, rl.GetFrameTime())
	deltaZ = rl.Vector3Scale(deltaZ, rl.GetFrameTime())

	if rl.IsKeyDown(rl.KeyW) {
		camera3D.Position = rl.Vector3Add(camera3D.Position, deltaX)
		camera3D.Target = rl.Vector3Add(camera3D.Target, deltaX)
	}
	if rl.IsKeyDown(rl.KeyA) {
		camera3D.Position = rl.Vector3Add(camera3D.Position, deltaZ)
		camera3D.Target = rl.Vector3Add(camera3D.Target, deltaZ)
	}
	if rl.IsKeyDown(rl.KeyS) {
		camera3D.Position = rl.Vector3Subtract(camera3D.Position, deltaX)
		camera3D.Target = rl.Vector3Subtract(camera3D.Target, deltaX)
	}
	if rl.IsKeyDown(rl.KeyD) {
		camera3D.Position = rl.Vector3Subtract(camera3D.Position, deltaZ)
		camera3D.Target = rl.Vector3Subtract(camera3D.Target, deltaZ)
	}

	if rl.IsKeyDown(rl.KeySpace) && CREATIVE_MODE {
		camera3D.Position.Y += ASCEND_SPEED * rl.GetFrameTime()
		camera3D.Target.Y += ASCEND_SPEED * rl.GetFrameTime()
	}
	if rl.IsKeyDown(rl.KeyLeftShift) && CREATIVE_MODE {
		camera3D.Position.Y -= ASCEND_SPEED * rl.GetFrameTime()
		camera3D.Target.Y -= ASCEND_SPEED * rl.GetFrameTime()
	}
}

var speed float32 = 0.0

func handleGravity() {
	if CREATIVE_MODE {
		speed = 0.0
		return
	}
	blockBelow := world.worldGetBlockFloat(camera3D.Position.X, camera3D.Position.Y-2, camera3D.Position.Z)
	if blockBelow == AirBlock || blockBelow == WaterBlock {
		speed += GRAVITY_ACCELERATION * rl.GetFrameTime()
		camera3D.Position.Y -= speed * rl.GetFrameTime()
		camera3D.Target.Y -= speed * rl.GetFrameTime()
	} else {
		speed = 0.0
	}
}
