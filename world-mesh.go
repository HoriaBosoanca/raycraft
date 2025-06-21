package main

import (
	"fmt"
	rl "github.com/gen2brain/raylib-go/raylib"
)

type ChunkMesh struct {
	OpaqueModel      ChunkModel
	TransparentModel ChunkModel
}

type ChunkModel struct {
	Initialized bool

	VertexCount    int32
	Vertices       []float32
	TriangleCount  int32
	Indices        []uint16
	Colors         []uint8
	TexcoordsCount int32
	Texcoords      []float32

	FinalModel rl.Model
}

func (world *World) generateMeshes() {
	for _, chunk := range world.chunks {
		chunk.generateChunkMesh()
	}
}

// this uses world for some much-needed optimizations, but can be removed
func (chunk *Chunk) generateChunkMesh() {
	chunk.mesh = &ChunkMesh{}
	for x := range chunk.blocks {
		for z := range chunk.blocks[x] {
			for y, block := range chunk.blocks[x][z] {
				worldPos := chunkPos2AndLocalPos2ToWorldPos2(chunk.position, Position2{X: x, Z: z})
				if block.data == AirBlock || world.isBlockSurrounded(worldPos.X, y, worldPos.Z) {
					continue
				}
				drawPos := rl.Vector3{X: float32(worldPos.X), Y: float32(y), Z: float32(worldPos.Z)}
				if isTransparent(block.data) {
					chunk.mesh.TransparentModel.addBlock(drawPos, block.data)
				} else {
					chunk.mesh.OpaqueModel.addBlock(drawPos, block.data)
				}
			}
		}
	}
	chunk.mesh.OpaqueModel.buildChunkMesh()
	chunk.mesh.TransparentModel.buildChunkMesh()
}

func (chunkModel *ChunkModel) addBlock(position rl.Vector3, block int8) {
	// Initialization
	if !chunkModel.Initialized {
		chunkModel.Initialized = true
		chunkModel.VertexCount = 0
		chunkModel.Vertices = make([]float32, 0)
		chunkModel.TriangleCount = 0
		chunkModel.Indices = make([]uint16, 0)
		chunkModel.Colors = make([]uint8, 0)
		chunkModel.TexcoordsCount = 0
	}

	// Vertices
	chunkModel.VertexCount += int32(len(cubeVertices) / 3)
	translatedVertices := make([]float32, len(cubeVertices))
	copy(translatedVertices, cubeVertices)
	for i := range translatedVertices {
		switch i % 3 {
		case 0:
			translatedVertices[i] += position.X
		case 1:
			translatedVertices[i] += position.Y
		case 2:
			translatedVertices[i] += position.Z
		}
	}
	chunkModel.Vertices = append(chunkModel.Vertices, translatedVertices...)

	// Indices
	chunkModel.TriangleCount += 12
	startIndex := uint16(len(chunkModel.Indices))
	for i := startIndex; i < startIndex+36; i++ {
		chunkModel.Indices = append(chunkModel.Indices, i)
	}

	// Colors
	for i := 0; i < 36; i++ {
		white := rl.White
		chunkModel.Colors = append(chunkModel.Colors, white.R, white.G, white.B, white.A)
	}

	// Textures
	coordinatesUV := make([]float32, len(cubeTexture))
	for i, v := range cubeTexture {
		// add the offset corresponding to the block's face's texture:

		// there are 36 UV points (3 per triangle; there are 12 triangles), so 72 float32s in the standard cube texture
		// each face has 12 float32s (72 total float32s / 6 faces = 12 float32s per face)
		// with i being in the range 1...71, i/12 is the index of each face (0...5)

		if i%2 == 0 { // even -> U
			coordinatesUV[i] = v + textureMap[block][i/12].X
		} else { // odd -> V
			coordinatesUV[i] = v + textureMap[block][i/12].Y
		}
	}
	chunkModel.Texcoords = append(chunkModel.Texcoords, coordinatesUV...)
}

func (chunkModel *ChunkModel) buildChunkMesh() {
	if !chunkModel.Initialized {
		fmt.Println("ChunkMesh not initialized")
		return
	}

	var mesh rl.Mesh
	mesh.VertexCount = chunkModel.VertexCount
	mesh.Vertices = &chunkModel.Vertices[0]
	mesh.TriangleCount = chunkModel.TriangleCount
	mesh.Indices = &chunkModel.Indices[0]
	mesh.Colors = &chunkModel.Colors[0]
	mesh.Texcoords = &chunkModel.Texcoords[0]

	rl.UploadMesh(&mesh, false)
	chunkModel.FinalModel = rl.LoadModelFromMesh(mesh)
	if chunkModel.FinalModel.Materials != nil {
		chunkModel.FinalModel.Materials.Maps.Texture = atlas
	}
}

var cubeVertices = []float32{
	// face 1
	0.0, 1.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 1.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 0.0, 0.0,

	// face 2
	0.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 0.0,

	// face 3
	1.0, 0.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 0.0, 0.0,

	// face 4
	0.0, 1.0, 1.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	0.0, 0.0, 1.0,

	// face 5
	1.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 0.0,

	// face 6
	0.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	1.0, 0.0, 0.0,
}
