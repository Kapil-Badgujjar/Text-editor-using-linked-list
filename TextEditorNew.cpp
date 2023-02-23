#include <bits/stdc++.h>
#include <conio.h>
#include <unistd.h>
#include <fstream>

using namespace std;

class mylist
{
public:
    string word;
    mylist *next;
    mylist(string w)
    {
        word = w;
        next = NULL;
    }
};

class undoObjectsStorage
{
    mylist *prev;
    mylist *current;
    int x;

public:
    void setValue(mylist *prevNode, mylist *currentlink, int operation)
    {
        prev = prevNode;
        if(currentlink!=NULL){
            current = new mylist(currentlink->word);
            current->next = currentlink->next;
        }
        else{
            current=NULL;
        }
        x = operation;
    }
    mylist *getLink() { return current; }
    mylist *getPrev() { return prev; }
    int getOperation() { return x; }

    undoObjectsStorage operator=(undoObjectsStorage s)
    {
        undoObjectsStorage temp;
        temp.current = s.current;
        temp.x = s.x;
        return temp;
    }
};

class undoHelperClass
{
    undoObjectsStorage *stack[5];
    int front = -1, back = -1;

public:
    bool isEmpty() { return !(front+1); }
    void push(mylist *prev, mylist *link, int operation)
    {
        if (front == -1)
        {
            front = back = 0;
            stack[back] = new undoObjectsStorage;
            stack[back]->setValue(prev, link, operation);
        }
        else if ((front + 4) % 5 == back)
        {
            front = (front + 1) % 5;
            back = (back + 1) % 5;
            stack[back] = new undoObjectsStorage;
            stack[back]->setValue(prev, link, operation);
        }
        else
        {
            back = (back + 1) % 5;
            stack[back] = new undoObjectsStorage;
            stack[back]->setValue(prev, link, operation);
        }
    }
    undoObjectsStorage *pop()
    {
        undoObjectsStorage *temp;
        if (back != -1)
        {
            if (front == back)
            {
                temp = stack[back];
                front = back = -1;                
            }
            else
            {
                temp = stack[back];
                back = (back + 4) % 5;
            }
        }
        else
            temp == NULL;
        return temp;
    }
};

class helperClass
{
public:
    void freeSpace(mylist *head)
    {
        if (head->next == NULL)
        {
            delete (head);
        }
        else
        {
            freeSpace(head->next);
        }
    }

    mylist *find(mylist *head, string pattern)
    {
        if (head == NULL)
            return NULL;
        mylist *temp = head;
        while (temp != NULL)
        {
            if (temp->word == pattern)
                return temp;
            temp = temp->next;
        }
        return NULL;
    }

    void insert(mylist *head, undoHelperClass *UHC)
    {
        if (head != NULL)
        {
            UHC->push(NULL, head, 1);
            head->next=NULL;
        }
        mylist *t = head;
        string temp = "";
        char c;
        bool flag = false;
        bool init = true;
        do
        {
            c = getchar();
            if (c == '\n' && init)
            {
                init = false;
                continue;
            }
            if (c != ' ' && c != '`' && c != '\n')
            {
                temp += c;
                flag = true;
            }
            else
            {
                if (c == ' ' && flag == true)
                {
                    t->word = temp;
                    t->next = new mylist("");
                    t = t->next;
                    temp = "";
                    flag = false;
                }
                if (c == '\n')
                {
                    t->word = temp;
                    t->next = new mylist("\n");
                    t = t->next;
                    t->next = new mylist("");
                    t = t->next;
                    temp = "";
                }
                if (c == '`')
                {
                    t->word = temp;
                }
            }
        } while (c != '`');
    }

    void update(mylist *head, undoHelperClass *UHC)
    {
        if (head == NULL)
        {
            cout << "Head is Empty!" << endl;
            return;
        }
        string oldWord, newWord;
        cout << "Enter the old word that you want to update : ";
        cin >> oldWord;
        mylist *check = find(head, oldWord);
        if (check == NULL)
        {
            cout << "Old word not found!" << endl;
            return;
        }
        mylist *prev = head;
        while (prev->next != check)
        {
            prev = prev->next;
        }
        UHC->push(prev, check, 2);
        cout << "Enter new word : ";
        cin >> newWord;
        check->word = newWord;
        cout << "Updated successfully..." << endl;
    }

