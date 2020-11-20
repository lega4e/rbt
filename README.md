# Red-Black Tree

Программа является реализацией красно-чёрного дерева — разновидности самобалансирующегося при удалении и вставки бинарного дерева поиска (см. github.com/lis411/bst). Программа подробно задокументирована в исходных кодах.

Пример использования:

```
lis@lis-pc:~/work/sct/rbt$ main
+ 4
Key 4 success inserted
+ 1
Key 1 success inserted
+ 9
Key 9 success inserted
+ 10
Key 10 success inserted
+ 11
Key 11 success inserted
+ 12
Key 12 success inserted
+ 3
Key 3 success inserted
p

4 b
  l: 1 b
    r: 3 r
  r: 10 r
    l: 9 b
    r: 11 b
      r: 12 r

- 10
Node with key 10 success erased
p

4 b
  l: 1 b
    r: 3 r
  r: 11 r
    l: 9 b
    r: 12 b

- 1
Node with key 1 success erased
p

4 b
  l: 3 b
  r: 11 r
    l: 9 b
    r: 12 b
```

Также присутствует дополнительная программа для генерации входных данных для красно-чёрного дерева:

```
lis@lis-pc:~/work/sct/rbt$ gen 10 0 100
+ 86
+ 76
+ 48
+ 54
+ 11
+ 18
+ 76
+ 85
+ 13
+ 72
- 72
- 18
- 86
- 54
- 76
- 85
- 13
- 76
- 48
- 11
```
