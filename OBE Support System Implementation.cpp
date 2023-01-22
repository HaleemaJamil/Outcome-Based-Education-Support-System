#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<cstring>
#include <vector>
#include <fstream>
using namespace std;

class Course;
class CLO;
class Question;
class OBE;
class Evaluation;

class Question
{
private:
    int Eid;
    int QId;
    int CLO_Id;
    char description[100];

public:
	//default constructor
    Question(int Eid = -1, int QId = -1, int CLO_Id = -1, string description = "")
    {
        this->Eid = Eid;
        this->QId = QId;
        this->CLO_Id = CLO_Id;
        strcpy(this->description, description.c_str());
    }

	//getters
    int getQId() {
        return QId;
    }
    int getCLO_Id() {
        return CLO_Id;
    }
    int getEid() {
        return Eid;
    }

	//setters
    void setQId(int id) {
        QId = id;
    }
    void setCLO_Id(int id) {
        CLO_Id = id;
    }
    void setEid(int id) {
        Eid = id;
    }
    void setDes(string des) {
        strcpy(this->description, des.c_str());
    }

	//print Question
    void print() {
        cout << "Question id: " << QId << "\n";
        cout << "CLO id: " << CLO_Id << "\n";
        cout << "Description: " << description << "\n";
    }
};

class Evaluation
{
private:
    int Cid;
    int id;
    vector<Question> questions;

public:
    Evaluation(int Cid = 0, int id = 0) {
        this->Cid = Cid;
        this->id = id;
    }

    int getId() {
        return id;
    }
    int getCid() {
        return Cid;
    }
    void setId(int id) {
        this->id = id;
    }
    void setCid(int Cid) {
        this->Cid = Cid;
    }

    int addQuestion(int Eid, int Q_No, int CLO_Id, string des)
    {
        Question obj(Eid, Q_No, CLO_Id, des);

        //adding question to the vector
        questions.push_back(obj);

        //adding question to the binary file
        ofstream f("questions.bin", ios::binary | ios::app);
        if (!f)
            return 1;
        else {
            f.write((char*)&obj, sizeof(obj));
            f.close();
            return 0;
        }
    }
    int RemoveQuestion(int Eid, int Q_No, int CLO_Id)
    {
        bool found = false;
        int index = 0;

        //finding the index of the question to be deleted
        int size = questions.size();
        while (index < size) {
            if (questions[index].getEid() == Eid && questions[index].getQId() == Q_No && questions[index].getCLO_Id() == CLO_Id) {
                found = true;
                break;
            }
            else
                index++;
        }

        //if question present
        if (found) {
            //deleting question from the vector

            vector<Question>::iterator it;
            it = index + questions.begin();
            questions.erase(it);

            //deleting the question from the binary file

            ifstream fquestion("questions.bin", ios::binary);
            ofstream dummy("dummy.bin", ios::binary);

            Question obj;

            while (fquestion.read((char*)&obj, sizeof(obj)))
            {
                if (obj.getCLO_Id() != CLO_Id || obj.getQId() == Q_No || obj.getEid() == Eid)
                {
                    dummy.write((char*)&obj, sizeof(obj));
                }
            }
            fquestion.close();
            dummy.close();

            remove("questions.bin");
            if (rename("dummy.bin", "questions.bin"))
                return 1;
            else
                return 0;
        }
    }
    int UpdateQuestion(int Eid, int Q_No, int CLO_Id, int newQ_No, int newCLO_Id, string newdes)
    {
        int index = 0;
        bool found = false;

        //finding the index of the question to be updated
        int size = questions.size();
        while (index < size) {
            if (questions[index].getEid() == Eid && questions[index].getQId() == Q_No && questions[index].getCLO_Id() == CLO_Id) {
                found = true;
                break;
            }
            else
                index++;
        }

        //if question present
        if (found)
        {
            //updating question in vector
            questions[index].setQId(newQ_No);
            questions[index].setCLO_Id(newCLO_Id);
            questions[index].setDes(newdes);

            //updating question in Binary file

            fstream questionf2("questions.bin", ios::in | ios::out | ios::binary);
            Question obj;

            while (questionf2.read((char*)&obj, sizeof(obj)))
            {
                if (obj.getEid() == Eid && obj.getQId() == Q_No && obj.getCLO_Id() == CLO_Id)
                {
                    obj.setQId(newQ_No);
                    obj.setCLO_Id(newCLO_Id);
                    obj.setDes(newdes);

                    std::streamoff fileptr = questionf2.tellg();
                    std::streamoff objsize = sizeof(obj);
                    questionf2.seekp(fileptr - objsize, ios::beg);

                    questionf2.write((char*)&obj, sizeof(obj));
                    questionf2.close();
                    return 1;
                }
            }
            questionf2.close();
            return 0;
        }
    }