    mylist *findEnd(mylist *head)
    {
        if (head == NULL)
        {
            cout << "Head is Empty!" << endl;
            return NULL;
        }
        mylist *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        return temp;
    }

    mylist *deleteFunction(mylist *head, undoHelperClass *UHC)
    {
        if (head == NULL)
        {
            cout << "Head is empty!" << endl;
            return head;
        }
        string deleteWord;
        cout << "Enter the word that you want to delete : ";
        cin >> deleteWord;
        mylist *check = find(head, deleteWord);
        if (check == NULL)
        {
            cout << "Word not found! \nCan't delete..." << endl;
            return head;
        }
        else
        {
            if (check != head)
            {
                mylist *prev = head;
                check = head;
                while (check != NULL)
                {
                    if (check->word == deleteWord)
                        break;
                    prev = check;
                    check = check->next;
                }
                UHC->push(prev, check, 4);
                prev->next = check->next;
            }
            else
            {
                UHC->push(NULL,head,4);
                check = head;
                head = head->next;
            }
            delete (check);
        }
        return head;
    }

    void printList(mylist *head)
    {
        mylist *temp = head;
        if (temp == NULL)
            return;
        while (temp->next != NULL)
        {
            if (temp->word == "\n")
                cout << temp->word;
            else
                cout << temp->word << " ";
            temp = temp->next;
        }
        cout << temp->word << endl;
        return;
    }

    mylist *search(mylist *head)
    {
        if (head->next == NULL)
        {
            cout << "Your Head is NULL!" << endl;
            return NULL;
        }
        string pattern;
        cout << "Enter the word that you want to search : ";
        cin >> pattern;
        return find(head, pattern);
    }

    void saveFunction(mylist *head)
    {
        mylist *t = head;
        string filename;
        cout << "Enter the file name with extension :";
        cin >> filename;
        ofstream save(filename);
        string temp;
        while (t != NULL)
        {
            temp += t->word + " ";
            t = t->next;
        }
        save << temp;
    }

    mylist *undoFunction(mylist *head, undoObjectsStorage *last,undoHelperClass *r)
    {
        mylist *temp = head;
        int operation = last->getOperation();
        mylist *prev = last->getPrev();
        mylist *current = last->getLink();
        switch (operation)
        {
        case 1:
            r->push(NULL,head,1);
            head = current; 
            break;
        case 2:
            r->push(prev,temp->next,2);
            prev->next->word = current->word;
            break;
        case 3: 
            r->push(prev,prev->next,3);
            prev->next = NULL;
            break;
        case 4: 
            if(prev==NULL){
                r->push(NULL,head,4);
                current->next=head;
                head=current;
            }
            else{
                r->push(prev,current,4);
                current->next = temp->next;
                prev->next = current;
            }
            break;
        }
        return head;
    }

    mylist *redoFunction(mylist *head, undoObjectsStorage *r, undoHelperClass *u){
         mylist *temp = head;
        int operation = r->getOperation();
        mylist *prev = r->getPrev();
        mylist *current = r->getLink();
        switch (operation){
            case 1: 
            u->push(NULL,head,1);
            head=current;
            break;
            case 2:
            u->push(prev,prev->next,2);
            prev->next->word = current->word;
            break;
            case 3:
            u->push(prev,prev->next,3);
            prev->next = current;
            //u->push(prev,NULL,3);
            break;
            case 4:
            if(prev==NULL){
                u->push(NULL,head,4);
                *head=*head->next;
            }
            else{
                u->push(prev,prev->next,4);
                prev->next=prev->next->next;
            }
            break;
        }
        return head;
    }

    void append(mylist *head){
         mylist *t = head;
        string temp = "";
        char c;
        bool flag = false;
        bool init = true;
        do
        {
            c = getchar();
            if (c == '\n' && init)
            {
                init = false;
                continue;
            }
            if (c != ' ' && c != '`' && c != '\n')
            {
                temp += c;
                flag = true;
            }
            else
            {
                if (c == ' ' && flag == true)
                {
                    t->word = temp;
                    t->next = new mylist("");
                    t = t->next;
                    temp = "";
                    flag = false;
                }
                if (c == '\n')
                {
                    t->word = temp;
                    t->next = new mylist("\n");
                    t = t->next;
                    t->next = new mylist("");
                    t = t->next;
                    temp = "";
                }
                if (c == '`')
                {
                    t->word = temp;
                }
            }
        } while (c != '`');
    }
};

