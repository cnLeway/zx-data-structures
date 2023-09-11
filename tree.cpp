//
// 这里我们是按ｃ风格实现二叉树，但也会大量使用c++的一些东西，方便我们快速实现。
// 等我们完全理解啦二叉树后，可以考虑再用c++的类方式重新实现一遍，体会一下两种
// 实现方式下的二叉树的使用方法。
// 使用c++的万能头文件，就不用考虑其余的include啦
#include <bits/stdc++.h>
using namespace std;

// 这ｃ函数需要用到的头文件。编程中每行代码中用的ｃ的函数或者变量，如果不是自己定义的
// 需要知道是在哪个头文件的。不知道的话，百度一下就知道啦。
#include <stdio.h>
#include <stdlib.h>

// 定义节点结构体
// 具体这个结构体应该如何定义，应该是和程序本身的需求相关。我们学过二叉树，
// 所以知道它的节点应该包含这些东西。其余的你就要结果实际情况来定义啦。
typedef struct Node
{
    // 节点可以按需求，添加你相加的东西，对于某个对象而言，name/value/id都是常见的。
    string name;
    int data;

    // full binary tree ID，我们的树可能并不是每次都是完全full二叉树，
    // 但是我们可以假定这个树是完全的，那么这个节点的的id应该是多少，其实就是full二叉树层序
    // 遍历（从上往下数，从左往右数）的结果。
    int fullId;

    // 　几叉树，你就几个指针吧
    struct Node *left;
    struct Node *right;
} Node;

// 定义树结构体，其实不定义这个树结构体也可以的。但是从逻辑上来说最好定义，而且这个可以为
// 后面用c++重新实现，就可以平移过去替代class Tree啦。
typedef struct Tree
{
    Node *root;
} Tree;

// 有啦typedef后面用结构体定义就可以省略struct啦，方便多啦。

// 这个加函数声明，因为有些函数可能在被定义前，会被排在文件前面的函数调用，这些函数必须事先声明。
void getNode(Node *node, int data, Node *&theNode, bool &found, Node *&prevLeft, Node *&prevRight);

// 下面就是各种函数的具体实现啦，具体应该实现哪些函数，这应该是在你写代码前就要想好的。
// 这应该和你定义的对象有关，我们写的二叉树，所以根据书本的定义，因为它是一个容器，容器本身就有
// create/destroy/traverse／size/empty之类的。
// 容器内的元素，一般都要实现add/del/update/lookup函数。所以一下你就知道你需要分成这么些函数。
// 千万不能混着一起，因为那样就是一次性的东西，别人复用不了。
// 然后你就按着二叉树的定义，依次去实现，可能就会发现，有些东西是通用的，把这些通用的抽出来单独形成函数。
Tree *createTree()
{
    // 具体函数里要做什么，那就是根据名字，来决定如何实现。函数的名字就代表了要做什么。
    // 我们需要返回一个Tree的结构体指针，来代表新生成的一棵树. 如何生成？就是新建一个结构体，把他
    // 的每个成员初始化好。假定外界还没有输入任何东西进来。
    Tree *tree = (Tree *)malloc(sizeof(Tree));
    tree->root = NULL;
    return tree;
}

// 删除树中的节点，递归方式。因为要free，所以只能是后续遍历。
// 否则，你会先删掉人家，之后还要用别人的node->left或者right。
void destroyTreeNode(Node *node)
{
    if (!node)
    {
        return;
    }

    destroyTreeNode(node->left);
    destroyTreeNode(node->right);
    free(node);
}

// 大部分情况下就是把此前分配的内存的东西，释放回去就行，但全局变量的值就不管啦。
// 但此前create时，tree->root肯定是空的，先可能就不是啦，所以分情况讨论。
void destroyTree(Tree *tree)
{
    if (tree->root != NULL)
    {
        destroyTreeNode(tree->root);
    }

    // 和create时的malloc对应起来。如果是new的就要用delete。
    free(tree);
}