    void deleteAllQuestions() {
        questions.clear();
    }
    int countQuestions(int CLO_Id) {
        int count = 0;
        int size = questions.size();
        for (int i = 0; i < size; i++)
        {
            if (questions[i].getCLO_Id() == CLO_Id) {
                count++;
            }
        }
        return count;
    }
    bool checkCLOHasTested(int CLO_Id) {
        if (countQuestions(CLO_Id) >= 2) {
            return true;
        }
        else
            return false;
    }

    void print() {
        cout << "Evaluation id: " << id << "\n";
    }
    void printAllQuestions() {
        for (int i = 0; i < questions.size(); i++) {
            questions[i].print();
        }
    }

    bool operator==(const Evaluation& c) {
        if (Cid == c.Cid && id == c.id) {
            return 1;
        }
        else
            return 0;
    }
};

class CLO
{
private:
    int Cid;
    int id;
    char name[50];
public:
	//default constructor
    CLO(int Cid = -1, int id = -1, string name = "") {
        this->Cid = Cid;
        this->id = id;
        strcpy(this->name, name.c_str());
    }

	//getters
    int getCid() {
        return Cid;
    }
    int getId() {
        return id;
    }
    char* getName() {
        return name;
    }

	//setters
    void setCid(int Cid) {
        this->Cid = Cid;
    }
    void setId(int id) {
        this->id = id;
    }
    void setName(string name) {
        strcpy(this->name, name.c_str());
    }

	//print CLO
    void print() {
        cout << "CLO id: " << id << "\n";
        cout << "CLO name: " << name << "\n";
    }

	//operator == overloaded
    bool operator==(const CLO& c) {
        if (Cid == c.Cid && id == c.id && (strcmp(name, c.name) == 0)) {
            return 1;
        }
        else
            return 0;
    }
};

class Course
{
private:
    int id;
    char name[50];
    vector<CLO> CLOs;
    vector<Evaluation> evals;
public:
    Course(int id = -1, string name = "") {
        this->id = id;
        strcpy(this->name, name.c_str());
    }

