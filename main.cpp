#include<iostream>
#include<string>
#include<fstream>
#include<queue>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
using namespace std;

void gotoxy(int x, int y) {//go to x and y for gui
	COORD pos;   pos.X = x; pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
class Products;
class PNode {
public:
	string ProductName;
	int ProductID;
	float ProductPrice;
	int ProductQuantity;
	PNode* next;
	PNode* pre;
	PNode() {
		ProductID = 0;
		ProductName = '\0';
		ProductPrice = 0.0;
		ProductQuantity = 1;
		next = nullptr;
		pre = nullptr;
	}
	PNode(string name, int id, float price, int quan) {
		ProductName = name;
		ProductID = id;
		ProductPrice = price;
		ProductQuantity = quan;
		next = nullptr;
		pre = nullptr;
	}
	~PNode()
	{
		if (next != nullptr) {
			delete next;
			next = nullptr;
		}
		if (pre != nullptr) {
			delete pre;
			pre = nullptr;
		}
	}
};
class SeenProducts {
public:
	PNode* head;
	PNode* tail;
	SeenProducts() {
		head = nullptr;
		tail = nullptr;
	}
	void append(PNode* node) { //for selecting fom ll when popped to user
		if (node == nullptr) {
			return;
		}
		PNode* newobj = new PNode();
		newobj->ProductID = node->ProductID;
		newobj->ProductName = node->ProductName;
		newobj->ProductPrice = node->ProductPrice;
		if (head == nullptr) {
			head = newobj;
			tail = newobj;
			newobj->pre = nullptr;
			newobj->next = nullptr;
		}
		else {
			tail->next = newobj;
			newobj->pre = tail;
			tail = newobj;
			newobj->next = nullptr;
		}
	}
};
class AddToCart;
class ProductStack {//product stack to pop 10 products at a time to user 
public:
	PNode* top;
	PNode* head;
	ProductStack() {
		top = nullptr;
		head = nullptr;
	}
	void PopToUser(AddToCart& cart);
	void insertList(int id, string name, float pri, int quan) {
		if (head == nullptr) {
			head = new PNode(name, id, pri, quan);
			top = head;
			top->pre = nullptr;
		}
		else {
			PNode* obj = new PNode(name, id, pri, quan);
			top->next = obj;
			obj->pre = top;
			top = top->next;

		}
	}


};
class ItemNode {
public:
	int itemId, quantity;
	string itemName;
	float itemPrice;
	ItemNode* next;
	ItemNode(int Id, int quan, string name, float price) {
		itemId = Id;
		quantity = quan;
		itemName = name;
		itemPrice = price;
		next = nullptr;
	}
	~ItemNode() {
		delete next;
	}
};

class ItemCategory {
public:
	string CategoryName;
	ItemNode* head;
	ItemCategory() { head = NULL; }
	ItemCategory(string name) {
		CategoryName = name;
		head = nullptr;
	}
	void AddItem(int id, int quan, string name, float price) { //added items to each node related to that category
		ItemNode* newItem = new ItemNode(id, quan, name, price);
		newItem->next = head;
		head = newItem;
	}

	void DisplayItemsFromStack(ProductStack& stack) {
		ItemNode* temp = head;
		while (temp != nullptr) {
			string productname = temp->itemName;
			int prodid = temp->itemId;
			float prop = temp->itemPrice;
			int quantity = temp->quantity;
			stack.insertList(prodid, productname, prop, quantity);//to insert items in stack
			temp = temp->next;
		}
	}
	void DisplayItems() { //displayig items of one category
		cout << "Items in " << "Category: " << CategoryName << endl;
		cout << "Item ID\t\tItem Name\t\tItem Price\t\tAvailability" << endl;
		ItemNode* temp = head;
		while (temp != nullptr) {
			cout << temp->itemId << "\t\t" << temp->itemName << "\t\t" << temp->itemPrice << "\t";
			if (temp->quantity > 0) {
				cout << "Available" << endl;
			}
			else {
				cout << "Not Available" << endl;
			}
			temp = temp->next;
		}cout << endl;
	}
};
class Items : public ItemCategory {//different linked list for items stock . will only be accessible to admin
public:
	ItemCategory* Shoes;
	ItemCategory* Cosmetics;
	ItemCategory* Perfumes;
	ItemCategory* Stationary;
	ItemCategory* Clothes;
	Items() {
		Shoes = new ItemCategory("Shoes");
		Cosmetics = new ItemCategory("Cosmetics");
		Perfumes = new ItemCategory("Perfumes");
		Stationary = new ItemCategory("Stationary");
		Clothes = new ItemCategory("Clothes");
	}
	void DisplayCategories() {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "                           --------------------------------------------------------------";
		cout << endl;
		cout << "                           ---------- C A T E G O R I E S -----------*";
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "                           1: SHOES\t2: COSMETICS\t3: PERFUMES\t4: STATIONARY\t5: CLOTHES" << endl;
	}
	void DeleteCategory(ItemCategory* category) {
		ItemNode* temp = category->head;
		while (temp != nullptr) {
			ItemNode* del = temp;
			temp = temp->next;
			delete del;
		}
		delete category;
	}
	void DisplayAllItems() {
		Shoes->DisplayItems();
		Cosmetics->DisplayItems();
		Perfumes->DisplayItems();
		Stationary->DisplayItems();
		Clothes->DisplayItems();
	}
	~Items() {
		DeleteCategory(Shoes);
		DeleteCategory(Cosmetics);
		DeleteCategory(Perfumes);
		DeleteCategory(Stationary);
		DeleteCategory(Clothes);
	}
};
void Sort(PNode** arr, int n);
class AddToCart {
public:
	PNode* head;
	PNode* tail;
	int count;
	AddToCart() {
		head = nullptr;
		tail = nullptr;
		count = 0;
	}
	void insert_at_end(PNode* obj) { //customer chose the items and added it to his cart(ll).
		if (obj == nullptr) {
			return;
		}
		PNode* newobj = new PNode;
		newobj->ProductID = obj->ProductID;
		newobj->ProductName = obj->ProductName;
		newobj->ProductPrice = obj->ProductPrice;
		newobj->ProductQuantity = obj->ProductQuantity;
		PNode* temp = head;
		PNode* temp2 = tail;
		if (temp == nullptr) {
			head = newobj;
			tail = newobj;
			newobj->pre = nullptr;
			newobj->next = nullptr;
		}
		else {
			tail->next = newobj;
			newobj->pre = tail;
			tail = newobj;
		}
		count++;
	}
	void Delete(int id) {
		PNode* temp = head;
		if (head->ProductID == id) {
			PNode* del = head;
			head = head->next;
			delete del;
			head->pre = nullptr;
			count--;
			cout << "Successfully deleted" << endl;
			return;
		}
		else if (tail->ProductID == id) {
			PNode* del = tail;
			tail = tail->pre;
			delete del;
			tail->next = nullptr;
			count--;
			cout << "Successfully deleted" << endl;
			return;
		}
		else {
			while (temp != nullptr) {
				if (temp->ProductID == id) {
					PNode* del = temp;
					if (temp->pre)
						temp->pre->next = temp->next;
					if (temp->next)
						temp->next->pre = temp->pre;
					delete temp;
					count--;
					cout << "Successfully deleted" << endl;
					return;
				}
				temp = temp->next;
			}
		}
		cout << "Could not find Product ID" << endl;
		return;
	}

	void printbill() {
		if (head == nullptr) {
			cout << "Empty list" << endl;
			return;
		}
		else {
			double total = 0;

			PNode** arr = new PNode * [count];
			for (int i = 0; i < count; i++) {
				arr[i] = NULL;
			}
			int i = 0;
			PNode* tr = head;
			while (tr) {
				if (tr != NULL) {
					arr[i] = tr;
				}

				tr = tr->next;
				i++;
			}
			Sort(arr, count);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "                                SORTED PRODUCTS IN QAUNTITY" << endl;
			for (int i = 0; i < count; i++) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "                           " << arr[i]->ProductID << "\t" << arr[i]->ProductName << "\t" << arr[i]->ProductPrice << "\t" << arr[i]->ProductQuantity << endl;
				//total = total + (price[i] * arr[i][1]);
			}
			PNode* temp = head;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "                              ----------PRODUCTS IN CART--------" << endl;
			while (temp != nullptr) {
				if (temp) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
					cout << "                           " << temp->ProductID << "\t" << temp->ProductName << "\t" << temp->ProductPrice << endl;
					total = total + (temp->ProductPrice * temp->ProductQuantity);
					temp = temp->next;
				}
				else {
					break;
				}
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "                                         TOTAL AMOUNT : " << total << endl;
			cout << "                           --------------------------------------------------------------";
			cout << endl;
			cout << "                           -------------------------------";
			cout << endl;
			system("pause");
			system("cls");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		}
	}

	void travese_print() {
		if (head == nullptr) {
			cout << "Empty list" << endl;
		}
		else {
			cout << "Linked list is :" << endl;
			PNode* temp;
			temp = head;
			while (temp->next != nullptr) {
				cout << temp->ProductID << endl;
				cout << temp->ProductName << endl;
				cout << temp->ProductPrice << endl;
				temp = temp->next;
			}
			cout << temp->ProductID << endl;
			cout << temp->ProductName << endl;
			cout << temp->ProductPrice << endl;
		}

	}

};