// 新加一个节点，
// 第一个参数是Tree
// 第二个参数是父节点的数值
// 第三个参数是新加节点的数值
// 第四个参数是新节点应该插到父节点的左边还是右边
// 函数参数的设计是考验能力的时候，需要结合实际情况来设置。
void addNode(Tree *tree, int prevData, int newData, bool toLeft)
{
    // 添加节点需要考虑树空和不空时，两种情况。但是无论那种情况，新节点都需要创建出来。
    // 所以第一步就是新建节点，并初始化好，记住malloc出来的，结构体的各个成员可能是
    // 随机值，（就是说你不知道会是多少），但是用new出来的东西，里面都会给你设置成０.
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = newData;
    newNode->left = NULL;
    newNode->right = NULL;
    if (tree->root == NULL)
    {
        // 如果是空树，那就简单啦。
        tree->root = newNode;
        newNode->fullId = 0;
        return;
    }

    // 如果树不空，那我们首要任务就是找到父节点的指针。因为我们需要它来知道它的left或者right
    Node *prevNode = NULL;
    Node *prevPrevLeft = NULL;
    Node *prevPrevRight = NULL;
    bool found = false;
    // getNode其实就是处理容器内元素的lookup方法（增删改查)
    // 注意c/c++的函数只能返回一个参数，要多个，那就定义一个数据结构，很麻烦的。
    // python是可以直接多个的，可以通过输入参数定义为加"&"表示引用的意思，函数内部改了它的话，
    // 在函数结束后，这些改动还是有效的。
    // 对于这个lookup函数，我们是在Tree里面查用户输入的abcd。所以前两个输入参数非常清晰，后面
    // 的几个参数都是我们，函数调用完后，我们希望里面放好了：待查节点的指针, 是否找到啦
    // 待查节点的父节点，如果它的左节点为待查节点，那prevPrevLeft就等于父节点的父节点，否则就是prevPrevRight
    getNode(tree->root, prevData, prevNode, found, prevPrevLeft, prevPrevRight);
    if (!prevNode || !found)
    {
        // 如果输入的东西不在当前树中，那我们就报错。
        // 只要是用户输入进来的，我们都要判断合法性，如果输错啦，程序就挂啦，就属于失败的设计。
        cout << __func__ << " prevNode " << prevNode << " found " << found << endl;
        free(newNode); // 救助任何时候，都要记得规归回内存，这应该是c/c++程序员时刻记心上的事情。
        return;
    }

    if (toLeft && !prevNode->left)
    {
        // 加到左边
        newNode->fullId = prevNode->fullId * 2 + 1; // 满二叉树的特点, 搞编程的人要记住的数学公式就是这么简单
        prevNode->left = newNode;
        return;
    }

    if (!toLeft && !prevNode->right)
    {
        // 加到右边
        newNode->fullId = prevNode->fullId * 2 + 2; // 满二叉树的特点
        prevNode->right = newNode;
        return;
    }

    // 想加到右边或者左边，但是它已经有啦，也是非法情况，提示一下。
    free(newNode); // 要释放内存啦
    printf("%s() prevNode is not empty to add sons\n", __func__);
}

// 获取大小，递归方式，很有趣的，认真体会一下
int getSizeNode(Node *node)
{
    if (node == NULL)
    {
        // 空的话当然就是０啦。
        return 0;
    }
    else
    {
        // 递归获取左子树大小，右子树大小，加上本身的１,非常的直接。
        return getSizeNode(node->left) + 1 + getSizeNode(node->right);
    }
}

// 获取树的高度
int getHeightNode(Node *node)
{
    if (node == NULL)
    {
        return 0;
    }
    else
    {
        // 取左字数高度，右边子树高度，谁高就取谁，然后加上自己的１。非常合理吧。
        int leftHeight = getHeightNode(node->left);
        int rightHeight = getHeightNode(node->right);
        return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
    }
}