    int addCLO(int Cid, int id, string name)
    {
        CLO obj(Cid, id, name);

        //adding CLO to the vector

        CLOs.push_back(obj);

        //adding CLO to the binary file

        ofstream f("CLOs.bin", ios::binary | ios::app);
        if (!f)
            return 1;
        else {
            f.write((char*)&obj, sizeof(obj));
            f.close();
            return 0;
        }
    }
    int removeCLO(int Cid, int id) {
        bool found = false;
        int index = 0;

        //finding the index of the CLO to be deleted
        int size = CLOs.size();
        while (index < size) {
            if (CLOs[index].getId() == id && CLOs[index].getCid() == Cid) {
                found = true;
                break;
            }
            else
                index++;
        }

        //if CLO present

        if (found) {
            //deleting CLO from the vector

            vector<CLO>::iterator it;
            it = index + CLOs.begin();
            CLOs.erase(it);

            //deleting the CLO from the binary file

            ifstream f("CLOs.bin", ios::binary);
            ofstream temp("temp.bin", ios::binary);

            CLO obj;

            while (f.read((char*)&obj, sizeof(obj)))
            {
                if (obj.getId() != id || obj.getCid() != Cid)
                {
                    temp.write((char*)&obj, sizeof(obj));
                }
            }
            f.close();
            temp.close();

            remove("CLOs.bin");
            if (rename("temp.bin", "CLOs.bin"))
                return 1;
            else
                return 0;
        }
    }
    int updateCLO(int Cid, int id, string name, int newId, string newName)
    {
        int index = 0;
        bool found = false;

        //finding the index of the CLO to be updated
        int size = CLOs.size();
        while (index < size) {
            if (CLOs[index].getId() == id && CLOs[index].getCid() == Cid) {
                found = true;
                break;
            }
            else
                index++;
        }

        //if CLO present

        if (found)
        {
            //updating CLO in vector

            CLOs[index].setId(newId);
            CLOs[index].setName(newName);

            //updating CLO in Binary file

            fstream f("CLOs.bin", ios::in | ios::out | ios::binary);
            CLO obj;

            while (f.read((char*)&obj, sizeof(obj)))
            {
                if (id == obj.getId() && Cid == obj.getCid())
                {
                    obj.setId(newId);
                    obj.setName(newName);

                    std::streamoff fileptr = f.tellg();
                    std::streamoff objsize = sizeof(obj);
                    f.seekp(fileptr - objsize, ios::beg);

                    f.write((char*)&obj, sizeof(obj));
                    f.close();
                    return 1;
                }
            }
            f.close();
            return 0;
        }
    }

    int addEval(int Cid, int id)
    {
        Evaluation obj(Cid, id);

        //adding Evaluation to the vector
        evals.push_back(obj);

        //adding evaluation to the binary file
        ofstream f("evaluations.bin", ios::binary | ios::app);
        if (!f)
            return 1;
        else {
            f.write((char*)&obj, sizeof(obj));
            f.close();
            return 0;
        }
    }
    int removeEval(int Cid, int id) {
        bool found = false;
        int index = 0;

        //finding the index of the evaluation to be deleted
        int size = evals.size();
        while (index < size) {
            if (evals[index].getId() == id && evals[index].getCid() == Cid) {
                found = true;
                break;
            }
            else
                index++;
        }

        //if evaluation present
        if (found)
        {
            evals[index].deleteAllQuestions();

            //deleting evaluation from the vector
            vector<Evaluation>::iterator it;
            it = index + evals.begin();
            evals.erase(it);

            //deleting the evalutaion from the binary file
            ifstream f("evaluations.bin", ios::binary);
            ofstream temp("temp2.bin", ios::binary);

            Evaluation obj;

            while (f.read((char*)&obj, sizeof(obj)))
            {
                if (obj.getId() != id || obj.getCid() != Cid)
                {
                    temp.write((char*)&obj, sizeof(obj));
                }
            }
            f.close();
            temp.close();

            remove("evaluations.bin");
            if (rename("temp2.bin", "evaluations.bin"))
                return 1;
            else
                return 0;
        }
    }
    int updateEval(int Cid, int id, int newId)
    {
        int index = 0;
        bool found = false;

        //finding the index of the evaluation to be updated
        int size = evals.size();
        while (index < size) {
            if (evals[index].getId() == id && evals[index].getCid() == Cid) {
                found = true;
                break;
            }
            else
                index++;
        }

        //if evaluation present
        if (found)
        {
            //updating evaluation in vector
            evals[index].setId(newId);

            //updating evaluation in Binary file
            fstream f("evaluations.bin", ios::in | ios::out | ios::binary);
            Evaluation obj;

            while (f.read((char*)&obj, sizeof(obj)))
            {
                if (id == obj.getId() && Cid == obj.getCid())
                {
                    obj.setId(newId);

                    std::streamoff fileptr = f.tellg();
                    std::streamoff objsize = sizeof(obj);
                    f.seekp(fileptr - objsize, ios::beg);

                    f.write((char*)&obj, sizeof(obj));
                    f.close();
                    return 1;
                }
            }
            f.close();
            return 0;
        }
    }

