#include<iostream>
#include<string>
#include<fstream>
using namespace std;

// Helper Class
class Helper                  //to avoid shallow copy of character arrays
{
public:
    static int strlength(char* str)
    {


        int count = 0;
        int i = 0;
        while (str[i] != '\0')
        {
            count++;
            i++;
        }
        return count;
    }
    static int strlength(const char* str)
    {
        int count = 0;
        int i = 0;
        while (str[i] != '\0')
        {
            count++;
            i++;
        }
        return count;
    }
    static void stringcopy(char*& destination, char*& source)
    {
        int result;
        int i = 0;
        result = strlength(source);

        while (i < result)
        {
            destination[i] = source[i];

            i++;
        }
        destination[i] = '\0';
    }
    static char* bufferingString(char* str)
    {
        int result;
        result = strlength(str);
        char* final = new char[result + 1];
        stringcopy(final, str);
        return final;
    }
    static char* concatenateStr(char* string1, const char* string2)
    {
        int len1 = strlength(string1);
        int len2 = strlength(string2);
        char* temp = new char[len1 + len2 + 1];
        int i = 0;
        while (i < len1)
        {
            temp[i] = string1[i];
            i++;
        }
        for (int j = 0; j < len2; j++, i++)
            temp[i] = string2[j];
        temp[i] = '\0';
        return temp;
    }
};
class User;
class SocialNetworkApplication;
class Page;
class date;
class post;
class activity;
class comment;
class object;

class object        //abstract class (parent class of user and page)
{
    char* ID;
public:
    virtual void post_on_timeline(post*& ptr) = 0;     //abstract function (polymorphism)
    virtual void print()
    {
        cout << ID << endl;
    }
    virtual void displayName() = 0;                    //abstract function (polymorphism)
    virtual ~object()
    {
        delete[] ID;
    }
};

class comment
{
    char* ID;
    char* text;
    object* commentby;                          //aggregation
    
public:
    static int total;
    comment() 
    {
        ID = new char[10];
        text = nullptr;
        commentby = nullptr;
    }
    comment(const comment& obj)                 //copy constructor
    {
        ID = Helper::bufferingString(obj.ID);
        text = Helper::bufferingString(obj.text);
    }
    comment& operator=(const comment& obj)
    {
        ID = Helper::bufferingString(obj.ID);
        text = Helper::bufferingString(obj.text);
        commentby = obj.commentby;
        return *this;
    }
    void setcommentby(object* user) 
    {
        commentby = user;
    }
    void setText(ifstream& fin)
    {
        char buffer[100] = "";
        fin.getline(buffer, 100);
        text = Helper::bufferingString(buffer);
    }
    void setID(ifstream& fin) 
    {
        char temp[20];
        fin >> temp;
        ID = Helper::bufferingString(temp);
    }
    comment(char* Text, object* user) 
    {
        char id[] = "c";
        string temp = std::to_string(gettotalcomment());
        char const* number = temp.c_str();
        ID = Helper::concatenateStr(id, number);
        text = Helper::bufferingString(Text);
        commentby = user;
    }
    object* getcommentby() 
    {
        return commentby;
    }
    void print()
    {
        cout << "\t";
        if (this->commentby == nullptr)
            cout << "Anonymous";
        else
            commentby->displayName();
        cout << " wrote :";
        cout << " " << text << "." << endl;
    }
    static void totalcomment(int count)
    {
        total = count;
    }
    int gettotalcomment()
    {
        return total;
    }

    ~comment() 
    {
        if (this == nullptr)
            return;
        if (ID != nullptr) 
        {
            delete[] ID;
        }
        if (text != nullptr) {
            delete[] text;
        }
    }
};
int comment::total = 0;

// class activity
class Activity {
    int type;
    char* value;
public:
    void readdatafromfile(ifstream& fin) 
    {
        char buffer[100];
        fin >> type;
        fin.getline(buffer, 100);
        value = Helper::bufferingString(buffer);
    }
    void print() 
    {
        if (type == 1)
            cout << "  is feeling ";
        else if (type == 2)
            cout << " is thinking about ";
        else if (type == 3)
            cout << " is making ";
        else
            cout << " is celebrating ";

        cout << value << endl;
    }
    ~Activity() 
    {
        if (value != nullptr) 
            delete[] value;
    }
};