void ordernow(ItemCategory* shoes, ItemCategory* cos, ItemCategory* clothes, ItemCategory* stat, ItemCategory* per, AddToCart& obj) {
	if (obj.head == nullptr) {
		cout << "Empty list" << endl;
		return;
	}
	else {
		PNode* temp = obj.head;
		ItemNode* node = shoes->head;
		while (node) {
			temp = obj.head;
			while (temp) {
				if (temp->ProductName == node->itemName) {
					node->quantity = node->quantity - temp->ProductQuantity;
				}
				temp = temp->next;
			}
			node = node->next;
		}
		temp = obj.head;
		node = cos->head;
		while (node) {
			temp = obj.head;
			while (temp) {
				if (temp->ProductName == node->itemName) {
					node->quantity = node->quantity - temp->ProductQuantity;
				}
				temp = temp->next;
			}
			node = node->next;
		}
		temp = obj.head;
		node = per->head;
		while (node) {
			temp = obj.head;
			while (temp) {
				if (temp->ProductName == node->itemName) {
					node->quantity = node->quantity - temp->ProductQuantity;
				}
				temp = temp->next;
			}
			node = node->next;
		}
		temp = obj.head;
		node = stat->head;
		while (node) {
			temp = obj.head;
			while (temp) {
				if (temp->ProductName == node->itemName) {
					node->quantity = node->quantity - temp->ProductQuantity;
				}
				temp = temp->next;
			}
			node = node->next;
		}
		temp = obj.head;
		node = clothes->head;
		while (node) {
			temp = obj.head;
			while (temp) {
				if (temp->ProductName == node->itemName) {
					node->quantity = node->quantity - temp->ProductQuantity;
				}
				temp = temp->next;
			}
			node = node->next;
		}
		double total = 0;

		PNode** arr = new PNode * [obj.count];
		for (int i = 0; i < obj.count; i++) {
			arr[i] = NULL;
		}
		int i = 0;
		PNode* tr = obj.head;
		while (tr) {
			if (tr != NULL) {
				arr[i] = tr;
			}

			tr = tr->next;
			i++;
		}
		Sort(arr, obj.count);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "                                SORTED PRODUCTS IN QAUNTITY" << endl;
		for (int i = 0; i < obj.count; i++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "                           " << arr[i]->ProductID << "\t" << arr[i]->ProductName << "\t" << arr[i]->ProductPrice << "\t" << arr[i]->ProductQuantity << endl;
			//total = total + (price[i] * arr[i][1]);
		}
		temp = obj.head;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "                              ----------PRODUCTS IN CART--------" << endl;
		while (temp != nullptr) {
			if (temp) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "                           " << temp->ProductID << "\t" << temp->ProductName << "\t" << temp->ProductPrice << endl;
				total = total + (temp->ProductPrice * temp->ProductQuantity);
				temp = temp->next;
			}
			else {
				break;
			}
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "                                         TOTAL AMOUNT : " << total << endl;
		cout << "                           --------------------------------------------------------------";
		cout << endl;
		cout << "                           -------------------------------";
		cout << endl;
		system("pause");
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
}
class CNode
{
public:
	int id;
	string customerName;
	string email;
	string address;
	int timeresume;
	//doubly ll -> product
	Products* products;
	CNode* next;
	CNode() {
		customerName = "0";
		id = 0;
		email = "0";
		address = "0";
		next = nullptr;
		timeresume = 0;
		products = nullptr;
	}
	CNode(int id, string cn, string em, string add, int it)
	{
		customerName = cn;
		this->id = id;
		email = em;
		address = add;
		next = nullptr;
		timeresume = it;
		products = nullptr;
	}
};


void displayBanner() {

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	cout << endl;
	cout << endl;
	cout << endl;
	cout << "                                                        W     W      A      N     N" << endl;
	cout << "                                                        W     W     A A     N N   N" << endl;
	cout << "                                                        W  W  W    AAAAA    N  N  N" << endl;
	cout << "                                                        WW   WW   A     A   N   N N" << endl;
	cout << "                                                        W     W  A       A  N     N" << endl;
	cout << endl;
	cout << endl;
	cout << endl;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY); // Reset to default intensity
}



