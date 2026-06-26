#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<fstream>
using namespace std;
string gettimestamp(){
    time_t now = time(0);
    string timestamp = ctime(&now);
    return timestamp;
}
class Bankaccount{
    private:
    string name;
    int acct_num;
    int balance;
    int pin;
    int login_pin;
    vector<string>history;

    public:
    Bankaccount(string n, int a, int b, int p, int l){
        name = n;
        acct_num = a;
        balance = b;
        pin = p;
        login_pin = l;
    }
    bool pinauth(int pinn){
        return pin == pinn;
    }
    bool logauth(int log){
        return login_pin == log;
    }
    void deposit(int amount, bool log = true, bool print = true){
        if (amount > 0 ){
            balance += amount;
            if(log){
                addhistory(gettimestamp() + "Deposit: " + to_string(amount));
            }
            if(print){
                cout<<"deposit successful"<<endl;
            }
        }else{
             cout<<"invalid deposit"<<endl;
            }
        }
     bool withdraw(int amount, bool log = true, bool print = true){
                if (amount > 0 && amount <= balance){
                balance -= amount;
                
                if(log){
                    addhistory(gettimestamp() + "withdraw: " + to_string(amount));
                }
                if(print){
                    cout<<"successful withdrawal"<<endl;
                }
                return true;
            }else{
            cout<<"invalid withdrawal"<<endl;
            return false;
            }
        }
int getbalance(){
    return balance;
}
int getacctnum(){
    return acct_num;
}
string getname(){
    return name;
}
void display(){
    cout<<name<<endl<<balance<<endl<<acct_num<<endl;
}
 void showhistory(){
        for(int i = 0; i < history.size(); i++){
            cout<<history[i]<<endl;
        }
    }
    void addhistory(string entry){
        history.push_back(entry);
    }
};
vector<Bankaccount>Bankaccounts;
Bankaccount* current_user = nullptr;
void createaccount(){
      string name;
    int balance;
    int acct_num;
    int pin;
    int log;
    cout<<"whats your name ";
    cin>> ws;
    getline(cin, name);
    cout<<"whats your account number";
    cin >> acct_num;
    cout<<"whats your initial balance";
    cin>> balance;
    cout<<"create your pin";
    cin>> pin;
    cout<<"create your login pin";
    cin>>log;

    Bankaccount newacc (name, acct_num,balance, pin, log);
    Bankaccounts.push_back(newacc);
}

Bankaccount* findaccount(int acct_num){
    for(int i = 0; i < Bankaccounts.size(); i++){
        if (acct_num == Bankaccounts[i].getacctnum() )
        {
            return &Bankaccounts[i];
            
        }
        
    }
        return nullptr;

    }
void login(){
    int acct;
    cout<<"enter your account number "<<endl;
    cin>>acct;
    Bankaccount* log = findaccount(acct);
    if(log == nullptr){
        cout<<"account number is incorrect";
        current_user = nullptr;
        return;
    }
    int pin;
    cout<<"enter your login pin"<<endl;
    cin>>pin;
    if(! log->logauth(pin)){
        cout<<"incorrect pin"<<endl;
        current_user = nullptr;
        return;
    }
    cout<<"LOGIN SUCCESSFUL "<<endl;
    current_user = log;
}
void logout(){
    current_user = nullptr;
}
void deposits(){
    int amount;
    cout<<"enter the amount"<<endl;
    cin>>amount;
    if(current_user != nullptr){
        current_user->deposit(amount);
    }else{
        cout<<"user not logged in";
    }
}
void withdrawal(){
    int amount;
    int pin;
    if(current_user != nullptr){
         cout<<"enter the amount"<<endl;
            cin>>amount;
        cout<<"enter your pin";
        cin>>pin;
        if(current_user->pinauth(pin)){
           current_user->withdraw(amount);
        }else{
        cout<<"incorrect pin";
        return;
        }
    }else{
        cout<<"user not logged in";
        return;
    }
}
void transfer(){
    int amount;
    int pin;
    int acct;
    Bankaccount* receiver;
    if(current_user != nullptr){
        cout<<"enter the account number of the recepient"<<endl;
         cin>>acct;
         cout<<"enter the amount"<<endl;
         cin>>amount;
         receiver = findaccount(acct);
        if(receiver == nullptr){
             cout<<"incorrect recepient account number "<<endl;
            return;
         }
        cout<<"enter your pin";
        cin>>pin;
        if(current_user->pinauth(pin)){
             if(current_user->withdraw(amount, false, false)){
                receiver-> deposit(amount, false, false);
               current_user->addhistory(gettimestamp() + "transfer: " + to_string(amount) + " to " + receiver->getname());
               receiver->addhistory(gettimestamp() + "transfer receiced from " + current_user->getname() + " amount: " + to_string(amount));
               cout<<"TRANSACTION SUCCESSFUL"<<endl;
                return ;
             }else{
                return;
             }
        }else{
        cout<<"incorrect pin";
        return;
        }
    }else{
        cout<<"user not logged in";
        return;
    }
}
void deleteaccount(){
            if(current_user != nullptr){
                int pin;
                cout<<"enter your pin"<<endl;
                cin>>pin;
                if(current_user->pinauth(pin)){
                     for(int i = 0; i < Bankaccounts.size(); i++){
                        if(current_user == &Bankaccounts[i]){
                            Bankaccounts.erase(Bankaccounts.begin() + i);
                             cout<<"account successfully deleted"<<endl;
                            logout();
                            return;

                        }
                    }
                    cout<<"account not found"<<endl;
                    return;
                        
                }else{
                    cout<<"incorrect pin";
                    return;
                }
          
        }else{
            cout<<"user not logged in"<<endl;
        }
    }
    void displayall(){
        if(current_user != nullptr){
                current_user->display();
        }else{
            cout<<"user not logged in "<<endl;
            return;
        }
    }
        void transactionhistory(){
            if(current_user != nullptr){
                current_user->showhistory();
            }else{
                 cout<<"user not logged in "<<endl;
            return;
            }
        }
    void saveaccounts(){
        ofstream file("accounts.txt");
        for(int i = 0; i < Bankaccounts.size(); i++){
            file<<Bankaccounts[i].getname()<<" "<<Bankaccounts[i].getbalance()<<" "<<Bankaccounts[i].getacctnum()<<endl;
        }
        file.close();
    }


    
int main(){
    cout<<"WELCOME TO UBA MOBILE BANK APP"<<endl;
    int choice;
   
    do{
         cout<<"1. create account "<<endl<<"2. login "<<endl<<"3. deposit "<<endl<<"4. withdraw "<<endl<<"5. transfer "<<endl<<"6. check account balance "<<endl<<"7. delete account "<<endl<<"8. log out "<<endl<<"9. transaction history"<<endl<<"10. exit"<<endl;
        cin>> choice;
        switch(choice){
        case 1:
        createaccount();
        saveaccounts();
        break;
        case 2:
        login();
        break;
        case 3:
        deposits();
        saveaccounts();
        break;
        case 4:
        withdrawal();
        saveaccounts();
        break;
        case 5:
        transfer();
        saveaccounts();
        break;
        case 6:
        displayall();
        break;
        case 7:
        deleteaccount();
        saveaccounts();
        break;
        case 8:
        logout();
        cout<<"login to continue"<<endl;
        break;
        case 9:
        transactionhistory();
        break;
        case 10:
        break;
        default :
        cout<<"invalid option"<<endl;

    }

}while(choice != 10);
cout<<"THANK YOU FOR CHOOSING UBA"<<endl;
return 0;

}