/*
 * autor:   lis
 * created: Nov 14 2020
 * changed: Nov 20 03:54:38 2020
 */

#include <iostream>
#include <fstream>

#include <lis/macro.hpp>


using namespace std;





/*
 * Структура узла красно-чёрного дерева,
 * содержит:
 * 1.Ключ
 * 2.Цвет вершины
 * 3.Указатели на левого и правого ребёнка
 * 4.Указатель на родителя
 */
struct Node
{
	int key;
	char color = '\0';

	Node *left = nullptr;
	Node *right = nullptr;
	Node *parent = nullptr;
};



/*
 * Освобождение памяти
 */
void free(Node *node)
{
	if(!node)
		return;

	free(node->left);
	free(node->right);
	delete node;
	return;
}


/*
 * Рекурсивное отображение дерева с указанием,
 * является ли данное поддерев правым или левым;
 * также указывается цвет (после ключа)
 */
void print(Node *node, char ch = 0, string const &tab = "")
{
	if(!node)
		return;

	cout << '\n' << tab;
	if(ch)
		cout << (ch == 'l' ? "l: " : "r: ");
	cout << node->key << " " << node->color;

	print(node->left, 'l', tab + "  ");
	print(node->right, 'r', tab + "  ");

	return;
}



// Поиск элемента
/*
 * Полностью повторяют функции BST:
 * см. github.com/lis411/bst для
 * подробного объяснения
 */
Node *find(Node *node, int key)
{
	while(node && node->key != key)
		node = key < node->key ? node->left : node->right;
	return node;
}

Node *min(Node *node)
{
	if(!node)
		return nullptr;
	while(node->left)
		node = node->left;
	return node;
}

Node *max(Node *node)
{
	if(!node)
		return nullptr;
	while(node->right)
		node = node->right;
	return node;
}

Node *next(Node *node)
{
	if(!node or node->right)
		return min(node->right);

	while(node->parent and node != node->parent->left)
		node = node->parent;

	return node->parent;
}

Node *prev(Node *node)
{
	if(!node or node->left)
		return max(node->left);

	while(node->parent and node != node->parent->right)
		node = node->parent;

	return node->parent;
}



// Алгоритмы преобразования дерева
/*
 * Вспомогательная функция, которая перемещает узел
 * what на место where (ставит в соответствие указатель
 * на дочерний элемент у родителя lhs и указатель на
 * родителя у rhs).
 *
 * Оставляет неккоректными указатели where->parent и
 * what->parent->left или what->parent->right (в
 * завимисости от того, каким — левым или правым —
 * узлом был what)
 */
void replant(Node **tree, Node *where, Node *what)
{
	// set parent -> child link
	if(!where->parent)
		*tree = what;
	else if(where == where->parent->left)
		where->parent->left = what;
	else
		where->parent->right = what;

	// set child -> parent link
	if(what)
		what->parent = where->parent;

	return;
}


// Алгоритмы поворота
/*
 * y, y->left is not null!
 */
void right_rotate(Node **tree, Node *y)
{
	Node *x = y->left;
	replant(tree, y, x);

	y->left = x->right;
	if(y->left)
		y->left->parent = y;

	x->right = y;
	y->parent = x;

	return;
}

/*
 * x, x->right is not null!
 */
void left_rotate(Node **tree, Node *x)
{
	Node *y = x->right;
	replant(tree, x, y);

	x->right = y->left;
	if(x->right)
		x->right->parent = x;

	y->left = x;
	x->parent = y;

	return;
}

/*
 * Алгоритм исправления дерева после вставки.
 * Подразумевается, что current существует.
 * Алгоритм исправляет ситуации, когда:
 *
 * 1. current является красным, но вместе с тем корнем
 * 2. current является красным и его родитель тоже
 *
 */
void fix(Node **tree, Node *current)
{
	Node *grandfather, *uncle;

again:
	/*
	 * Если текущий узел — корень, то просто
	 * перекрашиваем его; это возможно, так как
	 * это просто увеличит чёрную длину всех 
	 * путей на единицу
	 */
	if(!current->parent)
	{
		current->color = 'b';
		return;
	}

	/*
	 * Если родитель чёрный, то можно кончать,
	 * так как, во-первых, вставка красного не нарушает
	 * чёрные длины путей, а во-вторых, не нарушается
	 * условие, что у красного узла чёрные дети
	 */
	if(current->parent->color == 'b')
		return;

	/*
	 * Дед обязательно существует, так как отец — красный,
	 * а значит, не может быть корнем
	 */
	grandfather = current->parent->parent;

	if(current->parent == grandfather->left)
	{
		uncle = grandfather->right;
		if(!uncle or uncle->color == 'b')
		{
			if( current == current->parent->right )
			{
				current = current->parent;
				left_rotate(tree, current);
			}
			current->parent->color = 'b';
			grandfather->color = 'r';
			right_rotate(tree, grandfather);
			return;
		}
	}
	else
	{
		uncle = grandfather->left;
		if(!uncle or uncle->color == 'b')
		{
			if( current == current->parent->left )
			{
				current = current->parent;
				right_rotate(tree, current);
			}
			current->parent->color = 'b';
			grandfather->color = 'r';
			left_rotate(tree, grandfather);
			return;
		}
	}

	grandfather->color = 'r';
	grandfather->right->color  = 'b';
	grandfather->left->color = 'b';

	current = grandfather;
	goto again;
}