void swapp(PNode*& arr, PNode*& arr2) {
	int temp = arr->ProductID;
	arr->ProductID = arr2->ProductID;
	arr2->ProductID = temp;
	string name = arr->ProductName;
	arr->ProductName = arr2->ProductName;
	arr2->ProductName = name;
	float price = arr->ProductPrice;
	arr->ProductPrice = arr2->ProductPrice;
	arr2->ProductPrice = price;
	int te = arr->ProductQuantity;
	arr->ProductQuantity = arr2->ProductQuantity;
	arr2->ProductQuantity = te;
}

void Sort(PNode** arr, int n) {
	int gap = n;
	int i = 0;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (arr[i]->ProductID > arr[i + 1]->ProductID) {
				swap(arr[i], arr[i + 1]);
			}
		}
	}
}

void ProductStack::PopToUser(AddToCart& cart) {
	if (head != nullptr) {
		SeenProducts sProduct;
		cout << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "                                         ID\tProduct Name\t\tPrice\tQuantity" << endl;
		for (int i = 0; i < 5; i++) {
			PNode* temp = top;
			sProduct.append(temp);
			if (top) {
				cout << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				cout << "                                         " << top->ProductID << "\t" << top->ProductName << "\t\t" << top->ProductPrice << "\t" << top->ProductQuantity << endl;
			}
			else {
				break;
			}
			top = top->pre;
		}
		while (1) {
			int entid;
			cout << "ENTER 0 TO EXIT\nENTER ID: "; cin >> entid;
			int ans;
			if (entid == 0) {
				return;
			}
			cout << "Press 1 For Add to\nPress 2 For Removing" << endl;
			cin >> ans;
			PNode* temp = sProduct.head;
			while (temp->next != NULL && temp->ProductID != entid) {
				if (temp->ProductID == entid)
					break;
				else
					temp = temp->next;
			}
			if (temp == NULL) {
				continue;
			}
			if (temp->ProductID == entid && ans == 1) {
				cout << "Enter Quantity "; cin >> temp->ProductQuantity;
				cart.insert_at_end(temp);
			}
			else if (temp->ProductID == entid && ans == 2) {
				cart.Delete(entid);
			}
			cout << "Press 3 For Viewing the cart\nPress 4 For Printing bill\nPress 0 For Exit" << endl;
			cin >> ans;
			if (ans == 3) {
				cart.travese_print();
			}
			else if (ans == 4) {
				system("pause");
				system("cls");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "                           --------------------------------------------------------------";
				cout << endl;
				cout << "                           ------------- B I L L I N G -----------";
				cout << endl;
				cart.printbill();
			}
			else if (ans == 0) {
				return;
			}
		}
	}
}

