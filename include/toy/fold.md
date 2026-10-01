# fold

`FoldQueue<T,Op>(capacity,identity,operation)` 支持 push、pop、size、fold。
一次分配保存原始后段；前段耗尽时，把后段原位改成后缀聚合值。
后段只额外维护一个总聚合，不必给每个元素保存两份值。

`FoldDeque<T,Op>` 支持 push_front/back、pop_front/back、size、fold。
居中缓冲分成左右两半，分别维护朝向各自外端的聚合；某一侧耗尽时，
把剩余序列均分并重建聚合。该重分配保证更新均摊 O(1)，fold 最坏 O(1)。

Op 须满足结合律，但不要求交换律；identity 为单位元。
T 为平凡可复制类型，pop 要求非空，capacity 限制生命周期内的 push 总数。
