package main

type Structure struct {
	blocks [][][]int8
	size   Position3
}

var (
	tree_trunk = Structure{
		blocks: [][][]int8{
			{
				{OakLogBlock, OakLogBlock, OakLogBlock, OakLogBlock, OakLogBlock},
			},
		},
		size: Position3{X: 1, Y: 5, Z: 1},
	}
	tree_crown = Structure{
		blocks: [][][]int8{
			{
				{AirBlock, AirBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{AirBlock, AirBlock, AirBlock, AirBlock},
			},
			{
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, OakLeafBlock, OakLeafBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
			},
			{
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, OakLeafBlock, OakLeafBlock},
				{OakLogBlock, OakLogBlock, OakLogBlock, OakLeafBlock},
				{OakLeafBlock, OakLeafBlock, OakLeafBlock, OakLeafBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
			},
			{
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, OakLeafBlock, OakLeafBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
			},
			{
				{AirBlock, AirBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{OakLeafBlock, OakLeafBlock, AirBlock, AirBlock},
				{AirBlock, AirBlock, AirBlock, AirBlock},
			},
		},
		size: Position3{X: 5, Y: 4, Z: 5},
	}
)

func (chunk *Chunk) addStructure(structure Structure, chunkPos Position3) {
	for x := range structure.blocks {
		for z := range structure.blocks[x] {
			for y, block := range structure.blocks[x][z] {
				chunk.addBlock(block, Position3{X: chunkPos.X + x, Y: chunkPos.Y + y, Z: chunkPos.Z + z})
			}
		}
	}
}

func (chunk *Chunk) wouldTruncateStructure(structure Structure, localPos Position3) bool {
	if localPos.X < 0 || localPos.Y < 0 || localPos.Z < 0 {
		return true
	}
	if structure.size.X+localPos.X > CHUNK_SIZE || structure.size.Y+localPos.Y > CHUNK_HEIGHT || structure.size.Z+localPos.Z > CHUNK_SIZE {
		return true
	}
	return false
}