// Get node，就是那个增删改查的lookup功能
void getNode(Node *node, int data, Node *&theNode, bool &found, Node *&prevLeft, Node *&prevRight)
{
    // 递归时，其实很时候都会重新做很多事情，所以很多递归中都需要一个指示变量，提示不用干啦，就立刻结束。
    // 看到没有好几个变量参数都是"&引用"，其实就相当于全局变量的作用。
    // 这也可以看来出，如果用c++来实现的话就会简单啦，因为这个&引用，完全可以作为一个class中的成员，类中递归就
    // 自然可以访问啦。用c++重新实现Tree代码量应该会减少的
    if (found)
    {
        return;
    }

    // 只要不空，就中序遍历，检查每一个节点，是否是我要找的节点。
    if (node != NULL)
    {
        // 先左边找
        getNode(node->left, data, theNode, found, prevLeft, prevRight);

        // 找到没？找到啦，根据函数定义和参数设计，做相应的工作就行
        if (node->left && (node->left->data == data))
        {
            prevLeft = node;
        }
        if (node->right && (node->right->data == data))
        {
            prevRight = node;
        }
        if (node->data == data)
        {
            found = true;
            theNode = node;
            // printf("found %c \n", (char)node->data);
        }
        // printf("found %c \n", (char)node->data);
        // 再在右边找
        getNode(node->right, data, theNode, found, prevLeft, prevRight);
    }
}

// 中序遍历, 这些递归的遍历，写起来非常简单，就是那种很符合直觉，但是你可能不知道为什么这样就可以做。
// 可以多看几次王道书中Tree的遍历视频，结合那个隐藏在后面stack。多看，多谢几次。就会都明白的。
void midTraverse(Node *node)
{
    if (node != NULL)
    {
        midTraverse(node->left);
        printf("%c ", (char)node->data);
        midTraverse(node->right);
    }
}

void prevTraverse(Node *node)
{
    if (node != NULL)
    {
        printf("%c ", (char)node->data);
        prevTraverse(node->left);
        prevTraverse(node->right);
    }
}

void postTraverse(Node *node)
{
    if (node != NULL)
    {
        postTraverse(node->left);
        postTraverse(node->right);
        printf("%c ", (char)node->data);
    }
}

// 广度优先遍历，也就是我们说的层序遍历，符合我们直接感受的遍历
// 我们自然而然就会从上到下，从左到右看树中的每个元素，对不对。
// 前面几种遍历都是属于深度优先遍历，用递归实现起来非常简单。
// 不用递归也是可能的那就需要自己用stack来实现。递归其实就是使用
// 系统提供的stack(你看不到而已)。
// 记住这个：深度优先遍历离不开栈stack，广度优先遍历离不开队列queue。
// 广度优先遍历没有递归的说法啊，只有循环检查队列是否为空，不空就继续干活。
// 记住c++中的队列有两种STL：一种是queue，一种是list。
// 我们这里用的是list来模拟队列的。
void broadTraverse(Node *node)
{
    if (!node)
    {
        // 为空当然就什么都不要做啦
        return;
    }

    // 先定义一个队列给遍历时用
    list<Node *> queue;

    // 做事前把自己进队列中
    queue.push_back(node);
    while (!queue.empty())
    {
        // list这种数据结构的pop_front()是把自己删掉，无法返回自己。所以我们先front()返回自己。
        node = queue.front();
        queue.pop_front();

        // 这个printf就是访问我们遍历到某个节点时做的事情，对应visit node。
        // 具体做什么和实际问题相关，这里就是打印就行。
        printf("%c ", (char)node->data);

        // 做完啦了自己，就把自己左孩子进队列，右孩子进队列。
        // 如果是多叉树，那就还需要把n叉孩子进队列。
        if (node->left)
        {
            queue.push_back(node->left);
        }
        if (node->right)
        {
            queue.push_back(node->right);
        }
    }
}

