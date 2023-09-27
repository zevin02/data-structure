package bptree

import (
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestNewBPTree(t *testing.T) {
	bpt := NewBPTree(4)
	bpt.Set(10, 1)
	bpt.Set(23, 1)
	bpt.Set(33, 1)
	bpt.Set(35, 1)
	bpt.Set(15, 1)
	bpt.Set(2, 1)
	bpt.Set(13, 1)
	bpt.Set(39, 1)
	assert.Equal(t, int64(39), bpt.root.maxKey)
	assert.Equal(t, 2, len(bpt.root.Nodes))
	assert.Equal(t, int64(15), bpt.root.Nodes[0].maxKey)
	assert.Equal(t, int64(39), bpt.root.Nodes[1].maxKey)
	bpt.Set(12, 1)
	assert.Equal(t, 3, len(bpt.root.Nodes))
	bpt.Set(45, 1)
	assert.Equal(t, int64(45), bpt.root.maxKey)
	assert.Equal(t, 4, len(bpt.root.Nodes))
	bpt.Set(11, 1)
	bpt.Set(14, 1)
	bpt.Set(14, 15)
	//fmt.Println(bt)
	t.Log(bpt.Get(10))
	t.Log(bpt.Get(15))
	t.Log(bpt.Get(20))
	t.Log(bpt.Get(14))

}
