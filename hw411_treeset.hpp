//
//  GVTreeSet.hpp

#ifndef CS263_GVTreeSet
#define CS263_GVTreeSet
#include <stdexcept>
#include <ostream>
using namespace std;
namespace gvsu {
    template<typename Z>
    class TreeSet {
    private:
        class Node;
        /* instance variables */
        Node *rootPtr;   /* poiter to the root node */
        int nums;
        /* inner class(es) */
        struct Node { /* struct: is a class with public members */
            Z data;
            Node *parent; /* pointer to immediate parent */
            Node *left;
            Node *right;
        };
    public:
        class const_iterator;
        /* member functions */

        TreeSet () {
            rootPtr = nullptr;
            nums = 0;
        }

        ~TreeSet() {
            if (rootPtr == nullptr)
                return;
            clearAll(rootPtr);
        }

        /* copy constructor  */
        TreeSet (const TreeSet& twin)
        {
            rootPtr = duplicate (twin.rootPtr);
        }

        /* move constructor */
        TreeSet (TreeSet&& twin)
        {
            rootPtr = twin.rootPtr;
            twin.rootPtr = nullptr;
        }

        int numberOfNodes() const {
            return pnumberOfNodes(rootPtr);
        }

        int numberOfLeaves() const {
            return pnumberOfLeaves(rootPtr);
        }

        int numberOfFullNodes() const {
            return pnumberOfFullNodes(rootPtr);
        }

        /* copy assignment */
        TreeSet&  operator= (const TreeSet& rhs) {
            clearAll(rootPtr);
            rootPtr = duplicate (rhs.rootPtr);
            return *this;
        }

        /* move assignment */
        TreeSet& operator= (TreeSet&& rhs)
        {
            clearAll(rootPtr);
            this->rootPtr = rhs.rootPtr;
            rhs.rootPtr = nullptr;
            return *this;
        }

        const Z& findMax() const {
            return pfindMax(rootPtr);
        }

        bool contains (const Z& key)
        {
            return pcontains(key, rootPtr);
        }

        bool insert (const Z& key) {

            return pinsert(key, rootPtr, nullptr);
        }

        void remove (const Z& key) {
            premove(key, rootPtr);
        }

        vector<Z> levelOrder () const {
            vector<Z> output;
            vector<Node> q;

            if(rootPtr != nullptr){
                q.push_back(*rootPtr);
                for(int i = 0; i < nums; ++i){
                    if(q.at(i).left != nullptr)
                        q.push_back(*(q.at(i)).left);
                    if(q.at(i).right != nullptr)
                        q.push_back(*(q.at(i)).right);
                }
            for(int i = 0; i < nums; ++i){
                output.push_back(q.at(i).data);
            }
            }
            return output;
        }


        void print (ostream& os = std::cout) const {
            print (rootPtr, os, "");
        }

        const_iterator begin() {

            return const_iterator(findMin(rootPtr));
        }

        const_iterator end() {
            return const_iterator(nullptr);
        }

    private:

         Node * findMin(Node *t) const{
            if(t == nullptr)
                return nullptr;
            if(t->left == nullptr)
                return t;
            return findMin(t->left);
        }

        int pnumberOfNodes(Node *t) const {
            if(t == NULL)
                return 0;
            else return 1+pnumberOfNodes(t->left)+ pnumberOfNodes(t->right);
        }

        int pnumberOfFullNodes(Node *t) const {
            if(t == NULL)
                return 0;
            if(t->left != NULL && t->right!=NULL)
                return 1+pnumberOfFullNodes(t->left)+ pnumberOfFullNodes(t->right);
            else if(t->left != NULL) return pnumberOfFullNodes(t->left);
            else pnumberOfFullNodes(t->right);
        }

        int pnumberOfLeaves(Node *t) const {
            if(t == NULL)
                return 0;
            if(t->left == NULL && t->right==NULL)
                return 1;
            else
                return pnumberOfLeaves(t->left)+ pnumberOfLeaves(t->right);
        }

        bool pinsert(const Z& key, Node * & t, Node * p){
        if(t == nullptr){
                t = new Node;
                t->data = key;
                nums++;
                t->left = nullptr;
                t->right = nullptr;
                t->parent = p;
                return true;
            }
            else if(key < t->data)
                pinsert(key, t->left, t);
            else if(key > t->data)
                pinsert(key, t->right, t);

            return false;
        }

        bool pcontains (const Z& key, Node * & t) const
        {
            for(int i = 0; i < nums; ++i){
                if(t == nullptr)
                    return false;
                else if(key < t->data)
                    return pcontains(key, t->left);
                else if(key > t->data)
                    return pcontains(key, t->right);
                return true;
            }
            return false;
        }

        void premove (const Z& key, Node * & t) {
            if(t == nullptr){
                return;
            }
            else if(key < t->data)
                premove(key, t->left);
            else if(key > t->data)
                premove(key, t->right);
            else if(t->left != nullptr && t->right != nullptr){
                t->data = pfindMax(t->left);
                premove(t->data, t->left);
            }
            else{
                Node *old = t;
                if(t->left != nullptr)
                    t = t->left;
                else t = t->right;
                nums--;
                delete old;
            }
        }

        const Z& pfindMax(Node *t) const {
            if (rootPtr != nullptr)
            {
                while(t->right!=nullptr)
                    t=t->right;
                return t->data;
            }
            else
                throw runtime_error("can't findMax() on empty tree");
        }

        Node * duplicate (Node *theOtherNode)
        {
            if (theOtherNode != nullptr)
            {
                Node *n = new Node;
                n->data = theOtherNode->data;
                n->parent = theOtherNode->parent;
                n->left = duplicate(theOtherNode->left);
                n->right = duplicate(theOtherNode->right);
                return n;
            }
            else
                return nullptr;
        }

        void clearAll (Node* & pos)
        {
             if (pos != nullptr)
            {
                clearAll (pos->left);
                clearAll (pos->right);
                delete pos;
                pos = nullptr;
            }
        }

        void print (Node *pos, ostream& os, const string&& path) const
        {
            if (pos) {
                print (pos->left, os, path + "L");
                os << pos->data << " (" << path << ")" << endl;
                print (pos->right, os, path + "R");
            }
        }
    public:
        class const_iterator {
        public:
            Node *current;

            const_iterator (Node *n) : current{n}
            {

            }

            /* copy assignment */
            const_iterator operator= (const const_iterator& rhs) {
                this->current = rhs.current;
                return *this;
            }

            bool operator == (const const_iterator& rhs) const {
                return this->current == rhs.current;
            }
            bool operator != (const const_iterator& rhs) const {
                return this->current != rhs.current;
            }

            const_iterator& operator++ () {
                if(this->current ==nullptr)
                    return *this;

                Z data = this->current->data;

                if(this->current->right == nullptr)
                  while(this->current != nullptr && this->current->data <= data)
                    this->current = this->current->parent;

                else this->current = findMin(this->current->right);

                return *this;
            }

            Node * findMin(Node *t){
                if(t == nullptr)
                    return nullptr;
                if(t->left == nullptr)
                    return t;
                return findMin(t->left);
            }

            Z& operator *() const {
                return current->data;
            }
        };
    };

    template <typename Z>
    ostream& operator<< (ostream& os, const TreeSet<Z>& t)
    {
        t.print (os);
        return os;
    }

}


#endif
