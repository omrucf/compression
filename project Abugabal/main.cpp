#include <iostream>
#include <string>
#include <cstring>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <unordered_map>
#include <vector>

using namespace std;

struct node
{
    char id;
    int frequency;

    node *left, *right;
};

node *create_new_node(char c, int f, node *left, node *right)
{
    node *n = new node;
    n->id = c;
    n->left = left;
    n->right = right;
    n->frequency = f;
    return n;
}

struct comparator
{
    bool operator()(node *LEFT, node *RIGHT)
    {
        // highest priority item has lowest frequency
        return LEFT->frequency > RIGHT->frequency;
    }
};

void compr(node *root, string s, unordered_map<char, string> &huffcode) // COMPRESSING
{
    if (root == nullptr)
        return;

    if (!root->left && !root->right)
    {
        huffcode[root->id] = s;
    }

    compr(root->left, s + "0", huffcode);
    compr(root->right, s + "1", huffcode);
}

void decompr(node *root, int &i, string s) // DECOMPRESSING
{
    if (root == nullptr)
    {
        return;
    }

    if (!root->left && !root->right)
    {
        cout << root->id;
        return;
    }

    i++;

    if (s[i] == '0')
        decompr(root->left, i, s);
    else
        decompr(root->right, i, s);
}

void tree(node *root, ostream &out)
{
    if (!root)
    {
        return;
    }
    if (!root->left && !root->right)
    {
        out.put('1');
        out.put(root->id);
    }
    else
    {
        out.put('0');
    }
    tree(root->left, out);
    tree(root->right, out);
}

int bintodec(string n)
{

    int value = 0;
    int ix = 0;
    for (int i = n.length() - 1; i >= 0; i--)
    {

        if (n[i] == '1')
        {
            value += pow(2, ix);
        }
        ix++;
    }
    return value;
}

char ASCII(string s)
{
    int decimal = 0;
    decimal = bintodec(s);
    return char(decimal);
}

void compression(string, string);

void HUFFMAN(int ans, string File1, string File2)
{
    if (ans == 1)
    {
        unordered_map<char, int> f;
        unordered_map<char, string> hcodes;
        priority_queue<node *, vector<node *>, comparator> PQ;
        node *root;
        char tc;
        int bits = 0;
        int s = 0;
        string m, n; // m is the string that represents the binary number

        ifstream in;
        ofstream out;

        in.open(File1);
        out.open(File2 + ".bin", ios::binary);

        if (in.fail())
        {
            cout << "Error (original file)";
            exit(0);
        }
        else if (out.fail())
        {
            cout << "Error (writing file)";
            exit(0);
        }

        while (!in.eof())
        {
            in.get(tc);
            if (in.eof())
                break;
            f[tc]++;
        }
        in.close();

        for (auto pair : f)
            PQ.push(create_new_node(pair.first, pair.second, nullptr, nullptr));

        while (PQ.size() != 1)
        {
            node *left = PQ.top();
            PQ.pop();
            node *right = PQ.top();
            PQ.pop();
            s = left->frequency + right->frequency;
            PQ.push(create_new_node('\0', s, left, right));
        }
        root = PQ.top();
        PQ.pop();

        compr(root, "", hcodes);
        tree(root, out);

        in.open(File1);
        while (!in.eof())
        {
            in.get(tc);
            if (in.eof())
                break;
            bits += hcodes[tc].length();
            m += hcodes[tc];
        }
        while (m.length() % 8 != 0)
            m += '0';
        out << bits << " ";

        for (int i = 0; i < m.length(); i += 8)
        {
            if (m.length() - i < 8)
            {
                tc = ASCII(m.substr(i, m.length() - i));
            }
            else
            {
                tc = ASCII(m.substr(i, 8));
            }

            out.put(tc);
        }
        in.close();
        out.close();
        compression(File1, File2);
    }
    else if (ans == 2)
    {
        
    }
    else
    {
        exit(1);
    }
}

ifstream::pos_type size(string filename)
{
    ifstream in(filename, ifstream::ate | ifstream::binary);
    return in.tellg();
}

void compression(string file1, string file2)
{
    float original_size;
    float compressed_size;
    float compression_ratio;

    original_size = size(file1);
    compressed_size = size(file2 + ".bin");
    compression_ratio = (compressed_size) / (original_size);

    cout << "The compression ratio is: " << compression_ratio << endl;
}

int main()
{
    int ans = 1;
    string file1 = "test.txt", file2 = "compressed";
    // cout << "Compress = 1 || DECOMPRESS = 2: ";
    // cin >> ans;

    if (ans == 1 || ans == 2)
    {

        // cout << "Enter the original file name: ";
        // cin >> file1;
        // cout << "Compressed file: ";
        // cin >> file2;

        HUFFMAN(ans, file1, file2);
    }

    else
    {
        cerr << "INVALID SELECTION PROGRAM ENDING.." << endl;
    }

    return 0;
}
