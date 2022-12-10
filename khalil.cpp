#include<iostream>
#include<string>
#include<unordered_map>
#include<queue>
#include<fstream>
#include<cmath>
using namespace std; 

unordered_map<char, string> codes; 
unordered_map<char, int> frequencies;
unordered_map<char, double> prob;
double L_average, entropy;

struct huffmanTree 
{ 
    char data;          
    int frequency;
    bool leaf;           
    huffmanTree *left, *right;
    huffmanTree()
    {
        left = right = NULL;
        this->data = ' ';
        this->frequency = 0;
        leaf=0;
    }
    huffmanTree(char data, int frequency, bool leaf) 
    { 
        left = right = NULL; 
        this->data = data; 
        this->frequency = frequency;
        this->leaf=leaf; 
    } 
};

void average_length()
{
    int count=0;
    int temp=0;
    for(auto i : codes)
    {
        temp+=i.second.length();
        count++;
    }
    L_average=(double)temp/count;
}

void probabilities(string message)
{

    cout << "message: " << message.length() << endl;

    for(auto i : frequencies)
    {
        prob[i.first]=(double) i.second/message.length();
    }
}

void calc_entropy()
{
    for(auto i : prob)
    {
        entropy+=(double) i.second*log2(1/i.second);
    }
}

struct compare 
{ 
    bool operator()(huffmanTree* left, huffmanTree* right) 
    { 
        return (left->frequency > right->frequency); 
    } 
};

priority_queue<huffmanTree*, vector<huffmanTree*>, compare> Heap;

void printCodes(huffmanTree* root, string str) 
{ 
    if (!root)
    {
        return;
    } 
    if (root->leaf == 1) 
    {
        cout << root->data << ": " << str << "\n";
    } 
    printCodes(root->left, str + "0"); 
    printCodes(root->right, str + "1"); 
}

void storeCodes(huffmanTree* root, string str) 
{ 
    if (root==NULL)
    {
        return;
    } 
    if (root->leaf == 1)
    {
        codes[root->data]=str;
    } 
    storeCodes(root->left, str + "0"); 
    storeCodes(root->right, str + "1"); 
} 

void allCodes(int size) 
{ 
    huffmanTree *left, *right, *top; 
    for (auto i : frequencies) 
    {
        Heap.push(new huffmanTree(i.first, i.second,1));
    } 
    while (Heap.size() != 1) 
    { 
        left = Heap.top(); 
        Heap.pop(); 
        right = Heap.top(); 
        Heap.pop(); 
        top = new huffmanTree('$', left->frequency + right->frequency,0); 
        top->left = left; 
        top->right = right; 
        Heap.push(top); 
    } 
    storeCodes(Heap.top(), ""); 
}
 
void getFrequency(string str, int n) 
{ 
    for (int i=0; i<str.size(); i++) 
    {
        frequencies[str[i]]++;
    } 
} 

string decoder(huffmanTree* root) 
{
    string result="";
    string message;
    fstream read;
    read.open("output.bin");
    while(!read.eof())
    {
        getline(read,message);
    }
    read.close();
    struct huffmanTree* current = root; 
    for (int i=0; i<message.size(); i++) 
    { 
        if (message[i] == '0') 
        {
           current = current->left;
        } 
        else
        {
            current = current->right;
        }
        if (current->left==NULL && current->right==NULL) 
        { 
            result += current->data; 
            current = root; 
        } 
    } 
    return result; 
}

void compress(string &message)
{
    getFrequency(message, message.length()); 
    allCodes(message.length());
}

void Ratio(string original, string compressed)
{
    average_length();
    cout << "The compression ratio= " << (double)L_average/8 << endl;
}
  
int main() 
{
    ofstream ofs;
    ofs.open("output.bin", ofstream::out | ofstream::trunc);
    ofs.close();
    ofstream write;
    string encodedString;
    string decodedString; 
    string message;
    fstream read;
    read.open("input.txt"); 
    while(!read.eof())
    {
        getline(read,message);
    }
    read.close();
    cout<<"Original message is: "<<message<<endl;
    compress(message);
    write.open("output.bin", ios::out | ios :: binary | ios::app);  
    for (auto i: message) 
    {
        write << codes[i]; 
        encodedString+=codes[i];
    }
    write.close();
    cout << "Encoded Huffman message: " << encodedString << endl; 
    decodedString = decoder(Heap.top()); 
    cout << "Decoded Huffman message: " << decodedString << endl;
    Ratio("input.txt","output.bin");
    probabilities(message);
    calc_entropy();
    cout << "Coding efficiency= " << (double) entropy/L_average << endl;
}