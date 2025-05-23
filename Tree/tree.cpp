#include "tree.h"

const char* blue = "#87cefa";
const char* yellow = "#FFFFE0";
const char* green = "#177245";

// add verificator and dump

void PrintNode( FILE* stream, struct Node* node);
int DotNode(FILE* dot_file, struct Node* node, Side agree, int parent_num);

// add asserts

void Ctor(struct Tree* tree)
{
    MyAssert(tree, ARG_IN_FUNC);

    tree->root = (Node*) calloc(1, sizeof(Node));
    MyAssert(tree->root, ALLOC);

    tree->root->type = DEF_TYPE;
    tree->root->data = {};

    tree->root->right = NULL;
    tree->root->left  = NULL;
}

Node* CreateFict(struct Node* left, struct Node* right)
{
    struct Node* node = (Node*) calloc(1, sizeof(Node));
    MyAssert(node, ALLOC);

    node->type = DEF_TYPE;
    node->data = {};

    node->left  = left;
    node->right = right;

    return node;
}

Node* CreateNum(float value)
{
    struct Node* node = (Node*) calloc(1, sizeof(Node));
    MyAssert(node, ALLOC);

    node->type = NUM;
    node->data.value = value;

    return node;
}

Node* CreateVar(struct Node* left, struct Node* right, unsigned int var)
{
    struct Node* node = (Node*) calloc(1, sizeof(Node));
    MyAssert(node, ALLOC);

    node->type = VAR;
    node->data.var = var;

    node->left  = left;
    node->right = right;

    return node;

}

Node* CreateOper(struct Node* left, struct Node* right, Operations oper)
{
    struct Node* node = (Node*) calloc(1, sizeof(Node));
    MyAssert(node, ALLOC);

    node->type = OPER;
    node->data.oper = oper;

    node->left  = left;
    node->right = right;

    return node;
}

void SaveTree(struct Tree* tree)
{
    MyAssert(tree, ARG_IN_FUNC);

    FILE* database = fopen("DataBase.txt", "wb");
    MyAssert(database, FILES);

    PrintNode(database, tree->root);

    fclose(database);
}

// (
// print_node(node->left)
// printf(node->value)
// print_node(node->right)
// )

void PrintNode(FILE* stream, struct Node* node)
{
    MyAssert(stream, ARG_IN_FUNC);

    if(node != NULL)
    {
        fprintf(stream, "( %d ", node->type);

        if(node->type == OPER)
            fprintf(stream, " %d ", node->data.oper);

        else if(node->type == VAR)
            fprintf(stream, " %u ", node->data.var);
        
        else if(node->type == NUM)
            fprintf(stream, " %g ", node->data.value);

        else if(node->type == SORT)
            fprintf(stream, " %d ", node->data.sort);

        PrintNode(stream, node->left);
        PrintNode(stream, node->right);

        fprintf(stream, " )");
    }

    else
    {
        fprintf(stream, "nil");
    }

}

Node* DeleteNode(struct Node* node)
{
    if(node == NULL)
        return NULL;

    node->right = DeleteNode(node->right);
    node->left = DeleteNode(node->left);

    free(node);

    return NULL;
}

void Dtor(struct Tree* tree)
{
    MyAssert(tree, ARG_IN_FUNC);

    tree->size = 0;

    DeleteNode(tree->root);
}

void GraphDump(struct Tree* tree)
{
    MyAssert(tree, ARG_IN_FUNC);

$$$ printf("\n\n%d\n\n", __LINE__);
    FILE* dot_file = fopen("Tree.dot", "wb");
    MyAssert(dot_file, FILES);

    fprintf(dot_file, "digraph G{\n\trankdir=HR;\n");

    fprintf(dot_file, "\tbgcolor = \"#FFF0DB\";\n"); //\tnode[shape=plaintext];\n\tedge[color=white];\n instead of

	fprintf(dot_file, "\tnode0 [shape = Mrecord, style = filled, fillcolor = \"#FFB02E\", label = \"Tree_addres: %p | root: %p | size: %u\"];\n", 
                                                                                                                 tree,      tree->root,tree->size);

    DotNode(dot_file, tree->root, ZERO, 0);

    fprintf(dot_file, "\tnode0 -> node1 [color = \"#000000\"]\n}");

    fclose(dot_file);

    system("dot -Tpng -O Tree.dot");
    system("Tree.dot.png");
}

int DotNode(FILE* dot_file, struct Node* node, Side side, int parent_num)
{
    MyAssert(dot_file, ARG_IN_FUNC);

    const char* color = "#000000";
    int current_num = parent_num + 1;

    if(node == NULL)
        return 0;

    switch (side)
    {
        case LEFT:
        {
            color = blue;
            break;
        }
        
        case RIGHT:
        {
            color = yellow;
            break;
        }

        case ZERO:
        {
            color = green;
            break;
        }
        
        default:
            break;
    }
    
    if(node->type == OPER)
	    fprintf(dot_file, "\tnode%d [shape = Mrecord, style = filled, fillcolor = \"%s\", label = \"addres: %p | type: OPER | oper: %d | left(yes): %p | right(no): %p \"];\n", 
                                 parent_num + 1,                                    color,                  node,                   node->data.oper,node->left,     node->right);

    else if(node->type == VAR)
	    fprintf(dot_file, "\tnode%d [shape = Mrecord, style = filled, fillcolor = \"%s\", label = \"addres: %p | type: VAR | variable: %c | left(yes): %p | right(no): %p \"];\n", 
                                 parent_num + 1,                                    color,                  node,                      node->data.var, node->left,     node->right);
    
    else if(node->type == NUM)
	    fprintf(dot_file, "\tnode%d [shape = Mrecord, style = filled, fillcolor = \"%s\", label = \"addres: %p | type: NUM | value: %d | left(yes):  %p | right(no): %p \"];\n", 
                                 parent_num + 1,                                    color,                  node,                   node->data.value,node->left,     node->right);
    
    int left_child_num = DotNode(dot_file, node->left, LEFT, current_num);

    if(left_child_num != 0)
        fprintf(dot_file, "\tnode%d -> node%d [color = \"#000000\"\"]\n\n", current_num, left_child_num);

    int right_child_num = DotNode(dot_file, node->right, RIGHT, left_child_num);
    
    if(right_child_num != 0)
        fprintf(dot_file, "\tnode%d -> node%d [color = \"#000000\"\"]\n\n", current_num, right_child_num);

    return current_num;
}