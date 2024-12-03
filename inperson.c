#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_SIZE 2048 

typedef struct Node {
    char ch;
    int freq;
    struct Node *left, *right;
} Node;

int n;                                //字符总数
Node* HT;

Node* newNode(char ch, int freq);
Node* mergeNodes(Node* left, Node* right);
void insertNode(Node* arr[], int *size, Node* newNode);
void sortNodes(Node* arr[], int size);
Node* buildHuffmanTree(char data[], int freq[], int size);

void inorderTraversal(Node* root);
void saveHuffmanTree(Node* root, FILE* file);
Node* loadHuffmanTree(FILE* file);

void Init();                          //初始化函数声明
void CreateHTree(char a[], int w[]);  //建立哈夫曼树声明
void Select(int n, int *s1, int *s2); //从1-n选择权值最小的两个元素下标
void CreateHuffmanCode();             //哈夫曼编码
void Puteverycode();                  //输出每个字符的编码情况
int FILEhfmtree();                    //从文件中读取哈夫曼树
void Codetxt();                       //编码文件
void DeCodetxt();                     //译码文件
void Printcodetxt();                  //打印编码生成的文件
void Printhftree();                   //打印哈夫曼树表

int main()
{
    int i, j;
    char command;
    system("color 0f");
    while (1)
    {
        printf("========== 哈夫曼编/译码器 ==========\n");
        printf("[I] 初始化,建立哈夫曼树并保存\n");
        printf("[E] 编码(Encoding)\n");
        printf("[D] 译码(Decoding)\n");
        printf("[P] 打印代码文件(Print)\n");
        printf("[T] 打印哈夫曼树(Tree Printing)\n");
        printf("[Q] 退出程序\n");
        printf("请输入一个字母选择功能:");
        scanf("%c", &command);
        fflush(stdin);
        if ('Q' == command || 'q' == command)
            break;
        switch (command)
        {
        case 'i':
        case 'I':
            Init();
            break;
        case 'e':
        case 'E':
            Codetxt();
            break;
        case 'd':
        case 'D':
            DeCodetxt();
            break;
        case 'p':
        case 'P':
            Printcodetxt();
            break;
        case 't':
        case 'T':
            Printhftree();
            break;
        default:
            printf("输入的命令有误！\n");
            system("pause");
            system("cls");
            break;
        }
    }
    system("pause");
    return 0;
}

void Init()
{ //初始化函数
    int i, flag, com;
    FILE *fp;
    char ABC[27], temp;
    int ABCnums[27];
    int filenums[27];
    system("cls");
    printf("\n请输入字符集大小N, 0< n <=28\n");
    scanf("%d", &n);
    while (n < 1 || n >= MAX_TREE_SIZE)
    {
        printf("字符集过大，请重新输入\n");
        fflush(stdin);
        scanf("%d", &n);
    }

    printf("\n输入大写字母或空格和频度例如\nA12\nC23\n\n");
    for (i = 0; i < n; i++)
    {
        fflush(stdin);
        ABC[i] = getc(stdin);
        scanf("%d", &ABCnums[i]);
    }

    printf("字符  ");
    for (i = 0; i < n; i++)
    {
        if (ABC[i] == ' ')
       printf("空格 ");
        else
            printf("%-4c ", ABC[i]);
    }
    printf("\n频度  ");
    for (i = 0; i < n; i++)
        printf("%-4d ", ABCnums[i]);
    printf("\n");


    HT=buildHuffmanTree(ABC,ABCnums,n);
    printf("\n%-5s%-5s%-7s%-7s\n", "字符", "权值", "左孩子", "右孩子");
    inorderTraversal(HT);

    fp = fopen("hfmTree.txt", "w");
    if (fp == NULL)
    {
        printf("hfmTree.txt文件创建错误\n");
        system("pause");
        system("cls");
        return;
    }
    fprintf(fp, "%-5s%-5s%-5s%-11s\n", "字符", "权值", "是否为叶子节点");
    saveHuffmanTree(HT,fp);
    fclose(fp);
    printf("\n哈夫曼树已经写入hfmTree.txt中\n");
    system("pause");
    system("cls");
    fflush(stdin);
}


Node* newNode(char ch, int freq) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->ch = ch;
    temp->freq = freq;
    temp->left = temp->right = NULL;
    return temp;
}

Node* mergeNodes(Node* left, Node* right) {
    Node* temp = newNode('\0', left->freq + right->freq);
    temp->left = left;
    temp->right = right;
    return temp;
}