class Customers
{
public:
	CNode* head;
	CNode* tail;
	string filename = "customers.txt";
	static int customerCount;
	Customers()
	{
		head = nullptr;
		tail = nullptr;
	}
	void insertlist(int id, string cn, string em, string add, int ti)
	{
		CNode* customer = new CNode(id, cn, em, add, ti);

		if (head == nullptr)
		{
			head = customer;
			tail = customer;
		}
		else
		{
			CNode* temp = head;
			while (temp->next != NULL) {
				temp = temp->next;
			}
			temp->next = customer;
		}
		customerCount++;
	}
	CNode* findout(string name, int i = 0, CNode* node = nullptr) {
		if (node == nullptr && i == 0) {
			node = head;
		}
		else if (node == nullptr && i != 0) {
			return nullptr;
		}
		if (node->customerName == name) {
			return node;
		}
		return findout(name, i + 1, node->next);

		/*	CNode* temp = head;
			if (head->next == NULL) {
				if (temp->customerName == name) {
					return temp;
				}
			}
			while (temp->next != NULL) {
				if (temp->customerName == name) {
					return temp;
				}
				temp = temp->next;
			}
			return NULL;*/
	}
	bool storingcust(CNode* cust) {
		ofstream outFile(filename, ios::app);
		if (!outFile.is_open()) {
			cout << "Unable to sign up!" << endl;
			return false;
		};
		outFile << cust->customerName << "\t" << cust->email << "\t" << cust->address << "\t" << cust->id << "\t" << cust->timeresume << endl;
		outFile.close();
		return true;
	}
	bool restoringcust() {
		ifstream inFile(filename);
		if (!inFile.is_open()) {
			//cout << "Error!" << endl;
			return false;
		}
		CNode* temp = new CNode();
		while (!inFile.eof()) {
			inFile >> temp->customerName >> temp->email >> temp->address >> temp->id >> temp->timeresume;
			if (head != NULL) {
				CNode* temp2 = head;
				CNode* par = head;
				while (temp2 != NULL) {
					if (temp2->customerName == temp->customerName && temp2->timeresume != temp->timeresume) {
						if (head->next == NULL) {
							CNode* del = temp2;
							delete temp2;
							temp2 = NULL;
							head = NULL;
							break;
						}
						else {
							if (temp2->next != NULL) {
								par->next = temp2->next;
								CNode* del = temp2;
								delete(del);
								del = NULL;
								break;
							}
							else {
								par->next = NULL;
								CNode* del = temp2;
								delete(del);
								del = NULL;
								break;
							}

						}

					}
					par = temp2;
					temp2 = temp2->next;
				}


			}
			insertlist(temp->id, temp->customerName, temp->email, temp->address, temp->timeresume);
		}
		inFile.close();
		return false;
	}
	/*void Dequeue()
	{
		if (head == nullptr) {
			cout << "Empty list!" << endl;
			return;
		}
		CNode* t = head;
		head = head->next;
		delete t;
		customerCount--;
	}*/