//class date
class date {
    int day;
    int month;
    int year;
    static date currentdate;
public:
    date();
    date(int, int, int);
    ~date();
    int getyear();
    int getmonth();
    int getday();
    void readdatafromfile(ifstream&);
    void print();
    static void setcurrentdate(int, int, int);
    bool Isequal(date);
    static date getcurrentdate();
    date previousdate();
    //void setsharedby(object*);
};
int date::getday()
{
    return day;
}
int date::getmonth()
{
    return month;
}
int date::getyear()
{
    return year;
}
date date::previousdate() 
{
    date temp;
    temp.day = currentdate.day - 1;
    temp.month = currentdate.month;
    temp.year = currentdate.year;
    return temp;
}
date::date(int d, int m, int y) 
{
    day = y;
    month = m;
    year = y;
}
date date::getcurrentdate() 
{
    return currentdate;
}
bool date::Isequal(date rhs) 
{
    return(currentdate.day == rhs.day || currentdate.day == rhs.day + 1 && currentdate.month == rhs.month && currentdate.year == rhs.year);
}
void date::setcurrentdate(int D, int M, int Y)
{
    currentdate.day = D;
    currentdate.month = M;
    currentdate.year = Y;
}
date date::currentdate;
date::date() 
{
    day = 0;
    month = 0;
    year = 0;
    *this = date::currentdate;
}
date :: ~date() {}
void date::print() 
{
    cout << "( " << day << "/" << month << "/" << year << " )" << endl;
}
void date::readdatafromfile(ifstream& fin) 
{
    fin >> day;
    fin >> month;
    fin >> year;
}

//class post
class post                                      //parent class of memory (since memory is also a post)
{
    
    int no_of_likes = 0;
    int no_of_comments = 0;
    char* ID;
    char* text;
    int posttype;
    int no_of_post = 0;
    date postdate;                              //composition
    comment* comments = new comment[10];        //aggregation
    object* sharedby;                           //aggregation
    Activity* activity;                         //aggregation
    object** likelist;                          //
public:
    static int totalpost;
    void Addcomment(char* text, object* user) 
    {
        comment* ptr = new comment(text, user);
        this->addcomments(*ptr);
    }
    post();
    post(char* txt, object* currentuser);
    post(const post&);
    post operator=(const post& oj);
    static int getpost();
    static void setpost(int);
    void print();
    void readdatafromfile(ifstream&);
    void setsharedby(object*);
    void likedby(object*);
    void addcomments(comment&);
    char* getID();
    date getdate();
    char* gettext();
    char* getid();
    void viewlikeby();
    object* getsharedby();
    void printActivity();
    void printcomments();
    int getposttype();
    virtual void viewpost();
    ~post();
};
void post::viewpost() {
    if (posttype == 2)
        activity->print();   
    
    cout << text << "    ";
    postdate.print();
    cout << endl;
    this->printcomments();
}
post::post(char* txt, object* currentuser)
{
    char id[] = "post";
    string temp = std::to_string(totalpost);
    char const* Number = temp.c_str();
    ID = Helper::concatenateStr(id, Number);

    text = Helper::bufferingString(txt);
    comments = 0;
    sharedby = currentuser;

    activity = 0;
    likelist = 0;
    postdate = date::getcurrentdate();
    no_of_likes = no_of_comments = 0;
}
int post::getposttype() 
{
    return posttype;
}
void post::printcomments() 
{
    if (comments != nullptr) 
        for (int i = 0; i < no_of_comments; i++) 
            comments[i].print();
}
void post::printActivity()
{
    activity->print();
}
object* post::getsharedby()
{
    if (this != nullptr)
        return sharedby;
}
void post::viewlikeby() 
{
    if (this == nullptr || this->likelist == nullptr)
        return;
    for (int i = 0; likelist[i] != 0; i++)
        likelist[i]->print();
}
char* post::gettext() 
{
    return text;
}
char* post::getid() 
{
    return ID;
}
date post::getdate() 
{
    return postdate;
}
void post::print() 
{
    cout << ID << " " << text << endl;
}
post :: ~post() {
    if (ID) {
        delete[] ID;
    }
    if (text) {
        delete[] text;
    }
    if (activity) {
        delete[] activity;
    }
    if (likelist) {
        for (int i = 0; i < no_of_likes; i++)
            delete[] likelist[i];
        delete[] likelist;
    }
    delete[] comments;
}
char* post::getID() {
    return ID;
}
void post::addcomments(comment& Comment)
{
    if (this != nullptr && no_of_comments < 10)
    {
        comments[no_of_comments] = Comment;
        no_of_comments++;
    }
}
post::post() {
    no_of_likes = 0;
    ID = nullptr;
    text = nullptr;
    no_of_post = 0;
}
void post::setpost(int total)
{
    totalpost = total;
}
int post::getpost() 
{
    return totalpost;
}
void post::likedby(object* ptr) 
{
    if (this == nullptr)
        return;
    if (no_of_likes == 0) {
        likelist = new object * [10];
    }
    if (no_of_likes < 10) {
        likelist[no_of_likes] = ptr;
        likelist[no_of_likes + 1] = nullptr;
        no_of_likes++;
    }
}
int post::totalpost = 0;
void post::setsharedby(object* ptr) 
{
    sharedby = ptr;
}
void post::readdatafromfile(ifstream& fin) 
{
    //int posttype;
    char buffer[100];
    char Id[20];
    fin >> posttype;
    fin >> Id;
    ID = Helper::bufferingString(Id);
    postdate.readdatafromfile(fin);
    fin.ignore();
    fin.getline(buffer, 100);
    text = Helper::bufferingString(buffer);
    if (posttype == 2) 
    {
        activity = new Activity;
        activity->readdatafromfile(fin);
        //activity->print();
    }

}

