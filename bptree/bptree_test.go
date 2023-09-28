package bptree

import (
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestNewBPTreeSet(t *testing.T) {
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
	assert.Equal(t, 2, len(bpt.root.Nodes))
	assert.Equal(t, int64(12), bpt.root.Nodes[0].maxKey)
	assert.Equal(t, int64(45), bpt.root.Nodes[1].maxKey)
	assert.Equal(t, int64(10), bpt.root.Nodes[0].Nodes[0].maxKey)
	assert.Equal(t, int64(12), bpt.root.Nodes[0].Nodes[0].Next.maxKey)
	assert.Equal(t, int64(15), bpt.root.Nodes[0].Nodes[0].Next.Next.maxKey)
	assert.Equal(t, int64(33), bpt.root.Nodes[0].Nodes[0].Next.Next.Next.maxKey)
	assert.Equal(t, int64(45), bpt.root.Nodes[0].Nodes[0].Next.Next.Next.Next.maxKey)
	bpt.Set(46, 15)
	bpt.Set(37, 15)
	assert.Equal(t, int64(46), bpt.root.Nodes[1].maxKey)
	bpt.Set(38, 15)
	bpt.Set(40, 15)
	assert.Equal(t, int64(12), bpt.root.Nodes[0].maxKey)
	assert.Equal(t, int64(33), bpt.root.Nodes[1].maxKey)
	assert.Equal(t, int64(46), bpt.root.Nodes[2].maxKey)
	bpt.Set(47, 15)
	bpt.Set(48, 15)
	assert.Equal(t, int64(48), bpt.root.Nodes[2].maxKey)
	assert.Equal(t, 6, len(bpt.Scan(9, 16)))
	t.Log(bpt.Get(10))
	t.Log(bpt.Get(15))
	t.Log(bpt.Get(20))
	t.Log(bpt.Get(14))
	t.Log(bpt.Get(37))

}

func TestNewBPTreeDelete1(t *testing.T) {
	bpt := NewBPTree(4)
	bpt.Set(10, 1)
	bpt.Set(23, 1)
	bpt.Set(33, 1)
	bpt.Set(35, 1)
	bpt.Set(15, 1)
	bpt.Set(2, 1)
	bpt.Set(13, 1)
	bpt.Set(39, 1)
	bpt.Remove(33)
	assert.Equal(t, 3, len(bpt.root.Nodes[1].Items))
	bpt.Remove(39)
	assert.Equal(t, 2, len(bpt.root.Nodes[1].Items))
	assert.Equal(t, int64(35), bpt.root.maxKey)
	bpt.Remove(35)
	assert.Equal(t, int64(23), bpt.root.maxKey)
	bpt.Remove(13)
	assert.Equal(t, int64(10), bpt.root.Nodes[0].maxKey)
	bpt.Remove(10)
	assert.Equal(t, int64(23), bpt.root.Nodes[0].maxKey)
	assert.Equal(t, 1, len(bpt.root.Nodes))
	assert.Nil(t, bpt.Get(10))
	assert.NotNil(t, bpt.Get(23))
	assert.Equal(t, 1, bpt.Get(23))
}

func TestNewBPTreeDelete2(t *testing.T) {
	bpt := NewBPTree(4)
	bpt.Set(10, 1)
	bpt.Set(23, 1)
	bpt.Set(33, 1)
	bpt.Set(35, 1)
	bpt.Set(15, 1)
	bpt.Set(2, 1)
	bpt.Set(13, 1)
	bpt.Set(39, 1)
	bpt.Set(12, 1)
	bpt.Set(45, 1)
	bpt.Set(11, 1)
	bpt.Set(14, 1)
	bpt.Set(14, 15)
	bpt.Set(46, 15)
	bpt.Set(37, 15)
	bpt.Set(38, 15)
	bpt.Set(40, 15)
	bpt.Set(47, 15)
	bpt.Set(48, 15)
	bpt.Remove(45)
	assert.Equal(t, int64(46), bpt.root.Nodes[2].Nodes[2].maxKey)
	assert.Equal(t, int64(40), bpt.root.Nodes[2].Nodes[2].Items[0].key)
	assert.Equal(t, int64(47), bpt.root.Nodes[2].Nodes[3].Items[0].key)
	bpt.Remove(46)
	assert.Equal(t, 3, len(bpt.root.Nodes[2].Nodes))
	assert.Equal(t, int64(48), bpt.root.Nodes[2].Nodes[2].maxKey)
	assert.Equal(t, int64(47), bpt.root.Nodes[2].Nodes[2].Items[0].key)
	assert.Equal(t, 4, len(bpt.Scan(36, 41)))
	//借右兄弟的一个key过来
	bpt.Remove(37)
	//和右兄弟合并,第二层也和右节点进行合并
	bpt.Remove(10)
	assert.Equal(t, 2, len(bpt.root.Nodes))
	assert.Equal(t, 3, len(bpt.root.Nodes[0].Nodes))
	assert.Equal(t, int64(33), bpt.root.Nodes[0].maxKey)
	//与左节点进行合并
	bpt.Remove(48)
	assert.Equal(t, int64(47), bpt.root.maxKey)
	assert.Equal(t, 2, len(bpt.root.Nodes[1].Nodes))
	bpt.Remove(38)
	assert.Equal(t, int64(39), bpt.root.Nodes[1].Nodes[0].maxKey)
	//第二层节点向左边借一个node
	bpt.Remove(47)
	assert.Equal(t, int64(15), bpt.root.Nodes[0].maxKey)
	assert.Equal(t, int64(40), bpt.root.Nodes[1].maxKey)
	assert.Equal(t, 2, len(bpt.root.Nodes[0].Nodes))
	assert.Equal(t, 2, len(bpt.root.Nodes[1].Nodes))
	bpt.Remove(33)
	bpt.Remove(40)
	assert.Equal(t, 1, len(bpt.root.Nodes))
	assert.Equal(t, 3, len(bpt.root.Nodes[0].Nodes))
	assert.Equal(t, int64(39), bpt.root.Nodes[0].maxKey)
	bpt.Set(40, 1)
	assert.Equal(t, int64(40), bpt.root.maxKey)
	bpt.Set(41, 1)
	assert.Equal(t, int64(41), bpt.root.maxKey)
	assert.Equal(t, 4, len(bpt.root.Nodes[0].Nodes))
	bpt.Set(16, 1)
	assert.Equal(t, 12, len(bpt.Scan(1, 42)))
}
func TestNewBPTreeDelete3(t *testing.T) {
	tree := NewBPTree(3)
	for i := int64(1); i <= 10; i++ {
		tree.Set(i, i)
	}
	// 删除叶子节点，不需要合并
	tree.Remove(1)
	if tree.Get(1) != nil {
		t.Errorf("Delete failed, expected nil but got %v", tree.Get(1))
	}

	// 删除叶子节点，需要合并
	tree.Remove(2)
	if tree.Get(2) != nil {
		t.Errorf("Delete failed, expected nil but got %v", tree.Get(2))
	}

	// 删除内部节点，需要合并
	tree.Remove(3)
	if tree.Get(3) != nil {
		t.Errorf("Delete failed, expected nil but got %v", tree.Get(3))
	}

	// 删除内部节点，不需要合并
	tree.Remove(4)
	if tree.Get(4) != nil {
		t.Errorf("Delete failed, expected nil but got %v", tree.Get(4))
	}

	// 删除后树为空
	for i := int64(5); i <= 10; i++ {
		tree.Remove(i)
	}

}