	void traverse()
	{
		CNode* t = head;
		while (t) {
			cout << t->id << "\t" << t->email << "\t" << t->customerName << "\t" << t->address << endl;
			t = t->next;
		}

	}

	//Merge Sort
	//void MSort(string* arr, int left, int mid, int right)
	//{
	//	int n1 = mid - left + 1;
	//	int n2 = right - mid;

	//	string* leftarr = new string[n1];
	//	string* rightarr = new string[n2];
	//	for (int i = 0; i < n1; i++)
	//	{
	//		leftarr[i] = arr[left + i];
	//	}
	//	for (int j = 0; j < n2; j++)
	//	{
	//		rightarr[j] = arr[mid + 1 + j];
	//	}

	//	int i = 0, j = 0, k = left;
	//	while (i < n1 && j < n2)
	//	{
	//		if (leftarr[i] <= rightarr[j])
	//		{
	//			arr[k] = leftarr[i];
	//			i++;
	//		}
	//		else
	//		{
	//			arr[k] = rightarr[j];
	//			j++;
	//		}
	//		k++;
	//	}

	//	while (i < n1)
	//	{
	//		arr[k] = leftarr[i];
	//		i++; k++;
	//	}

	//	while (j < n2)
	//	{
	//		arr[k] = rightarr[j];
	//		j++; k++;
	//	}