// 空不空
int isEmpty(Tree *tree)
{
    return tree->root == NULL;
}

// 这个和上面那个destroyTreeNode重复啦。可以删掉
// This is means to remove node and its sons, will not touch node's prev nodes
// 这个第二个参数好像没有任何意义，我们全程没有用到，先不管啦，以后再改吧。
void delNode(Node *node, int &data)
{
    if (!node)
    {
        return;
    }

    delNode(node->left, data);
    delNode(node->right, data);
    free(node); // 其实就这一句核心，就是释放内存
}

// 记住经常看到两个函数名字非常类似，感觉干的内容也应该差不多。
// 其实是由于参数不一样。我们这里用到了Tree，但是这个功能需要用到递归，就只能用node结构体做参数啦。
// 第一个参数是树，第二个参数是哪个节点的数值。记住，如果不是叶节点，那就需要递归删掉所有节点。
void deleteNode(Tree *tree, int data)
{
    if (tree->root == NULL)
    {
        printf("%s() tree is empty\n", __func__);
        return;
    }

    // 得找到要删除节点的指针，还有它的父节点，到底要删除节点时左孩子，还是有孩子。
    // 这些东西你在编码前就要想清楚，当然是要根据任务定义来。
    Node *theNode;
    bool found;
    Node *prevLeft = NULL;
    Node *prevRight = NULL;
    getNode(tree->root, data, theNode, found, prevLeft, prevRight);
    if (!found)
    {
        printf("%s() node not found\n", __func__);
        return;
    }

    // 其实这个第二个参数是没有意义的
    delNode(theNode, theNode->data);
    if (prevLeft)
    {
        // 这个节点是人家的左节点，所以左节点为空
        // cout << prevLeft->data << endl;
        prevLeft->left = NULL;
    }
    if (prevRight)
    {
        // 这个节点是人家的右节点，所以右节点为空
        // cout << prevRight->data << endl;
        prevRight->right = NULL;
    }
    if (theNode == tree->root)
    {
        // 这个第一个节点，所以要把树的root设为空
        tree->root = NULL;
    }
}

// 每一个程序，如果是那种交互式的，都需要有一个帮助
// 除非那种一锤子买卖的程序，所有交互式程序，都可以按后面的框架进行
// 对于这种容器类的东西，应该根据定义，就可以得知，从容器本身的操作，从容器内每个元素的操作
void printHelp(void)
{
    cout << endl;
    cout << "Now send different CMDs to test TREE functionality repeatedly, until you input 'exit'" << endl;
    cout << endl;
    cout << "create             to create empty TREE" << endl;
    cout << "destroy            to destroy TREE" << endl;
    cout << "size               to get the size of TREE" << endl;
    cout << "height             to get the height of TREE" << endl;
    cout << "empty              to check if it's empty or not REE" << endl;
    cout << "help               to print the help information again" << endl;
    cout << "exit               to exit test" << endl;
    cout << "add <c> <c> <0/1>  to add new node into TREE" << endl;
    cout << "del <c>            to del one node from TREE" << endl;
    cout << "mid                to mid traverse TREE" << endl;
    cout << "post               to post traverse TREE" << endl;
    cout << "prev               to prev traverse TREE" << endl;
    cout << "broad              to BFS traverse TREE" << endl;
    cout << endl;
}

