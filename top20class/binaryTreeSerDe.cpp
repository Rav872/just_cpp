#include<iostream>
#include<vector>
#include<sstream>

using namespace std;

#include"btreeUtil.h"

/*
   Problem Statement:
       Serialize and De-Serialize the Tree into String Format and and back to Tree 
*/

using namespace bTreeUtil;

#define NULLCHAR '#'
#define COMMA ','

class BTreeSerDe {

    private:
        string findInOrder(BTree *root) {
            string result = "";

            if(root == NULL) return "";

            result += findInOrder(root->left);
            result += to_string(root->data) + COMMA;
            result += findInOrder(root->right);

            return result;
        }

        string findPreOrder(BTree *root) {
            string result = "";

            if(root == NULL) return "";

            result += to_string(root->data) + COMMA;

            result += findPreOrder(root->left);
            result += findPreOrder(root->right);

            return result;
        }

    public:
        //vector<char> serString;
        string serString;
        int nodeCounter = 0;

/*
Adhoc ->
    Preorder With Null Links

Using Stack ( recursive ) approach to traverse the tree and then using extra variable on Data Segment to Store result.
*/

        void BTreeSerialise1(BTree *root) {

            if (root == NULL) return;

            //serString.push_back((char) '`' + (root->data)); // Just to Map with characters
            //serString.push_back(COMMA);
            serString += to_string(root->data) + COMMA; // Just to Map with characters
            if(root->left == NULL) {
            /*
                serString.push_back(NULLCHAR); // Taking Extra Memory to Solve the Problem
                serString.push_back(COMMA);
            */
                serString += NULLCHAR;
                serString += COMMA;
            }

            BTreeSerialise1(root->left);

            if(root->right == NULL) {
            /*
                serString.push_back(NULLCHAR);
                serString.push_back(COMMA);
            */
                serString += NULLCHAR;
                serString += COMMA;
            }

            BTreeSerialise1(root->right);
        }

        BTree *buildTree(vector <string> &data) {

            if(data[nodeCounter] == "#")
                return NULL;

            root = createNode(stoi(data[nodeCounter]));

            cout << " Node Addr: " << root << endl;

            nodeCounter++;

            root->left = buildTree(data);

            nodeCounter++;

            root->right = buildTree(data);

            return root;
        }

        BTree *BTreeDeSerialise1(string data)  {
            vector <string> pString;
            int count = 0;
            string substr;
            BTree *root = NULL;

            //stringstream s_stream(data); // Creating String Stream Like it is reading from cin
            if(!data.empty()) // Removing Trailing Comma
                data.erase(std::prev(data.end()));

            istringstream s_stream(data);

            while(s_stream.good()) {
                getline(s_stream, substr, COMMA);
                pString.push_back(substr);
            }

            root = buildTree(pString);

            return root;
        }

/*
    Adhoc ->
        Preorder With Null Links

   Using direct recursive approach to solve the problem.
*/
        string BTreeSerialise2(BTree *root) {
            string left = "";
            string right = "";
            if(root == NULL) return "#,";

            left = BTreeSerialise2(root->left);
            right = BTreeSerialise2(root->right);

            return to_string(root->data) + "," + left + right; 
        }

    /* 
       Level Order with Null Links
     */

        string BTreeSerialise3(BTree *root) {
            string result = "";

            vector<BTree *> queue;
            BTree *tmp = root;

            queue.push_back(tmp);

            while(!queue.empty()) {
                tmp = queue.front();
                queue.erase(queue.begin());

                if(tmp == NULL) {
                    result += "#,"; 
                    continue;
                }

                result += to_string(tmp->data) + COMMA; 

                if(tmp->left != NULL) {
                    queue.push_back(tmp->left);
                }
                else if(tmp->left == NULL)
                    queue.push_back(NULL);

                if(tmp->right != NULL) {
                    queue.push_back(tmp->right);
                }
                else if(tmp->right == NULL)
                    queue.push_back(NULL);
            }
            return result;
        }
/*
    Using PreOrder + InOrder Strings to Serialise
*/
        string BTreeSerialise4(BTree *root) {
            string inOrder = "";
            string preOrder = "";

            inOrder = findInOrder(root);
            cout << "Inorder string is " << inOrder << endl;
            preOrder = findPreOrder(root);
            cout << "Pre order string is " << preOrder << endl;

            return preOrder + NULLCHAR + COMMA + inOrder;
        }
};

int main() {

    BTreeSerDe *obj = new BTreeSerDe();

    cout << "Enter Size of Tree " << endl;
    cin >> bTreeUtil::size;

//    createOneChildTree();
    createRandomTree();
    displayTree(bTreeUtil::root);

/*
    Serialize Strings
*/

    obj->BTreeSerialise1(bTreeUtil::root); // Storing in serString Global Object
    cout << "PreOrder Serialise " << obj->serString << endl;

//    cout << "PreOrder Serialise is " << obj->BTreeSerialise2(bTreeUtil::root) << endl;
//    cout << "Level Order Serialise " << obj->BTreeSerialise3(bTreeUtil::root) << endl;

//    cout << "Serialise String with In+Pre " << obj->BTreeSerialise4(bTreeUtil::root) << endl;

/* 
    De-Serialise Strings
*/
    BTree *node = obj->BTreeDeSerialise1(obj->serString);
    cout << " Addr : " << node << endl;
    displayTree(node);

    return 0;
}