// Program starts here
int main()
{
    bool flag = false;                          // to check if we want to create new string or not : if string is not created then we can't perform any operation
    mylist *head, *x, *endNode;                 // Head pointer
    helperClass h;                              // object of helper class
    undoHelperClass *UHC = new undoHelperClass; // object of undo helper class
    undoHelperClass *redoObject=new undoHelperClass; // object of undo for redo
    char choice;                                // variable to get choice
    system("CLS");
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    cout << "|                                                          Text Editor                                                    |" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
    while (!flag)
    {
        cout << "Press [ n ] to create new string, [ x ] to exit : ";
        choice = getch();
        system("CLS");
        if (choice == 'x')
            break;
        else if (choice == 'n')
        {
            head = new mylist("");
            cout << "New node is Created!\nPress any key to continue." << endl;
            getch();
            flag = true;
            system("CLS");
        }
        else
        {
            cout << "Please choose correct option!" << endl;
        }
    }
    while (flag)
    {
        cout << endl
             << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << "|       Options : Press->      [ i ] To insert.  [ u ] To update. [ a ] To append. [ s ] To save.    [ d ] To delete.     |" << endl;
        cout << "|                              [ p ] To display. [ f ] To search. [ z ] To undo.   [ y ] To redo.    [ x ] To exit.       |" << endl;
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
        cout << endl
             << "Please choose an option  :   ";
        choice = getch();
        system("CLS");
        switch (choice)
        {
        case 'i':  cout<<">>> Insert Mode <<<"<<endl<<endl;    // in insert we pass two arguments to store the data , one argument is head, and another one is object of undohelper class
            h.insert(head, UHC); // calling insert function
            system("CLS");
            cout << "Data inserted successfully...\nPress any key to continue... " << endl; getch();  system("CLS");
            break;
        case 'u':   cout<<">>> Update Mode <<<"<<endl<<endl;
            h.update(head, UHC);
            getch(); system("CLS");
            break; // update
        case 'a': cout<<">>> Append Mode <<<"<<endl<<endl;
            endNode = h.findEnd(head);
            UHC->push(endNode, endNode, 3);
            h.append(endNode->next = new mylist(""));
            cout<<">>> Data appended successfully."<<endl; getch(); system("CLS");
            break; // append
        case 'z':   
            if(!UHC->isEmpty()){
                head = h.undoFunction(head, UHC->pop(),redoObject);
                cout<<"Undo successfully."<<endl; getch(); system("CLS");
            }
            else{
                cout<<"Can't undo!"<<endl;
                getch(); system("CLS");
            }
            break;
        case 'y':
            if(!redoObject->isEmpty()){
                head = h.redoFunction(head, redoObject->pop(),UHC);
                cout<<"Redo successfully."<<endl; getch(); system("CLS");
            }
            else{
                cout<<"Can't redo!"<<endl;
                getch(); system("CLS");
            }
            break; // redo
        case 'd':   cout<<">>> Delete Mode <<<"<<endl; 
            head = h.deleteFunction(head, UHC);
            cout<<"Deleted successfully."<<endl; getch(); system("CLS");
            break;
        case 'p': cout<<">>> Output Display Mode <<<"<<endl<<endl;
            h.printList(head);
            getch(); system("CLS");
            break; // print
        case 'f':  cout<<">>> Search Mode <<<"<<endl;
            x = h.search(head);
            if (x != NULL)
                cout << "Found" << endl;
            else
                cout << "Not found" << endl;
            getch(); system("CLS");
            break;
        case 's':   cout<<">>> File Save Mode <<<"<<endl;
            h.saveFunction(head);
            getch(); system("CLS");
            break; // save
        case 'x': cout<<"Editor Closed Successfully"<<endl;
            exit(0); // exit the program
        }
    }
    cout << "Text editor exited successfully.";
    return 0;
}