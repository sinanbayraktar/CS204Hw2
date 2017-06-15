#include <iostream>
#include <string>
#include<sstream>
#include<fstream>
using namespace std;         // this program is a tabling program for f1 players with names and points

struct node
{
	string name;
	int point;
	node * next;

	//default constructor
	node::node ()
		:name(""), point(0), next(NULL)
	{}

	//constructor
	node::node (string a, int i,node *n)
		:name(a), point(i), next(n)
	{}	
};

int list_length (node * head_ptr)  // lenght of the list
{
	node *cursor;
	int answer=0;
	for(cursor=head_ptr; cursor != NULL; cursor=cursor->next)
		answer++;
	return answer;
}


bool SearchList(node *head, string name1)  // checking is there any node with this name 
{
	node *temp = head;
	while (temp != NULL)
	{
		if(temp->name == name1) //If the node with the given name is found
			return true;			
		temp = temp->next;
	}

	return false;
}

node * FindNameNode(node *head, string name2)  // finding the looking node
{
	// int counter=0;
	node *ptr = head;
	while (ptr != NULL && ptr->name != name2)
	{ 
		ptr = ptr->next;
		// counter++;
	}
	return ptr;
}

node * FindNameNodePrevious(node *head, string name2)   // finding the node which is before the looking node
{
	int counter=0;
	node *ptr = head;
	while (ptr != NULL && ptr->name != name2)
	{ 
		ptr = ptr->next;
		counter++;
	}

	node *temp = head;
	for (int i=0; i < counter-1; i++)
	{ 
		temp = temp->next;
	}

	return temp;
}

void ClearList(node *head)   // clearing the list at the end
{
	node *ptr;
	while(head!=NULL)
	{
		ptr=head;
		head=head->next;
		delete ptr;
	}
}

void SortList (node *head)       //   sorting linked list respect to name
{
	node * temphead = head;
	int temproll;
	string tempname;
	int counter = 0;
	while (temphead)
	{
		temphead = temphead->next;
		counter++;
	}
	temphead = head;

	for (int j=0; j<counter; j++)
	{
		while (temphead->next)  //iterate through list until next is null
		{
			if (temphead->name > temphead->next->name)
			{
				temproll = temphead->point;
				temphead->point = temphead->next->point;
				temphead->next->point = temproll;

				tempname = temphead->name;
				temphead->name = temphead->next->name;
				temphead->next->name = tempname;
				temphead = temphead->next;  //increment node
			}
			else 
				temphead = temphead->next;//increment node
		}	
		temphead = head;  //reset temphead
	}

}

void InputOrganization (int a, node *head, string nameFile, int pointFile)    // if appropiate, put the node and sort the list
{	
	node *temp = head;

	if (pointFile <= 0 && !SearchList(temp, nameFile))   // daha önce yoksa ve puanı negatifse ekleme
	{
		cout<<nameFile<<" has not been added \nsince the initial points cannot be non-positive."<<endl;
	}		

	else
	{
		if (!SearchList(temp, nameFile))   // if there is no same name
		{
			while(temp->next != nullptr)
			{
				temp = temp->next;
			}

			temp->next=new node(nameFile, pointFile, NULL);    // new node
			temp=temp->next;

			SortList (head);

			cout<<nameFile<<" has been added to the list with initial points "<<pointFile<<endl;
		}

		else if (SearchList(temp, nameFile))   // if there is same name
		{
			FindNameNode(temp, nameFile)->point = FindNameNode(temp, nameFile)->point + pointFile;  // finding that node and updating it's point

			if (FindNameNode(temp, nameFile)->point <= 0)  // if new point is not positive, deletion
			{
				FindNameNodePrevious(temp, nameFile)->next = FindNameNode(temp, nameFile)->next;
				delete FindNameNode(temp, nameFile);

				cout<<nameFile<<" has been removed from the list since his points became non-positive."<<endl;
			} 

			else
			{
				cout<<nameFile<<" has been updated and new point is "<<FindNameNode(temp, nameFile)->point<<endl;
			}

		}
	}
}


int main()
{
	node *myList;			//my linklist
	myList = new node;

	while (true)
	{
		int a;   // option number
		cout<<"Formula 1 Points Table System\n-----------------------------------"<<endl;
		cout<<"Please select one option [1..4]:\n1. Load driver names and points from a file\n2. Insert a new driver / Modify points of an existing driver\n3. Display points table in alphabetical order\n4. Exit"<<endl;
		cin>>a;		

		if (a == 1)   // Load driver names and points from a file option
		{
			string fileName;
			cout<<"Enter the name of the file: ";
			cin>>fileName;

			ifstream input;
			input.open(fileName);

			while(input.fail())  // error message for wrong file names
			{
				cout<< "Error:Could not open the file "<< fileName<< endl;

				cout<<"Enter the input file name"<< endl;
				cin>>fileName;

				ifstream input;
				input.open(fileName);
			}

			string line;
			while(!input.eof())
			{
				getline(input, line);
				stringstream ss(line);

				string nameFile;
				int pointFile;

				while (ss >> nameFile >> pointFile)
				{
					InputOrganization (a, myList, nameFile, pointFile);
				}
			}
		}

		else if (a == 2)   // Insert a new driver / Modify points of an existing driver option
		{
			string newname;
			int newpoint;

			cout<<"Please enter name of the driver you wish to add/modify: "<<endl;
			cin>>newname;
			cout<<"Please enter how many points you wish to initialize/add/remove: "<<endl;
			cin>>newpoint;

			InputOrganization (a, myList, newname, newpoint);

		}

		else if (a == 3)    // Display points table in alphabetical order option
		{
			node *ptr = myList;

			ptr = ptr->next;

			if (ptr == NULL)
				cout<<"The points table is empty"<<endl;

			else
			{
				for (int i=0; i < list_length(myList)-1 ; i++)
				{
					cout<<ptr->name<<" "<<ptr->point<<endl;
					ptr = ptr->next;
				}
			}
		}

		else if (a == 4)   // Exit option
		{
			ClearList (myList);

			cin.ignore();
			cin.get();
			return 0; 
		}
	}
}
