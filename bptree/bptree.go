package bptree

import (
	"sync"
)

//BPItem 用来记录数据
type BPItem struct {
	key int64
	val interface{}
}

//BPNode 叶子节点和内部节点使用相同的结构
type BPNode struct {
	maxKey int64     //非叶子节点，每个索引值存储当前叶子节点的存储的最大key
	Nodes  []*BPNode //当前非叶子节点存储的多个子节点,叶子节点的Nodes=nil
	Items  []BPItem  //当前存储的
	Next   *BPNode   //叶子节点中指向下一个叶子节点
	parent *BPNode   //指向他的父节点
}

//setValue 向当前的叶子节点种插入元素
func (node *BPNode) setValue(key int64, value interface{}) (isMax bool) {
	item := BPItem{key: key, val: value} //构造一个叶子节点插入的对象
	num := len(node.Items)               //获得当前叶子叶子节点原本有多少个值
	if num < 1 {
		//当前叶子节点还没有元素,直接就可以进行插入了
		node.Items = append(node.Items, item)
		node.maxKey = item.key //更新当前节点的最大key值
		return false
	} else if key < node.Items[0].key {
		//当前的key，比第一个key还要小
		node.Items = append([]BPItem{item}, node.Items...) //把之前的元素移动到这个元素之后
		return false
	} else if key > node.Items[num-1].key {
		//当前的元素比最大的key还要大
		node.Items = append(node.Items, item) //在当前的最后插入
		node.maxKey = item.key

		//他的父亲节点的最大值也需要更新

		return true
	}
	//不是上面的这些情况
	//就说明当前的num不是1,同时是在中间进行插入的
	//遍历到比这个小，比这个大的位置
	for i := 0; i < num; i++ {
		if node.Items[i].key > key {
			//如果当前的比key大，说明找到了位置
			node.Items = append(node.Items, BPItem{}) //插入一个位置
			copy(node.Items[i+1:], node.Items[i:])    //往后移动一位
			node.Items[i] = item
			return false
		} else if node.Items[i].key == key {
			//如果当前的key就等于那个key，直接更新
			node.Items[i] = item
			return false
		}
	}
	return false
}

//给当前的节点添加孩子节点
func (node *BPNode) addChild(child *BPNode) {
	num := len(node.Nodes)
	if num < 1 {
		//当前的节点数量=0,直接添加
		node.Nodes = append(node.Nodes, child)
		node.maxKey = child.maxKey
		return
	} else if child.maxKey < node.Nodes[0].maxKey {
		//当前比第一个值还小,插入到第一个位置
		node.Nodes = append([]*BPNode{child}, node.Nodes...)
		return
	} else if child.maxKey > node.Nodes[num-1].maxKey {
		//比最后一个大，插入到结尾
		node.Nodes = append(node.Nodes, child)
		node.maxKey = child.maxKey
		return
	}
	//在中间,中间插入
	for i := 0; i < num; i++ {
		if node.Nodes[i].maxKey > child.maxKey {

			node.Nodes = append(node.Nodes, nil)   //添加一个元素
			copy(node.Nodes[i+1:], node.Nodes[i:]) //向后移动一个位置
			node.Nodes[i] = child
			return
		}
	}
}

//BPTree B+树的定义
type BPTree struct {
	mutex sync.Mutex
	root  *BPNode //当前的根节点
	width int     //当前B+树的阶数,正常的B+树的阶数，再几十到几百之间，
	halfw int     //用于[m/2]=ceil(M/2),拆分的时候，一个floor((M+1)/2),一个ceil((M+1)/2)
}

//NewBPTree 初始化B+树
func NewBPTree(width int) *BPTree {
	if width < 3 {
		width = 3
	}
	bt := &BPTree{}
	bt.root = NewLeafNode(width)
	bt.width = width
	bt.halfw = (bt.width + 1) / 2
	return bt
}

//NewLeafNode 申请width+1是因为插入的时候，可能会暂时出现key大于width的情况，后期再分裂处理
//初始化一个叶子节点
func NewLeafNode(width int) *BPNode {
	Node := &BPNode{}
	Node.Items = make([]BPItem, width+1)
	Node.Items = Node.Items[0:0] //相当于清空了列表,不是nil
	return Node
}