// class memoryyy
class Memory :public post       //inheritance
{
    post* SharedPost;

public:

    Memory(post* ptr, char* Text, object* CurrentUser) :post(Text, CurrentUser)
    {
        SharedPost = ptr;
    }
    void viewPost()
    {
        cout << "~~~";
        getsharedby()->print();
        cout << " shared a memory~~~";

        cout << "...";
        getdate().print();
        cout << "\n\"" << gettext() << "\"\n";
        cout << "\t\t(" << date::getcurrentdate().getyear() - SharedPost->getdate().getyear() << " Years Ago)\n";

        //SharedPost->viewpost();

        cout << endl << endl;
    }
    ~Memory()
    {
        delete SharedPost;
    }
};

//class userrr
class User : public object          //inheritance
{
    
    char* ID;
    char* first;
    char* last;
    int no_of_pages;
    int no_of_friends;
    int no_of_posts = 0;
    User** Friendlist;          //aggregation
    Page** likedpages;          
    post** timeline;            
    date shareddate;            //composition
public:
    static int totaluser;
    void Readdatafromfile(ifstream&);
    ~User()
    {
        if (first != 0)
            delete[] first;
        if (last != 0)
            delete[] last;
        if (ID != 0)
            delete[] ID;
        if (Friendlist) {
            for (int i = 0; i < no_of_friends; i++) {
                delete[] Friendlist[i];
            }
            delete[] Friendlist;
        }
        if (likedpages) {
            for (int i = 0; i < no_of_pages; i++) {
                delete[] likedpages[i];
            }
            delete[] likedpages;
        }
        if (timeline) {
            for (int i = 0; i < no_of_posts; i++) {
                delete[] timeline[i];
            }
            delete[] timeline;
        }
    }
    User();
    User(const User&);
    User operator=(const User&);
    static void setuser(int user);
    static int getuser();
    char* getfirst()
    {
        return first;
    }
    char* getlast()
    {
        if (last != nullptr)
        return last;
    }
    char* getID()
    {
        return ID;
    }
    void Likedpages(Page*& page)
    {
        if (no_of_pages == 0)
            likedpages = new Page * [10];
        if (no_of_pages < 10)
            if (likedpages != nullptr)
            {
                likedpages[no_of_pages] = page;
                likedpages[no_of_pages + 1] = nullptr;
                no_of_pages++;
            }
    }
    void print()
    {
        cout << ID << " " << first << " " << last << endl;
    }
    void Addfriends(User*& Friend)
    {
        if (no_of_friends == 0)
            Friendlist = new User * [10];
        if (no_of_friends < 10)
            if (Friendlist != nullptr)
            {
                Friendlist[no_of_friends] = Friend;
                Friendlist[no_of_friends + 1] = nullptr;
                no_of_friends++;
            }
    }
    void viewfriendlist()
    {
        for (int i = 0; Friendlist[i] != 0; i++)
            Friendlist[i]->print();
    }
    void viewlikedpaged();
    void displayName()
    {
        cout << first << "  " << last << " ";
    }
    void display_home_page();
    void viewtimeline()
    {
        if (this->timeline == nullptr)
            return;
        for (int i = 0; timeline[i] != 0; i++)
        {
            this->displayName();
            timeline[i]->viewpost();
            cout << endl;
        }
    }
    void PrintLatestPosts()
    {
        date temp = date::getcurrentdate();
        for (int i = 0; i < no_of_posts; i++)
            if (temp.Isequal(timeline[i]->getdate()))
            {
                this->displayName();
                timeline[i]->viewpost();
            }
       /* date temp2(14, 11, 2017);
        temp2.print();
        for (int i = 0; i < no_of_posts; i++)
            if (temp2.Isequal(timeline[i]->getdate()))
            {
                this->displayName();
                timeline[i]->viewpost();
            }*/
    }
    void sharememory(post* ptr, char* text)
    {
        post* tempptr = new Memory(ptr, text, this);
        this->post_on_timeline(tempptr);
    }
    void viewmemories()
    {
        cout << "We hope you enjoy looking back and sharing your memories on SocialNetworkApplication, from the most recent to those long ago.\n\n";
        for (int i = 0; i < no_of_posts; i++)
        {
            post* currentpost = timeline[i];
            date PostDate = currentpost->getdate();
            if (PostDate.getday() == date::getcurrentdate().getday() && PostDate.getmonth() == date::getcurrentdate().getmonth() && PostDate.getyear() != date::getcurrentdate().getyear())
            {
                cout << "On this Day\n";
                cout << date::getcurrentdate().getyear() - PostDate.getyear() << " Year Ago\n";
                displayName();
                timeline[i]->viewpost();
                cout << endl;
            }
        } 
    }
    void post_on_timeline(post*& ptr)
    {
        if (no_of_posts == 0)
            timeline = new post * [12];
        timeline[no_of_posts] = ptr;
        if (no_of_posts < 9)
            timeline[no_of_posts + 1] = nullptr;
        no_of_posts++;
    }
};
int User::totaluser = 0;