    int getId() {
        return id;
    }
    char* getName() {
        return name;
    }
    void setId(int id) {
        this->id = id;
    }
    void setName(string name) {
        strcpy(this->name, name.c_str());
    }

    void print() {
        cout << "Course id: " << id << "\n";
        cout << "Course name: " << name << "\n";
    }
    void printallCLOs() {
        int size = CLOs.size();
        cout << "Course id: " << id << "\n";
        for (int i = 0; i < size; i++) {
            CLOs[i].print();
        }
    }
    void printallEvals() {
        int size = evals.size();
        cout << "Course id: " << id << "\n";
        for (int i = 0; i < size; i++)
        {
            evals[i].print();
        }
    }

    int doesEvalExist(int Eid)
    {
        //finding the index of the evaluation to be searched
        int size = evals.size();
        int index = 0;

        while (index < size) {
            if (evals[index].getId() == Eid) {
                return index;
            }
            index++;
        }
        return -1;
    }

    void addQuestionToEvaluation(int index, int Qid, int CLO_id, string des)
    {
        evals[index].addQuestion(evals[index].getId(), Qid, CLO_id, des);
    }
    void removeQuestionFromEvaluation(int index, int Qid, int CLO_id) {
        evals[index].RemoveQuestion(evals[index].getId(), Qid, CLO_id);
    }
    void updateQuestionFromEvaluation(int index, int Qid, int CLO_id, int newQid, int newCLO_id, string newdes) {
        evals[index].UpdateQuestion(evals[index].getId(), Qid, CLO_id, newQid, newCLO_id, newdes);
    }

    void printQuestionsInEval() {
        for (int i = 0; i < evals.size(); i++)
        {
            cout << "Evaluation id: " << evals[i].getId();
            cout << "\n";
            evals[i].printAllQuestions();
        }
    }
    void removeAllCLOs() {
        CLOs.clear();
    }
    void removeAllEvals() {
        for (int i = 0; i < evals.size(); i++) {
            evals[i].deleteAllQuestions();
        }
        evals.clear();
    }
    
    int checkAllCLOsHasTested() 
    {
        bool tested = false;

        for (int i = 0; i < CLOs.size(); i++)
        {
            for (int j = 0; j < evals.size(); i++) {
                if (evals[j].checkCLOHasTested(CLOs[i].getId()) == true) {
                    tested = true;
                    break;
                }
            }
            if (!tested)
                return 0;
        }
        return 1;
    }

    bool operator==(const Course& c) {
        if (id == c.id && (strcmp(name, c.name) == 0)) {
            return 1;
        }
        else
            return 0;
    }
};

class Interface {
public:
    vector<Course> courses;
    vector<Course>::iterator itr2;

    void populateCourseVector() {
        ifstream f("courses.bin", ios::in | ios::binary);
        Course obj;

        while (f.read((char*)&obj, sizeof(obj)))
        {
            courses.push_back(obj);
        }
        f.close();
    }

    int addCourse(int id, string name)
    {
        Course obj(id, name);

        //adding course to the vector
        courses.push_back(obj);

        //adding course to the binary file
        ofstream f("courses.bin", ios::binary | ios::app);
        if (!f)
            return 1;
        else {
            f.write((char*)&obj, sizeof(obj));
            f.close();
            return 0;
        }
    }
    void updateCourse(int id, string name, int newId, string newName)
    {
        int index = 0;
        bool found = false;

        //finding the index of the course to be updated
        int size = courses.size();
        while (index < size) {
            if (courses[index].getId() == id && courses[index].getName() == name) {
                found = true;
                break;
            }
            else
                index++;
        }

        //if CLO present
        if (found)
        {
            //updating course in vector
            courses[index].setId(newId);
            courses[index].setName(newName);
        }
    }
    void removeCourse(int id, string name) 
    {
        bool found = false;
        int index = 0;

        //finding the index of the course to be deleted
        int size = courses.size();
        while (index < size)
        {
            if (courses[index].getId() == id && courses[index].getName() == name) {
                found = true;
                break;
            }
            else
                index++;
        }

        //go to CLOs remove all CLOs with this Cid
        //go to evals remove all evals with this Cid
        //go to questions remove all questions in the respective evals
        courses[index].removeAllCLOs();
        courses[index].removeAllEvals();
        //if CLO present
        if (found)
        {
            //deleting CLO from the vector
            vector<Course>::iterator it;
            it = index + courses.begin();
            courses.erase(it);
        }
    }
};