/*
 * Работает точно так же, как и insert в bst,
 * однако есть пару отличий:
 *
 * 1. Вставляемый узел окрашивается в красный
 * 2. Вызывает функцию fix, которая изменяет
 *    дерево, чтобы оно вновь стало красно-чёрным
 */
bool insert(Node **tree, int key)
{
	Node *father = nullptr;
	Node *current = *tree;
	while(current)
	{
		if(key == current->key)
			return false;

		father = current;
		if(key < current->key)
			current = current->left;
		else
			current = current->right;
	}

	Node *inserting = new Node { key };
	inserting->color = 'r';
	inserting->parent = father;
	if(!father)
		*tree = inserting;
	else if(key < father->key)
		father->left = inserting;
	else
		father->right = inserting;
	fix(tree, inserting);
	return true;
}

/*
 * Функция подразумевает, что переданный ей узел (x) 
 * является поддеревом, в котором чёрная высота стала
 * на единицу меньше положенной; x может быть пустым
 * поддеревом (nullptr), поэтому необходимо указать
 * родителя x, чтобы можно было получить доступ к под-
 * дереву, один из детей которого имеет неправильную
 * чёрную высоту
 */
void fix_rm(Node **tree, Node *x, Node *parent)
{
	Node *b; // brother

	/*
	 * Если x — корень, то изменение его высоты
	 * не нарушит свойств красно-чёрного дерева;
	 * если же x — красный, то, чтобы увеличить
	 * его чёрную высоту, достаточно окрасить его
	 * в чёрный. Иначе входим в цикл.
	 */
	while(parent and (!x or x->color == 'b'))
	{
		if(x == parent->left)
		{
			b = parent->right;

			/*
			 * Делаем, чтобы цвет брата был чёрный
			 */
			if(b->color == 'r')
			{
				b->parent->color = 'r';
				b->color = 'b';
				left_rotate(tree, parent);
				b = parent->right;
			}

			/*
			 * Возможно, оба ребёнка брата чёрные, тогда
			 * мы можем окрасить брата в красный, тем самым
			 * уменьшив его чёрную высоту и сравняв её с
			 * нашим поддеревом x; в этом случае установим
			 * x в своего родителя и вернёмся к началу
			 */
			if(
				(!b->left or b->left->color == 'b') and
				(!b->right or b->right->color == 'b')
			)
			{
				b->color = 'r';
				x = parent;
				parent = parent->parent;
			}
			else
			{
				/*
				 * Делаем, чтобы правый ребёнок брата был
				 * красным
				 */
				if(!b->right or b->right->color == 'b')
				{
					b->left->color = 'b';
					b->color = 'r';
					right_rotate(tree, b);
					b = parent->right;
				}

				/*
				 * Перекрашиваем и вертим дерево, чтобы
				 * установить необходимую чёрную высоту
				 */
				b->color = parent->color;
				parent->color = 'b';
				b->right->color = 'b';
				left_rotate(tree, parent);
				break;
			}
		}

		/*
		 * Полностью идентичный код вышепреведённому за
		 * тем исключением, что все обращения к левому
		 * ребёнку заменены обращениями к правому и,
		 * соответственно, наоборот; также все повороты
		 * влево заменены на повороты вправо и наоборот
		 */
		else
		{
			b = parent->left;
			if(b->color == 'r')
			{
				b->parent->color = 'r';
				b->color = 'b';
				right_rotate(tree, parent);
				b = parent->left;
			}

			if(
				(!b->left or b->left->color == 'b') and
				(!b->right or b->right->color == 'b')
			)
			{
				b->color = 'r';
				x = parent;
				parent = parent->parent;
			}
			else
			{
				if(!b->left or b->left->color == 'b')
				{
					b->right->color = 'b';
					b->color = 'r';
					left_rotate(tree, b);
					b = parent->left;
				}
				b->color = parent->color;
				parent->color = 'b';
				b->left->color = 'b';
				right_rotate(tree, parent);
				break;
			}
		}
	}

	/*
	 * Если элемент красный и поэтому мы вышли из
	 * цикла, то устанавливаем его цвет в чёрный;
	 * однако необходима проверка, так как если
	 * мы удаляем лист, x будет nullptr
	 */
	if(x)
		x->color = 'b';
	return;
}