// class page
class Page : public object
{
   
    char* ID;
    char* Title;
    post** timeline;
    int no_of_posts;
public:
    static int totalpage;
    void Readdatafromfile(ifstream& fin) {
        char tempbuffer[80];
        fin >> tempbuffer;
        ID = Helper::bufferingString(tempbuffer);
        fin.getline(tempbuffer, 80);
        Title = Helper::bufferingString(tempbuffer);
    }
    Page() {
        ID = 0;
        Title = 0;
    }
    Page(const Page& obj) { // copy constructor
        ID = Helper::bufferingString(obj.ID);
        Title = Helper::bufferingString(obj.Title);
    }
    Page operator=(const Page& obj) { // assignment operator
        ID = Helper::bufferingString(obj.ID);
        Title = Helper::bufferingString(obj.Title);
        return *this;
    }

    ~Page()
    {
        if (ID != nullptr)
            delete[] ID;
        if (Title != nullptr)
            delete[] Title;
        if (timeline)
        {
            for (int i = 0; i < no_of_posts; i++)
                delete[] timeline[i];
            delete[] timeline;
        }
    }
    static int getpage()
    {
        return totalpage;
    }
    static void setpage(int page)
    {
        totalpage = page;
    }
    char* gettitle()
    {
        return Title;
    }
    char* getID()
    {
        return ID;
    }
    void print()
    {
        cout << ID << "  " << Title << endl;
    }
    void post_on_timeline(post*& ptr)
    {
        if (no_of_posts == 0)
            timeline = new post * [12];
        timeline[no_of_posts] = ptr;
        timeline[no_of_posts + 1] = nullptr;
        no_of_posts++;
    }
    void viewtimeline()
    {
        for (int i = 0; i < no_of_posts; i++)
        {
            this->displayName();
            timeline[i]->viewpost();
        }
    }
    void displayName()
    {
        cout << Title;
    }
    void PrintLatestPosts()
    {
        date temp = date::getcurrentdate();
        for (int i = 0; i < no_of_posts; i++)
            if (temp.Isequal(timeline[i]->getdate()))
            {
                this->displayName();
                timeline[i]->viewpost();
            }
        //date temp2(14, 11, 2017);
        //temp2.print();
        //for (int i = 0; i < no_of_posts; i++)
        //    if (temp2.Isequal(timeline[i]->getdate()))
        //    {
        //        this->displayName();
        //        timeline[i]->viewpost();
        //    }
    }
    void viewpage()
    {
        for (int i = 0; i < no_of_posts; i++)
        {
            this->displayName();
            timeline[i]->viewpost();
        }
    }
};
int Page::totalpage = 0;

