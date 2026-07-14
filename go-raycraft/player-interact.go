package main

import rl "github.com/gen2brain/raylib-go/raylib"

func drawCrosshair() {
	rl.DrawTexture(cursor, int32(rl.GetScreenWidth()/2)-cursor.Width/2, int32(rl.GetScreenHeight()/2)-cursor.Height/2, rl.White)
}

func getPlayerRay() rl.Ray {
	return rl.GetScreenToWorldRay(rl.Vector2{X: float32(rl.GetScreenWidth()) / 2.0, Y: float32(rl.GetScreenHeight()) / 2.0}, camera3D)
}

func drawPlayerTarget() {
	block, blockPos, _ := world.getClosestBlockHit(getPlayerRay(), MAX_CONTINUOUS_CHUNK_TARGET_SEARCH)
	if block != nil && blockPos != nil {
		rl.DrawCube(rl.Vector3Add(position3ToVector3(*blockPos), rl.Vector3{X: 0.5, Y: 0.5, Z: 0.5}), 1.01, 1.01, 1.01, rl.Color{G: 121, B: 241, A: 127})
	}
}

func breakTargetBlock() {
	block, blockPos, chunk := world.getClosestBlockHit(getPlayerRay(), MAX_TRIGGERED_CHUNK_TARGET_SEARCH)
	if block != nil && chunk != nil && blockPos != nil {
		chunk.addBlock(AirBlock, worldPos3ToLocalPos3(*blockPos))
		chunk.generateChunkMesh()
	}
}