void insertNode(Node* arr[], int *size, Node* newNode) {
    arr[*size] = newNode;
    (*size)++;
}

void sortNodes(Node* arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j]->freq > arr[j + 1]->freq) {
                Node* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

Node* buildHuffmanTree(char data[], int freq[], int size) {
    Node* nodes[MAX_TREE_SIZE];
    int n = 0;
    for (int i = 0; i < size; i++) {
        nodes[n++] = newNode(data[i], freq[i]);
    }
    while (n > 1) {
        sortNodes(nodes, n);
        Node* left = nodes[0];
        Node* right = nodes[1];
        Node* merged = mergeNodes(left, right);
        for (int i = 2; i < n; i++) {
            nodes[i - 2] = nodes[i];
        }
        n -= 2;
        nodes[n++] = merged;
    }
    return nodes[0];
}

// 中序遍历：左子树 -> 根节点 -> 右子树
void inorderTraversal(Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);   // 遍历左子树
        printf("%-5s%-5d%-5s%-5s%-5s%-5s\n", root->ch, root->freq, root->left->ch, root->left->freq, root->right->ch, root->right->freq);      // 输出根节点
        inorderTraversal(root->right);  // 遍历右子树
    }
}


void printCodes(Node* root, char* str, int top) {
    if (root->left) {
        str[top] = '0';
        printCodes(root->left, str, top + 1);
    }
    if (root->right) {
        str[top] = '1';
        printCodes(root->right, str, top + 1);
    }
    if (!root->left && !root->right) {
        str[top] = '\0';
        printf("%c: %s\n", root->ch, str);
    }
}

void encode(Node* root, char* str, int top, char codes[256][256]) {
    if (root->left) {
        str[top] = '0';
        encode(root->left, str, top + 1, codes);
    }
    if (root->right) {
        str[top] = '1';
        encode(root->right, str, top + 1, codes);
    }
    if (!root->left && !root->right) {
        str[top] = '\0';
        strcpy(codes[(int)root->ch], str);
    }
}

void decode(Node* root, char* binaryStr, char* decodedStr) {
    Node* current = root;
    int index = 0, decodedIndex = 0;
    while (binaryStr[index]) {
        if (binaryStr[index] == '0')
            current = current->left;
        else
            current = current->right;
        if (!current->left && !current->right) {
            decodedStr[decodedIndex++] = current->ch;
            current = root;
        }
        index++;
    }
    decodedStr[decodedIndex] = '\0';
}

void saveHuffmanTree(Node* root, FILE* file) {
    if (root == NULL) {
        return;
    }

    // 保存当前节点的数据
    // 使用字符和频率，0表示不是叶子节点，1表示叶子节点
    if (root->left == NULL && root->right == NULL) {
        // 叶子节点
        fprintf(file, "%c %d 1\n", root->ch, root->freq);
    } else {
        // 非叶子节点
        fprintf(file, "%c %d 0\n", root->ch, root->freq);
    }

    // 递归保存左子树和右子树
    saveHuffmanTree(root->left, file);
    saveHuffmanTree(root->right, file);    
}


Node* loadHuffmanTree(FILE* file) {
    char ch;
    int freq, isLeaf;
    
    // 读取节点数据
    if (fscanf(file, " %c %d %d", &ch, &freq, &isLeaf) != 3) {
        return NULL; // 文件读取完毕
    }
    
    // 创建当前节点
    Node* node = createNode(ch, freq);
    
    // 如果是非叶子节点，递归加载左右子树
    if (isLeaf == 0) {
        node->left = loadHuffmanTree(file);
        node->right = loadHuffmanTree(file);
    }

    return node;
}



int main() {
    char arr[] = "example data";
    int freq[] = {5, 9, 12, 13, 16, 45};

    int size = sizeof(arr) / sizeof(arr[0]) - 1;

    Node* root = buildHuffmanTree(arr, freq, size);

    char str[100];
    printCodes(root, str, 0);

    char codes[256][256] = {0};
    encode(root, str, 0, codes);

    char* text = "example";
    char binaryStr[1000] = {0};
    for (int i = 0; text[i]; i++) {
        strcat(binaryStr, codes[(int)text[i]]);
    }

    printf("Encoded: %s\n", binaryStr);

    char decodedStr[1000];
    decode(root, binaryStr, decodedStr);
    printf("Decoded: %s\n", decodedStr);

    return 0;
}
