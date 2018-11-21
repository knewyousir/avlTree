#include<iostream>

using std::cin;
using std::cout;
using std::endl;

template<class T>
class RBT;

template<class T>
class RBTNode{
	T data;
	RBTNode<T>* parent, *left, *right;
	bool isRed;
public:
	friend class RBT<T>;
	RBTNode(const T& newData = T(), RBTNode<T>* newParent = nullptr,
		RBTNode<T>* newLeft = nullptr, RBTNode<T>* newRight = nullptr, bool isRed = 1) :
			data(newData), parent(newParent), left(newLeft), right(newRight), isRed(isRed) {;}
	void printInOrder()const{
		if (left != nullptr)
			left->printInOrder();
		cout << data << endl;
		if (right != nullptr)
			right->printInOrder();
	}
};

template<class T>
class RBT{
	RBTNode<T>* root;
public:
	RBT() : root(nullptr) {}

	//Big 3
	RBT(const RBT<T>& rhs) : root(nullptr) {*this = rhs; }
//	virtual ~RBT() { clear(); } //Not needed since we aren't implementing removals
	RBT& operator=(const RBT<T>& rhs);

	void insert(const T& toInsert);
	void insertFix(RBTNode<T>* toFix);
	void singleCR(RBTNode<T>*& target);
	void singleCCR(RBTNode<T>*& target);
	void printInOrder() const { root->printInOrder(); }
};

int main(){

	RBT<int> theTree;

	int input;

	while(true){
		cout<<"Please enter a positive integer, -2 to print, or -1 to exit: ";
		cin>>input;
		if(input == -1)
			break;
		else if(input == -2)
			theTree.printInOrder();
		else if(input > -1)
			theTree.insert(input);
		else
			cout<<"Bad input detected. Try again!"<<endl;
	}

	cout<<"program exited safely!"<<endl;
	return 1;
}

template <class T>
void RBT<T>::insert(const T& toInsert){
//	size++;
	if (root == nullptr){
		cout<<"root == nullptr!"<<endl;
		root = new RBTNode<T>(toInsert);
		root->isRed = 0;
	}
	else{
		RBTNode<T>* temp = root;
		RBTNode<T>* prev = temp;
		while (temp != nullptr){
			prev = temp;
			if (toInsert < temp->data){
				cout<<"slotted left!"<<endl;
				temp = temp->left;

			}
			else{
				cout<<"slotted right!"<<endl;
				temp = temp->right;
			}
		}
		//now temp points to null and prev points to the node we want to insert onto
		if (toInsert < prev->data){ //insert onto the left of Prev
			prev->left = new RBTNode<T>(toInsert, prev);
			cout<<"inserted left!"<<endl;
			if(prev != nullptr)
				insertFix(prev->left);
		}
		else{
			prev->right = new RBTNode<T>(toInsert, prev);
			cout<<"inserted right!"<<endl;
			if(prev != nullptr)
				insertFix(prev->right);
		}
	}
	return;
}

template<class T>
void RBT<T>::insertFix(RBTNode<T>* toFix){
	RBTNode<T>* temp;
	if(root == toFix){
		toFix->isRed = 0; //isBlack
		return;
	}
	while(toFix->parent != nullptr && toFix->parent->isRed == 1){
		cout<<"toFix parent is red!"<<endl;
		RBTNode<T>* gdprnt = toFix->parent->parent;
		if(gdprnt->left == toFix->parent){
			cout<<"toFix parent is on gdprnt left!"<<endl;
			if(gdprnt->right != nullptr){
				cout<<"gdprnt right is not null!"<<endl;
				temp = gdprnt->right;
				if(temp->isRed == 1){
					toFix->parent->isRed = 0;
					temp->isRed = 0;
					gdprnt->isRed = 1;
					toFix = gdprnt;
				}
			}
			else{
				cout<<"gdprnt right is null!"<<endl;
				if(toFix->parent->right == toFix){
					toFix = toFix->parent;
					singleCCR(toFix);
				}
				toFix->parent->isRed = 0;
				gdprnt->isRed = 1;
				singleCR(gdprnt);
			}
		}
		else{
			cout<<"toFix parent is on gdprnt right!"<<endl;
			if(gdprnt->left != nullptr){
				cout<<"gdprnt left is not null!"<<endl;
				temp = gdprnt->left;
				if(temp->isRed == 1){
					toFix->parent->isRed = 0;
					temp->isRed = 0;
					gdprnt->isRed = 1;
					toFix = gdprnt;
				}
			}
			else{
				cout<<"gdprnt left is null!"<<endl;
				if(toFix->parent->left == toFix){
					toFix = toFix->parent;
					singleCR(toFix);
				}
				toFix->parent->isRed = 0;
				gdprnt->isRed = 1;
				singleCCR(gdprnt);
			}
		}
		root->isRed = 0;
	}
	return;
}

template<class T>
void RBT<T>::singleCR(RBTNode<T>*& target){
	cout<<"started single CR!"<<endl;
	if(target->left == nullptr)
		return;
	else{
		RBTNode<T>* temp = target->left;
		if(temp->right != nullptr){
			target->left = target->right;
			temp->right->parent = target;
		}
		else
			target->left = nullptr;
		if(target->parent != nullptr)
			temp->parent = target->parent;
		if(target->parent == nullptr)
			root = temp;
		else{
			if(target == target->parent->left)
				target->parent->left = temp;
			else
				target->parent->right = temp;
		}
		temp->right = target;
		target->parent = temp;
	}
	return;
}

template<class T>
void RBT<T>::singleCCR(RBTNode<T>*& target){
	cout<<"started single CCR!"<<endl;
	if(target->right == nullptr)
		return;
	else{
		RBTNode<T>* temp = target->right;
		if(target->left != nullptr){
			target->right = temp->left;
			temp->left->parent = target;
		}
		else
			target->right = nullptr;
		if(target->parent != nullptr)
			temp->parent = target->parent;
		if(target->parent == nullptr)
			root = temp;
		else{
			if(target == target->parent->left)
				target->parent->left = temp;
			else
				target->parent->right = temp;
		}
		temp->left = target;
		target->parent = temp;
	}
	return;
}