void User::viewlikedpaged() 
{
    for (int i = 0; likedpages[i] != 0; i++)
        likedpages[i]->print();
        //cout << likedpages[i]->getID << " " << likedpages[i]->gettitle() <<endl;
}
int User::getuser() 
{ // getters/setters for total pages and user
    return totaluser;
}
void User::setuser(int user) 
{
    totaluser = user;
}
User::User() 
{
    first = 0;
    last = 0;
    ID = 0;
    no_of_pages = 0;
    no_of_friends = 0;
}
User::User(const User& obj) 
{
    first = Helper::bufferingString(obj.first);
    last = Helper::bufferingString(obj.last);
    ID = Helper::bufferingString(obj.ID);
}
User User :: operator=(const User& obj) 
{
    ID = Helper::bufferingString(obj.ID);
    first = Helper::bufferingString(obj.first);
    last = Helper::bufferingString(obj.last);
    return *this;
}
void User::display_home_page() {
    date temp = date::getcurrentdate();
    temp.print();

    for (int i = 0; i < no_of_friends; i++)
        Friendlist[i]->PrintLatestPosts();
    
    for (int i = 0; i < no_of_pages; i++)
        likedpages[i]->PrintLatestPosts();
}
void User::Readdatafromfile(ifstream& fin) 
{
    char tempbuffer[80];
    fin >> tempbuffer;
    ID = Helper::bufferingString(tempbuffer);
    fin >> tempbuffer;
    first = Helper::bufferingString(tempbuffer);
    fin >> tempbuffer;
    last = Helper::bufferingString(tempbuffer);
}

