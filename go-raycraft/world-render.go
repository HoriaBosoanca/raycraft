package main

import rl "github.com/gen2brain/raylib-go/raylib"

func (world *World) renderWorld(renderDistance int) {
	distanceFromCam := make(map[int][]*Chunk)

	for chunkPos, chunk := range world.chunks {
		cameraWorldPos := Position2{X: int(camera3D.Position.X), Z: int(camera3D.Position.Z)}
		cameraChunkPos := worldPos2ToChunkPos2(cameraWorldPos)
		if chunkPos.X-cameraChunkPos.X <= renderDistance &&
			chunkPos.X-cameraChunkPos.X >= -renderDistance &&
			chunkPos.Z-cameraChunkPos.Z <= renderDistance &&
			chunkPos.Z-cameraChunkPos.Z >= -renderDistance {
			camPos2 := worldPos2ToChunkPos2(Position2{X: int(camera3D.Position.X), Z: int(camera3D.Position.Z)})
			if distanceFromCam[distPos2(chunkPos, camPos2)] == nil {
				distanceFromCam[distPos2(chunkPos, camPos2)] = make([]*Chunk, 0)
			}
			distanceFromCam[distPos2(chunkPos, camPos2)] = append(distanceFromCam[distPos2(chunkPos, camPos2)], chunk)
		}
	}

	for i := len(distanceFromCam) - 1; i >= 0; i-- {
		for _, chunk := range distanceFromCam[i] {
			rl.DrawModel(chunk.mesh.OpaqueModel.FinalModel, rl.Vector3{}, 1.0, rl.White)
			rl.DrawModel(chunk.mesh.TransparentModel.FinalModel, rl.Vector3{}, 1.0, rl.White)
		}
	}

	rl.DrawGrid(2*WORLD_SIZE, CHUNK_SIZE)
}