	//}

	//void Sort(string* arr, int left, int right)
	//{
	//	if (left < right)
	//	{
	//		int mid = left + (right - left) / 2;
	//		Sort(arr, left, mid);
	//		Sort(arr, mid + 1, right);
	//		MSort(arr, left, mid, right);
	//	}
	//}
};
int Customers::customerCount = 0;
void addItem(Items& obj) {
	int idShoes = 1000, idCos = 1005, idPerf = 1010, idStat = 1015, idClothes = 1020;
	obj.Shoes->AddItem(idShoes, 10, "SportsShoes", 499);
	idShoes++;
	obj.Shoes->AddItem(idShoes, 10, "CasualShoes", 249);
	idShoes++;
	obj.Shoes->AddItem(idShoes, 10, "Heels      ", 1299);
	idShoes++;
	obj.Shoes->AddItem(idShoes, 10, "Sneakers", 4999);
	idShoes++;
	obj.Shoes->AddItem(idShoes, 10, "Flats     ", 749);

	obj.Cosmetics->AddItem(idCos, 10, "Foundation", 799);
	idCos++;
	obj.Cosmetics->AddItem(idCos, 10, "Lipstick", 499);
	idCos++;
	obj.Cosmetics->AddItem(idCos, 10, "Concealer", 549);
	idCos++;
	obj.Cosmetics->AddItem(idCos, 10, "Mascara     ", 399);
	idCos++;
	obj.Cosmetics->AddItem(idCos, 10, "Eye-Liner", 399);

	obj.Perfumes->AddItem(idPerf, 10, "FloralPerfume", 699);
	idPerf++;
	obj.Perfumes->AddItem(idPerf, 10, "CitrusPerfume", 799);
	idPerf++;
	obj.Perfumes->AddItem(idPerf, 10, "StrongPerfume", 999);
	idPerf++;
	obj.Perfumes->AddItem(idPerf, 10, "FruityPerfume", 899);
	idPerf++;
	obj.Perfumes->AddItem(idPerf, 10, "SpicyPerfume", 599);

	obj.Stationary->AddItem(idStat, 10, "Pencils    ", 10.4);
	idStat++;
	obj.Stationary->AddItem(idStat, 10, "NoteBooks", 99.0);
	idStat++;
	obj.Stationary->AddItem(idStat, 10, "Art & Craft Box", 499);
	idStat++;
	obj.Stationary->AddItem(idStat, 10, "Paints    ", 399);
	idStat++;
	obj.Stationary->AddItem(idStat, 10, "Markers      ", 599);

	obj.Clothes->AddItem(idClothes, 10, "Eastern-Small-Sizes", 1299);
	idClothes++;
	obj.Clothes->AddItem(idClothes, 10, "Western-Small-Sizes", 1599);
	idClothes++;
	obj.Clothes->AddItem(idClothes, 10, "Western-Medium-Sizes", 1299);
	idClothes++;
	obj.Clothes->AddItem(idClothes, 10, "Western-Large-Sizes", 1299);
	idClothes++;
	obj.Clothes->AddItem(idClothes, 10, "Eastern-Medium-Sizes", 1299);
	idClothes++;
	obj.Clothes->AddItem(idClothes, 10, "Eastern-Large-Sizes", 1299);
	idClothes++;
	obj.Clothes->AddItem(idClothes, 10, "Eastern-XL-Sizes", 1299);
}
class Admin :public AddToCart {
public:
	string FileName = "Credentials.txt";
	string FileName2 = "Cart.txt";
	bool SignUp(string username, string password, string adddress, string email) {
		ofstream outFile(FileName, ios::app);
		if (!outFile.is_open()) {
			cout << "Unable to sign up!" << endl;
			return false;
		}
		outFile << username << "\t" << password << "\t" << adddress << "\t" << email << endl;
		outFile.close();
		return true;
	}
	bool checkCredentials(string userName, string password) { //to check sign-in that when signed up, the database should have that in file and it matches the one with ehich we signed up
		ifstream inFile(FileName);
		if (!inFile.is_open()) {
			cout << "Error!!!" << endl;
			return false;
		}
		string uName, Pass, add, email;
		while (!inFile.eof()) {
			inFile >> uName >> Pass >> add >> email;
			if (uName == userName && Pass == password) {
				return true;
			}
		}
		inFile.close();
		return false;
	}
	bool storingammount(CNode* cust, AddToCart& cart) {
		ofstream outFile(FileName2, ios::app);
		if (!outFile.is_open()) {
			cout << "Unable to sign up!" << endl;
			return false;
		}
		PNode* temp = cart.head;
		outFile << cust->customerName << cust->timeresume << "\t";
		while (temp != NULL) {
			if (temp->next == NULL) {
				outFile << temp->ProductID << "\t" << temp->ProductName << "\t" << temp->ProductPrice << "\t" << temp->ProductQuantity << "\t" << "0o" << "\t";
			}
			else {
				outFile << temp->ProductID << "\t" << temp->ProductName << "\t" << temp->ProductPrice << "\t" << temp->ProductQuantity << "\t" << "0" << "\t";
			}

			temp = temp->next;
		}
		outFile << endl;
		outFile.close();
		return true;
	}
	bool restorecart(CNode*& node, AddToCart& cart) {
		ifstream inFile(FileName2);
		if (!inFile.is_open()) {
			cout << "Error!" << endl;
			return false;
		}
		string ch;
		string line;
		char time = (char)node->timeresume + '0';
		string userName = node->customerName + time;
		string uName;
		//double total;
		PNode* newobj = new PNode;
		while (!inFile.eof()) {
			inFile >> uName;
			if (uName == userName) {
				while (!inFile.eof()) {
					inFile >> newobj->ProductID >> newobj->ProductName >> newobj->ProductPrice >> newobj->ProductQuantity >> ch;
					cart.insert_at_end(newobj);
					if (ch == "0o") {
						break;
					}
				}
			}
		}
		inFile.close();
		return false;
	}
};
int main()
{
	gotoxy(40, 5);
	cout << "--------------------------------------------------------------";
	cout << endl;
	cout << "                                         ---------- W E L C O M E     T O ----------";
	cout << endl;
	displayBanner();
	cout << "                                         -------- S H O P P I N G     S T O R E --------";
	cout << endl;
	cout << "                                         -------------------------------------------------------------";
	cout << endl;
	system("pause");
	system("cls");
	Customers customer;
	Items item;
	int id = 0;
	Admin admin;
	while (1) {
		customer.restoringcust();
		CNode* temp = NULL;
		AddToCart cart;
		char ent;
		cout << "Press s to Start Shopping " << endl;
		cin >> ent;
		if (ent == 's' || ent == 'S') {
			id++;
		}
		else {
			break;
		}
		int categ;
		string pass;
		string cn; string em; string add;
		int sign;
		cout << "Press 1 to sign in & 2 to sign up" << endl; cin >> sign;
		if (sign == 1) {
			while (true) {
				cout << "Enter Username : "; cin >> cn;
				cout << "Enter Password : "; cin >> pass;
				if (!admin.checkCredentials(cn, pass)) {
					cout << "Incorrect Credentials!" << endl;
				recheck:
					cout << "1: sign up\t2: sign in again\t0: exit" << endl;
					int choice;
					cin >> choice;
					if (choice == 1) {
						cout << "Enter Username : "; cin >> cn;
						cout << "Enter Password : "; cin >> pass;
						cout << "Enter Email : "; cin >> em;
						cout << "Enter Address : "; cin >> add;
						admin.SignUp(cn, pass, em, add);
					}
					else if (choice == 2) {
						continue;
					}
					else {
						cout << "do you want to exit?\t1: yes    2:no" << endl;
						int c;
						cin >> c;
						if (c == 2) {
							goto recheck;
						}
						system("cls");
						system("color fc");
						cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
						cout << "------------------------------------------------------ THANK YOU! ------------------------------------------------------" << endl;
						cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
						return 0;
					}
				}
				temp = customer.findout(cn);
				if (temp != NULL) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
					cout << "Signed in Successfully" << endl;
					system("pause");
					system("cls");
				}
				char n;
				cout << "Press 1 to view cart\nPress s to Continue Shopping" << endl;
				cin >> n;
				admin.restorecart(temp, cart);
				if (n == '1') {
					system("pause");
					system("cls");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
					cout << "                           --------------------------------------------------------------";
					cout << endl;
					cout << "                           ------------- B I L L I N G -----------";
					cout << endl;
					cart.printbill();
					temp->timeresume++;
				}
				else if (n == 's') {

				}

				break;
			}
		}
		else if (sign == 2) {
			cout << "Enter your User Name : "; cin >> cn;
			cout << "Enter your Password : "; cin >> pass;
			cout << "Enter Email : "; cin >> em;
			cout << "Enter Address : "; cin >> add;
			if (admin.SignUp(cn, pass, add, em)) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "Successfully registered! " << endl;
				system("pause");
				system("cls");
			}
			else {
				return -1;
			}
		}
		else {
			cout << "Invalid attempt!" << endl;
			return -1;
		}
		if (temp == NULL) {
			CNode* cu = new CNode(id, cn, em, add, 0);
			temp = cu;
			customer.insertlist(id, cn, em, add, 0);
		}
		ProductStack obj;
		//1: Shoes\t2: Cosmetics\t3: Perfumes\t4: Stationary\t5: Clothes"
		//addItem(item);
		while (1) {
			item.DisplayCategories();
			cin >> categ;
			cout << "                           --------------------SELECT A PRODUCT BY ENTERING ID--------------------" << endl;
			if (categ == 1) {
				obj.head = NULL;
				addItem(item);
				item.Shoes->DisplayItemsFromStack(obj);
			}
			if (categ == 2) {
				obj.head = NULL;
				addItem(item);
				item.Cosmetics->DisplayItemsFromStack(obj);
			}
			if (categ == 3) {
				obj.head = NULL;
				addItem(item);
				item.Perfumes->DisplayItemsFromStack(obj);
			}
			if (categ == 4) {
				obj.head = NULL;
				addItem(item);
				item.Stationary->DisplayItemsFromStack(obj);
			}
			if (categ == 5) {
				obj.head = NULL;
				addItem(item);
				item.Clothes->DisplayItemsFromStack(obj);
			}
			char n;
			obj.PopToUser(cart);
			cout << "Press n For Categories\nPress o to order \nPress 0 For Sign out" << endl;
			cin >> n;
			if (n == '0') {
				customer.storingcust(temp);
				admin.storingammount(temp, cart);
				customer.head = NULL;
				break;
			}
			else if (n == 'o') {
				customer.storingcust(temp);
				admin.storingammount(temp, cart);
				customer.head = NULL;
				ordernow(item.Shoes, item.Cosmetics, item.Clothes, item.Stationary, item.Perfumes, cart);
				continue;
			}
			if (n == 'n' || n == 'N') {
				continue;
			}
			else {
				break;
			}
		}
	}
}