void erase(Node **tree, Node *r)
{
	Node *d = r;
	Node *tofix, *tofixpar;
	char dorgcolor = d->color;

	if(!r->left)
	{
		tofix = r->right;
		replant(tree, r, r->right);
		tofixpar = r->parent;
	}
	else if(!r->right)
	{
		tofix = r->left;
		replant(tree, r, r->left);
		tofixpar = r->parent;
	}
	else
	{
		d = min(r->right);
		dorgcolor = d->color;
		tofix = d->right;
		tofixpar = d;

		if(d->parent != r)
		{
			replant(tree, d, d->right);
			d->right = r->right;
			r->right->parent = d;
			tofixpar = d->parent;
		}

		replant(tree, r, d);
		d->left = r->left;
		d->left->parent = d;

		d->color = r->color;
	}

	delete r;
	if(dorgcolor == 'b')
		fix_rm(tree, tofix, tofixpar);
	return;
}

bool erase(Node **tree, int key)
{
	Node *r = find(*tree, key);
	if(!r)
		return false;
	erase(tree, r);
	return true;
}





/*
 * Функция проверки, является ли данное
 * дерево бинарным деревом поиска;
 * (используется для проверки на красно-
 * чёрное дерево)
 */
bool is_bst(Node *node, int *min = nullptr, int *max = nullptr)
{
	if(!node)
		return true;

	if(
		( min and node->key < *min ) or
		( max and node->key > *max ) or
		( node->left  and node->left->key  > node->key ) or
		( node->right and node->right->key < node->key )
	)
		return false;

	return
		is_bst(node->left,  min, &node->key) and
		is_bst(node->right, &node->key, max);
}

/*
 * Проверяет, является ли данное дерево
 * красно-чёрным деревом
 */
bool is_rbt(Node *node, bool isroot = true, int *bh = nullptr)
{
	if(!node)
	{
		if(bh)
			*bh = 0;
		return true;
	}

	if( isroot and ( !is_bst(node) or node->color != 'b' ) )
	{
		cout << "root fail" << endl;
		return false;
	}

	if(
		node->color == 'r' and
		( (node->left  and node->left->color  == 'r') or
		  (node->right and node->right->color == 'r') )
	)
	{
		cout << "double red fail on " << node->key << endl;
		return false;
	}

	int lbh, rbh;
	if(
		!is_rbt(node->left, false, &lbh) or
		!is_rbt(node->right, false, &rbh)
	)
		return false;

	if( lbh != rbh )
	{
		cout << "black height fail on " << node->key << endl;
		return false;
	}

	if(bh)
		*bh = lbh + (node->color == 'b' ? 1 : 0);

	return true;
}





// main
/*
 * Основная функция программы реализует 
 * следующие команды:
 * + KEY // добавить ключ KEY в дерево
 * - KEY // удалить ключ KEY из дерева
 * p     // отобразить дерево
 *
 * При выполнении каждой команды выполняется
 * проверка, является ли дерево RBT.
 *
 * Для выхода используйте CTRL+D.
 */
int main( int argc, char *argv[] )
{
	Node *tree = nullptr;
	char com;
	string nil;
	int first;


	TRY

	while(cin >> com)
	{
		switch(com)
		{
		case '+':
			if(!(cin >> first))
				goto invalid_command;

			if(insert(&tree, first))
				cout << "Key " << first << " success inserted" << endl;
			else
				cout << "Key " << first << " already exists" << endl;
			break;

		case '-':
			if(!(cin >> first))
				goto invalid_command;

			if(erase(&tree, first))
				cout << "Node with key " << first << " success erased" << endl;
			else
				cout << "Can't find key " << first << endl;
			break;

		case 'p':
			print(tree);
			cout << endl << endl;
			break;

		default:
			cerr << "Error: unknown command" << endl;
			getline(cin, nil);
			break;
		}

		if(!is_rbt(tree))
		{
			cerr << "Tree is't RBT!" << endl;
			print(tree);
			throw "is't RBT";
		}
		continue;

	invalid_command:
		cerr << "Invalid command" << endl;
		cin.clear();
		getline(cin, nil);
	}

	CATCH

	free(tree);

	return 0;
}





// end