class SocialNetworkApplication 
{
    Page** pages;
    User** users;
    post** posts;
    comment** comments;
public:
    void LoadData(const char* pagefile, const char* userfile, const char* postfile, const char* commentfile) 
    {
        ifstream fin;
        this->Loadpages(pagefile);
        this->Loadusers(userfile);
        this->loadposts(postfile);
        this->loadcomments(commentfile);
    }
    void loadcomments(const char* commentfile) 
    {
        int total;
        ifstream fin;
        object* objptr;
        post* postptr;
        fin.open(commentfile);
        fin >> total;
        comment::totalcomment(total);

        comments = new comment * [total];

        for (int i = 0; i < total; i++) 
        {
            char postid[40] = "";
            char userid[40] = "";

            comments[i] = new comment;
            comments[i]->setID(fin);
            fin >> postid;
            fin >> userid;
            comments[i]->setText(fin);
            objptr = search_object_by_ID(userid);
            comments[i]->setcommentby(objptr);
            postptr = search_post_by_id(postid);
            postptr->addcomments(*comments[i]);
            //comments[i]->print();
        }
        fin.close();
    }
    void Loadpages(const char* pagefile) 
    {
        int totalpages = 0;
        ifstream fin;
        fin.open(pagefile);
        if (fin.is_open())
            fin >> totalpages;

        cout << endl << totalpages << endl;
        Page::setpage(totalpages);
        pages = new Page * [totalpages];
        for (int i = 0; i < totalpages; i++) 
        {
            pages[i] = new Page;
            pages[i]->Readdatafromfile(fin);
            //pages[i]->print();
        }
        fin.close();
    }
    void loadposts(const char* postfile) 
    {
        int totalpost;
        ifstream fin;
        fin.open(postfile);
        fin >> totalpost;
        char buffer[50];
        char id[10];
        object* ptr2;
        post::setpost(totalpost);
        posts = new post * [totalpost];
        for (int i = 0; i < totalpost; i++)
        {
            posts[i] = new post();
            posts[i]->readdatafromfile(fin);
            //posts[i]->print();
            fin >> buffer;
            object* objptr = search_object_by_ID(buffer);
            posts[i]->setsharedby(objptr);
            char id[30] = "";
            while (id[0] != '-')
            {
                fin >> id;
                if (id[0] == 'u') 
                    ptr2 = search_friend(id);
                else if (id[0] == '-')
                    break;
                else
                    ptr2 = search_page_by_ID(id);
                posts[i]->likedby(ptr2);
            }
            objptr->post_on_timeline(posts[i]);
        }
        fin.close();
    }
    void Loadusers(const char* userfile) 
    {
        int totalusers;
        ifstream fin;
        fin.open(userfile);
        if (!fin.is_open())
            cout << "Not Open\n";
        fin >> totalusers;
        //cout << totalusers << endl;
        User::setuser(totalusers);
        users = new User * [totalusers];
        char*** list = new char** [20];
        for (int i = 0; i < totalusers; i++) 
        {
            users[i] = new User;
            users[i]->Readdatafromfile(fin);
            //cout << "user";
            //users[i]->print();
            char tempbuffer[50] = "";
            char* temp;
            //cout << "total users " << totalusers << endl;

            int j = 0;
            char* pid;
            list[i] = new char* [10];
            while (tempbuffer[0] != '-')
            {
                fin >> tempbuffer;
                if (tempbuffer[0] == '-')
                    break;
                else 
                {
                    //cout << tempbuffer << "buffer" << endl;
                    temp = Helper::bufferingString(tempbuffer);
                    list[i][j] = temp;
                    //cout << "list" << list[i][j] << endl;;
                    j++;
                }
            }
            list[i][j] = nullptr;
            Page* ptr;
            char tempbuffer2[20] = "";
            fin >> tempbuffer2;
            while (tempbuffer2[0] != '-') 
            {
                if (tempbuffer2[0] == '-')
                    break;
                cout << tempbuffer2;
                pid = Helper::bufferingString(tempbuffer2);
                ptr = search_page_by_ID(pid);
                users[i]->Likedpages(ptr);
                fin >> tempbuffer2;
            }
        }

        User* userPtr;
        for (int k = 0; k < totalusers; k++)
            for (int j = 0; list[k][j] != nullptr; j++) 
            {
                userPtr = search_friend(list[k][j]);
                users[k]->Addfriends(userPtr);
            }
        deallocatefriend(list, totalusers);
    }
    Page* search_page_by_ID(char* page) {
        int flag = 0;
        int total = Page::getpage();
        //cout << total;
        for (int i = 0; i < total; i++)
        {
            flag = 0;
            // pages[i]->print();
            char* ptr2 = pages[i]->getID();
            if (flag == 0)
            {
                for (int j = 0; page[j] != '\0'; j++)
                {
                    if (page[j] == ptr2[j])
                        flag = 0;
                    else
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                    return pages[i];
            }
        }
        return 0;
    }
    User* search_friend(char* user)
    {
        int total = User::getuser();
        char* check;
        int flag = 0;
        for (int i = 0; i < total; i++) 
        {
            flag = 0;
            check = users[i]->getID();
            if (flag == 0) 
            {
                for (int j = 0; user[j] != '\0'; j++) 
                {
                    if (user[j] == check[j])
                        flag = 0;
                    else 
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                    return users[i];
            }
        }
        return 0;
    }
    object* search_object_by_ID(char* Object) 
    {
        object* ptr;
        if (Object[0] == 'u')
            ptr = search_friend(Object);
        else
            ptr = search_page_by_ID(Object);
        return ptr;
    }
    post* search_post_by_id(char* ID) 
    {
        int total = post::getpost();
        int flag = 0;

        for (int i = 0; i < total; i++)
        {
            flag = 0;
            char* ptr2 = posts[i]->getID();
            if (flag == 0) 
            {
                for (int j = 0; ID[j] != '\0'; j++)
                {
                    if (ID[j] == ptr2[j])
                        flag = 0;
                    else
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0)
                    return posts[i];
            }
        }
        return posts[0];
    }
    void run()
    {
        system("CLS");
        cout << endl;
        cout << "Command :        Set Current system Date 15/11/2017" << endl;
        cout << "System Date:     15/11/2017            " << endl;

        date::setcurrentdate(15, 11, 2017);
        char ID[] = "u7";
        cout << "Command : set current user 'u7'" << endl;
        User* currentuser = search_friend(ID);
        cout << currentuser->getfirst() << " " << currentuser->getlast() << " successfully sent as current user  " << endl;

        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << " Command :     View Friend List " << endl;
        cout << "............................................................................" << endl;
        cout << currentuser->getfirst() << " " << currentuser->getlast() << " --- Friend List " << endl << endl;
        cout << endl;
        currentuser->viewfriendlist();
        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << " Command :     View liked pages " << endl;
        cout << "............................................................................" << endl;
        cout << currentuser->getfirst() << " " << currentuser->getlast() << " --- Liked Pages " << endl << endl;
        currentuser->viewlikedpaged();
        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << "Command :       View Home" << endl;
        cout << "............................................................................" << endl;
        cout << currentuser->getfirst() << " " << currentuser->getlast() << " --- Home Page " << endl << endl;
        currentuser->print();
        currentuser->display_home_page();
        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << "Command :      View TimeLine " << endl;
        cout << "............................................................................" << endl;
        cout << currentuser->getfirst() << " " << currentuser->getlast() << " --- Time Line " << endl << endl;
        currentuser->print();
        currentuser->viewtimeline();
        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << "Command:      LikePost(post5) " << endl << endl;
        char temp[] = "post5";
        currentuser->print();
        post* postptr = search_post_by_id(temp);
        postptr->viewlikeby();
        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << "Command:      LikePost(post5) " << endl;
        cout << "Command:      ViewLikedList(post5)" << endl << endl;
        object* objectptr = search_object_by_ID(ID);
        postptr->likedby(objectptr);
        postptr->viewlikeby();

        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << "Command:      PostComment(post4, Good Luck for your Result) " << endl;
        cout << "Command:      View Post (post4)" << endl << endl;
        char temp1[] = "post4";
        postptr = search_post_by_id(temp1);
        object* ptr = postptr->getsharedby();
        cout << "...";
        ptr->displayName();
        cout << " shared ";
        cout << postptr->gettext();
        cout << endl;
        postptr->printcomments();
        cout << "\n............................................................................" << endl;
        cout << "Command:      PostComment(post8, Thanks for the wishes) " << endl;
        cout << "Command:      View Post (post8)" << endl << endl;
        char temp2[] = "post8";
        postptr = search_post_by_id(temp2);
        char text[] = "Thanks for the wishes";
        postptr->Addcomment(text, currentuser);
        int type = postptr->getposttype();
        ptr = postptr->getsharedby();
        cout << "...";
        ptr->displayName();
        cout << " shared ";
        if (type == 2)
            postptr->printActivity();
        cout << postptr->gettext();
        cout << endl;
        postptr->printcomments();


        char post4[] = "post10";
        char comment3[] = "Never thought I will be specialist in this field...";
        post* Post4 = search_post_by_id(post4);

        Memory* mem = new Memory(Post4, comment3, currentuser);

        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << "Command:     See Your Memories\n";
        cout << "............................................................................" << endl;
        currentuser->viewmemories();

        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << "Command:     Share Memory (" << post4 << "," << comment3 << ")\n";
        cout << "Command:     View timeline " << endl << endl << endl;
        cout << currentuser->getfirst() << " " << currentuser->getlast() << " --- Time Line " << endl << endl;
        //mem->viewPost();
        /*char post10[] = "post10";
        char comment3[] = "Never thought I will be specialist in this field...";
        post* Post10 = search_post_by_id(post10);*/
        currentuser->viewmemories();
        cout << endl;
        currentuser->viewtimeline();

        cout << endl << endl;
        cout << "............................................................................" << endl;
        cout << "Command:       View Page (p1) " << endl;
        cout << "............................................................................" << endl;
        char temp3[] = "p1";
        Page* pageptr = search_page_by_ID(temp3);
        pageptr->viewpage();
        cout << endl << endl;
    }
    void deallocatefriend(char*** friendlist, int total)
    {
        if (friendlist == nullptr)
            return;
        for (int i = 0; i < total; i++)
            delete[] friendlist[i];
        delete[] friendlist;
    }
    void postcomments(const char* postid, const char* text, const char* userid)
    {
        int length = strlen(postid);
        char ID[6] = "";
        for (int i = 0; i < length; i++)
            ID[i] = postid[i];
        post* post = search_post_by_id(ID);
    }
    void viewhome();
    ~SocialNetworkApplication()
    {
        /*for (int i = 0; i < comment::total; i++)
           delete[] comments[i];
        for (int i = 0; i < post::totalpost; i++)
            delete[] posts[i];
        for (int i = 0; i < User::totaluser; i++)
            delete[] users[i];
        for (int i = 0; i < Page::totalpage; i++)
            delete[] posts[i];*/
        delete[] pages;
        delete[] users;
        delete[] comments;
        delete[] posts;
    }
};

int main() 
{
    SocialNetworkApplication SNA;
    SNA.LoadData("Pagelist.txt", "Friendslist.txt", "Posts.txt", "Comments.txt");
    SNA.run();
    system("pause");
    return 0;
}