func newIndexNode(width int) *BPNode {
	Node := &BPNode{}
	Node.Nodes = make([]*BPNode, width+1)
	Node.Nodes = Node.Nodes[0:0] //相当于清空了列表,不是nil
	return Node
}

//Get B+树的查询
func (t *BPTree) Get(key int64) interface{} {
	t.mutex.Lock()
	defer t.mutex.Unlock()
	node := t.root //先获得当前的根节点
	for i := 0; i < len(node.Nodes); {
		//遍历当前的根节点，从最小的开始进行查询
		if key <= node.Nodes[i].maxKey {
			//当前的key小于就可以切换到子节点
			node = node.Nodes[i]
			i = 0
			continue
		}
		i++
	}
	//这个走出来的话，就是叶子节点，因为叶子节点的没有子节点
	if len(node.Nodes) > 0 {
		//当前没有走到叶子节点
		return nil
	}
	//在当前的叶子节点中查询数据
	for i := 0; i < len(node.Items); i++ {
		if node.Items[i].key == key {
			return node.Items[i].val //查询找到了当前的数据
		}
	}
	return nil
}

//Set 往B+树中插入数据
func (t *BPTree) Set(key int64, value interface{}) {
	t.mutex.Lock()
	defer t.mutex.Unlock()
	t.setValue(nil, t.root, key, value)
}

//setValue 递归的实现数据的插入
func (t *BPTree) setValue(parent *BPNode, node *BPNode, key int64, value interface{}) {
	//插入时，首先要先定位到叶子节点,如果是非叶子节点的话，就会直接离开,这个循环
	for i := 0; i < len(node.Nodes); i++ {
		if key <= node.Nodes[i].maxKey || i == len(node.Nodes)-1 { //遍历到了最后一个都没有，就进入到最后一个节点中
			t.setValue(node, node.Nodes[i], key, value) //低归的进入到这个函数中进行添加节点
			break
		}
	}
	//定位到叶子节点
	if len(node.Nodes) < 1 {
		//当前低归到了叶子节点,没有子节点
		node.setValue(key, value)
	}
	//节点先插入后分裂，尝试是否会发生节点的分裂,这个地方既可能是非叶子节点，也可能是叶子节点
	nodeNew := t.splitNode(node) //
	if nodeNew != nil {
		//当前出现了一个新的节点，说明发生了节点的分裂
		//如果父节点不存在的话，就需要创建一个新的父节点
		if parent == nil {
			parent = newIndexNode(t.width)
			parent.addChild(node) //把当前的节点添加为子节点
			t.root = parent       //更新根节点
		}
		parent.addChild(nodeNew) //把新节点也添加上
	}
	if parent != nil && parent.maxKey < key { //一层一层往上面低归修改最大值
		parent.maxKey = key
	}

}

//对节点进行分裂
func (t *BPTree) splitNode(node *BPNode) *BPNode {
	if len(node.Nodes) > t.width {
		//如果当前的非叶子节点，他的子节点数超过了width宽度，说明就要创建新节点
		node2 := newIndexNode(t.width)                                            //构造一个新节点
		node2.Nodes = append(node2.Nodes, node.Nodes[t.halfw:len(node.Nodes)]...) //移动一半的数据到新节点中
		node2.maxKey = node2.Nodes[len(node2.Nodes)-1].maxKey
		//修改原节点的数据
		node.Nodes = node.Nodes[0:t.halfw]
		node.maxKey = node.Nodes[len(node.Nodes)-1].maxKey
		return node2
	} else if len(node.Items) > t.width {
		//如果当前是叶子节点，同时当前节点的元素超过了width宽度，就需要增加新的节点
		node2 := NewLeafNode(t.width)                                             //得到一个新节点
		node2.Items = append(node2.Items, node.Items[t.halfw:len(node.Items)]...) //移动后面一半的节点到node2中
		node2.maxKey = node2.Items[len(node2.Items)-1].key                        //最大值，就是最后一个元素的值了
		//修改原来节点的数据
		node.Next = node2 //指向下一个个节点
		node.Items = node.Items[0:t.halfw]
		node.maxKey = node.Items[len(node.Items)-1].key //更新当前节点的最大值
		return node2
	}
	//当前元素不需要进行更新
	return nil
}