int main(void)
{
    // 我们所有的操作都是对树的，所有函数离不开它，如果改成c++后就清楚啦。都自动属于同一个类
    Tree *tree = NULL;
    printHelp(); // 先打印帮助信息，告诉用户如何用你的程序, 关键各种命令和格式

    // 这是所有交互式命令的通用框架
    // 就是不停的读一行，然后，解析这一行，推断出是什么命令，然后执行相关命令.
    string cmd;
    while (true)
    {
        // 不停的读取命令行，不输入就一直卡住，等着你从键盘敲会出啊。
        // 不要以别的格式读取啊，具体命令解析进入命令处理后再去做，否则就没法用这个通用框架啦
        getline(cin, cmd);

        // 后面就简单啦，读前面几个字符啦，比较关键字，(这个不就是和那个哈夫曼编码有点关系吗)
        if (cmd.substr(0, strlen("exit")) == string("exit"))
        {
            // 该结束啦
            break;
        }

        if (cmd.substr(0, strlen("help")) == string("help"))
        {
            // 因为是个黑屏程序，交互多啦，可能忘记帮助啦，重新打一遍
            printHelp();
            continue; // 处理完一条正常命令，马上去读下一条可能得命令
        }

        if (cmd.substr(0, strlen("create")) == string("create"))
        {
            if (tree)
            {
                // 树已经创建好啦
                cout << "Tree is already created" << endl;
                continue;
            }

            tree = createTree();
            cout << "Tree is created" << endl;
            continue;
        }

        if (cmd.substr(0, strlen("destroy")) == string("destroy"))
        {
            if (!tree)
            {
                cout << "Tree is not ready" << endl;
                continue;
            }

            destroyTree(tree);
            cout << "Tree is destroyed" << endl;
            tree = NULL; // 更新全局变量，让别人知道树没啦
            continue;
        }

        if (cmd.substr(0, strlen("add")) == string("add"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            // 对于有参数的命令，这就是从已经读进来的命令行中取出参数的方法
            char str1[9];  // 其实就是add
            char prevData; // 哪个父节点
            char data;     // 新节点的值
            int toLeft;    // 左边还是右边啊
            // 用sscanf就行，不会的话baidu一下就学会啦,多用几次自然就学会啦
            sscanf(cmd.c_str(), "%s %c %c %d", str1, &prevData, &data, &toLeft);
            addNode(tree, prevData, data, toLeft);
            continue;
        }

        if (cmd.substr(0, strlen("del")) == string("del"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            char str1[9]; // del
            char data;    // 待删节点的数值
            // 用sscanf就行，不会的话baidu一下就学会啦,多用几次自然就学会啦
            sscanf(cmd.c_str(), "%s %c", str1, &data);
            deleteNode(tree, data);
            cout << cmd << " result " << data << endl;
            continue;
        }

        if (cmd.substr(0, strlen("empty")) == string("empty"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            cout << cmd << " result " << isEmpty(tree) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("size")) == string("size"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            cout << cmd << " result " << getSizeNode(tree->root) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("height")) == string("height"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            cout << cmd << " result " << getHeightNode(tree->root) << endl;
            continue;
        }

        if (cmd.substr(0, strlen("mid")) == string("mid"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            cout << cmd << endl;
            midTraverse(tree->root);
            cout << endl;
            continue;
        }

        if (cmd.substr(0, strlen("prev")) == string("prev"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            cout << cmd << endl;
            prevTraverse(tree->root);
            cout << endl;
            continue;
        }

        if (cmd.substr(0, strlen("post")) == string("post"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            cout << cmd << endl;
            postTraverse(tree->root);
            cout << endl;
            continue;
        }

        if (cmd.substr(0, strlen("broad")) == string("broad"))
        {
            if (!tree)
            {
                // TREE必须先建才能够干别的
                cout << "Please create tree first" << endl;
                continue;
            }
            cout << cmd << endl;
            broadTraverse(tree->root);
            cout << endl;
            continue;
        }

        // 如果到这里来啦，那就前面都没匹配上，很有可能就是用户输入错误啦
        cout << "Unsupported CMD, please check it carefully: " << cmd << endl;
        printHelp();
    }

    if (tree)
    {
        // 防止没有destroy就直接exit啦，最后再删一次
        destroyTree(tree);
        cout << "Tree is destroied\n";
    }

    return 0; // 程序结束啦
}