int menu() {
    int option = 0, optionToAdd = -1, optionToRemove = -1, optionToUpdate = -1, optionToPrint = -1;

    Interface I;
    //I.populateCourseVector();

    cout << "Welcome to the OBE System\n";
    cout << "-------------------------\n";

    while (option != -1) {

        cout << "\nDo you want to:\n";
        cout << "Option -1: Exit\n";
        cout << "Option 1: Add\n";
        cout << "Option 2: Remove\n";
        cout << "Option 3: Update\n";
        cout << "Option 4: Print\n";
        cout << "Option 5: Check\n";
        cout << "Option: ";
        cin >> option;

        if (option == 1) {
            cout << "\nDo you want to:\n";
            cout << "Option 1: Add a Course\n";
            cout << "Option 2: Add a CLO\n";
            cout << "Option 3: Add an Evaluation\n";
            cout << "Option 4: Add a Question\n";
            cout << "Option: ";
            cin >> optionToAdd;

            if (optionToAdd == 1)
            {
                //add a course
                int id;
                string name;

                cout << "Enter Course id: ";
                cin >> id;
                cout << "Enter Course name: ";
                cin >> name;

                I.addCourse(id, name);
                cout << "Added successfully!\n";
            }
            else if (optionToAdd == 2)
            {
                //add a CLO respective to a course
                int id, Cid;
                string name;

                int index = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched

                int size = I.courses.size();
                while (index < size) {
                    if (I.courses[index].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        index++;
                }
                if (found) {
                    cout << "Enter CLO id: ";
                    cin >> id;
                    cout << "Enter CLO name: ";
                    cin >> name;
                    I.courses[index].addCLO(Cid, id, name);
                    cout << "Added successfully!\n";
                }
                else
                    cout << "Incorrect Course id!\n";

            }
            else if (optionToAdd == 3) {
                //add an evaluation respective to a course
                int id, Cid;

                int index = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched

                int size = I.courses.size();
                while (index < size) {
                    if (I.courses[index].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        index++;
                }
                if (found) {
                    cout << "Enter Evaluation id: ";
                    cin >> id;
                    I.courses[index].addEval(Cid, id);
                    cout << "Added successfully!\n";
                }
                else
                    cout << "Incorrect Course Id.\n";
            }
            else if (optionToAdd == 4)
            {
                //add a question respective to a course
                int Qid, CLO_id, Eid, Cid;
                string des;

                int i = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched
                int size = I.courses.size();
                while (i < size) {
                    if (I.courses[i].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        i++;
                }
                if (found)
                {
                    cout << "Enter corresponding Evaluation ID: ";
                    cin >> Eid;

                    //finding the index of the Evaluation to be searched
                    int index = I.courses[i].doesEvalExist(Eid);

                    if (index != -1)
                    {
                        cout << "Enter Question id: ";
                        cin >> Qid;
                        cout << "Enter CLO id: ";
                        cin >> CLO_id;
                        cout << "Enter Question description: ";
                        cin >> des;
                        I.courses[i].addQuestionToEvaluation(index, Qid, CLO_id, des);
                        cout << "Added successfully!\n";
                    }
                    else
                        cout << "Incorrect Evaluation id!\n";
                }
                else
                    cout << "Incorrect Course id!\n";

            }
            else {
                return -1;
            }
        }
        else if (option == 2) {
            cout << "\nDo you want to:\n";
            cout << "Option 1: Remove a Course\n";
            cout << "Option 2: Remove a CLO\n";
            cout << "Option 3: Remove an Evaluation\n";
            cout << "Option 4: Remove a Question\n";
            cout << "Option: ";
            cin >> optionToRemove;

            if (optionToRemove == 1) {
                //remove a course
                int id;
                string name;

                cout << "Enter Course id: ";
                cin >> id;
                cout << "Enter Course name: ";
                cin >> name;
                I.removeCourse(id, name);
                cout << "Removed successfully\n";
            }
            else if (optionToRemove == 2) {
                //remove a CLO respective to a course
                int id, Cid;
                int index = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched
                int size = I.courses.size();
                while (index < size) {
                    if (I.courses[index].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        index++;
                }
                if (found) {
                    cout << "Enter CLO id: ";
                    cin >> id;
                    I.courses[index].removeCLO(Cid, id);
                    cout << "Removed successfully!\n";
                }
                else
                    cout << "Incorrect Course Id.\n";

            }
            else if (optionToRemove == 3) {
                //remove a evaluation respective to a course
                int id, Cid;
                int index = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched
                int size = I.courses.size();
                while (index < size) {
                    if (I.courses[index].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        index++;
                }
                if (found) {
                    cout << "Enter Evaluation id: ";
                    cin >> id;
                    I.courses[index].removeEval(Cid, id);
                    cout << "Removed successfully!\n";
                }
                else
                    cout << "Incorrect Course Id.\n";
            }
            else if (optionToRemove == 4)
            {
                //remove a question respective to a course
                int Qid, CLO_id, Eid, Cid;
                string des;

                int i = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched
                int size = I.courses.size();
                while (i < size) {
                    if (I.courses[i].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        i++;
                }
                if (found)
                {
                    cout << "Enter corresponding Evaluation ID: ";
                    cin >> Eid;

                    //finding the index of the Evaluation to be searched
                    int index = I.courses[i].doesEvalExist(Eid);

                    if (index != -1)
                    {
                        cout << "Enter Question id: ";
                        cin >> Qid;
                        cout << "Enter CLO id: ";
                        cin >> CLO_id;

                        I.courses[i].removeQuestionFromEvaluation(index, Qid, CLO_id);
                        cout << "Removed successfully!\n";
                    }
                    else
                        cout << "Incorrect Evaluation id!\n";
                }
                else
                    cout << "Incorrect Course id!\n";
            }
            else {
                return -1;
            }
        }
        else if (option == 3) {
            cout << "\nDo you want to:\n";
            cout << "Option 1: Update a Course\n";
            cout << "Option 2: Update a CLO\n";
            cout << "Option 3: Update an Evaluation\n";
            cout << "Option 4: Update a Question\n";
            cout << "Option: ";
            cin >> optionToUpdate;

            if (optionToUpdate == 1) {
                //update a course
                int id, newId;
                string name, newName;

                cout << "Enter Course id: ";
                cin >> id;
                cout << "Enter Course name: ";
                cin >> name;
                cout << "Enter new Course id: ";
                cin >> newId;
                cout << "Enter new Course name: ";
                cin >> newName;

                I.updateCourse(id, name, newId, newName);
                cout << "Updated successfully!\n";
            }
            else if (optionToUpdate == 2) 
            {
                //update a CLO respective to a course
                int id, Cid, newId;
                string name, newName;
                int index = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched
                int size = I.courses.size();
                while (index < size) {
                    if (I.courses[index].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        index++;
                }
                if (found) {
                    cout << "Enter CLO id: ";
                    cin >> id;
                    cout << "Enter new CLO id: ";
                    cin >> newId;
                    cout << "Enter CLO name: ";
                    cin >> name;
                    cout << "Enter new CLO name: ";
                    cin >> newName;
                    I.courses[index].updateCLO(Cid, id, name, newId, newName);
                    cout << "Updated successfully!\n";
                }
                else
                    cout << "Incorrect Course Id.\n";
            }
            else if (optionToUpdate == 3) {
                //update an evaluation respective to a course
                int id, Cid, newId;
                int index = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched
                int size = I.courses.size();
                while (index < size) {
                    if (I.courses[index].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        index++;
                }
                if (found) {
                    cout << "Enter Evaluation id: ";
                    cin >> id;
                    cout << "Enter new Evaluation id: ";
                    cin >> newId;
                    I.courses[index].updateEval(Cid, id, newId);
                    cout << "Updated successfully!\n";
                }
                else
                    cout << "Incorrect Course Id.\n";
            }
            else if (optionToUpdate == 4)
            {
                //update a question respective to an evaluation
                int Qid, CLO_id, Eid, Cid, newQid, newCLO_id;
                string des;

                int i = 0;
                bool found = false;

                cout << "\nEnter corresponding Course ID: ";
                cin >> Cid;

                //finding the index of the Course to be searched
                int size = I.courses.size();
                while (i < size) {
                    if (I.courses[i].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        i++;
                }
                if (found)
                {
                    cout << "Enter corresponding Evaluation ID: ";
                    cin >> Eid;

                    //finding the index of the Evaluation to be searched
                    int index = I.courses[i].doesEvalExist(Eid);

                    if (index != -1)
                    {
                        cout << "Enter Question id: ";
                        cin >> Qid;
                        cout << "Enter CLO id: ";
                        cin >> CLO_id;
                        cout << "Enter new Question id: ";
                        cin >> newQid;
                        cout << "Enter new CLO id: ";
                        cin >> newCLO_id;
                        cout << "Enter new Question description: ";
                        cin >> des;

                        I.courses[i].updateQuestionFromEvaluation(index, Qid, CLO_id, newQid, newCLO_id, des);
                        cout << "Updated successfully!\n";
                    }
                    else
                        cout << "Incorrect Evaluation id!\n";
                }
                else
                    cout << "Incorrect Course id!\n";
            }
            else {
                return -1;
            }
        }
        else if (option == 4) {
            cout << "\nDo you want to:\n";
            cout << "Option 1: Print all Courses\n";
            cout << "Option 2: Print all CLOs wrt Courses\n";
            cout << "Option 3: Print all Evaluations wrt Courses\n";
            cout << "Option 4: Print all Questions\n";
            cout << "Option: ";
            cin >> optionToPrint;

            if (optionToPrint == 1) {
                //print all Courses();
                for (int i = 0; i < I.courses.size(); i++) {
                    I.courses[i].print();
                }
            }
            else if (optionToPrint == 2) {
                //Print all CLOs wrt Courses
                for (int i = 0; i < I.courses.size(); i++) {
                    I.courses[i].printallCLOs();
                }
            }
            else if (optionToPrint == 3) {
                //Print all Evaluations wrt Courses
                for (int i = 0; i < I.courses.size(); i++) {
                    I.courses[i].printallEvals();
                }
            }
            else if (optionToPrint == 4) {
                //Print all Questions wrt Courses
                for (int i = 0; i < I.courses.size(); i++)
                {
                    cout << "Course id: " << I.courses[i].getId();
                    cout << "\n";
                    I.courses[i].printQuestionsInEval();
                }
            }

        }
        else if (option == 5)
        {
            cout << "Option 1: Check All CLOs Tested\n";
            int option = 1;
            if (option == 1) 
            {
                int Qid, CLO_id, Eid, Cid;
                string des;

                int i = 0;
                bool found = false;

                cout << "\nEnter Course ID: ";
                cin >> Cid;

                //finding the index of the Course
                int size = I.courses.size();
                while (i < size) {
                    if (I.courses[i].getId() == Cid) {
                        found = true;
                        break;
                    }
                    else
                        i++;
                }
                if (found)
                {
                    if (I.courses[i].checkAllCLOsHasTested() == 1) {
                        cout << "All CLOs have been tested\n";
                    }
                    else
                        cout << "All CLOs have not been tested\n";
                }
            }
        }
        else if (option == -1) {
            return 0;
        }
    }

    return -1;
}

int main() {
    if (menu() == 0)
    {
        cout << "Task successful\n";
    }
    else
        cout << "Invalid option selected\n";
    return 0;
}
