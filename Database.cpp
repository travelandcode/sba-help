//Library Declaration
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>




//Global Varible Declaration
FILE *Database; //Database file
FILE *LoginDatabase;
int  field_size = 50, record_size = 213, end_of_file, choice_int, choice_int_1;
// char choice[4], choice_1[4]; //conditional storage variables
int curr=0;
int max=56;

//Date format
typedef struct{
	int day, month, year;
}Date;

//Record layout
typedef struct{
	int id;
	char fname[25], lname[25];
	Date DOB;
	char gender[10];
	int years_worked;
	char position[25];
	int late, absent;
	char address [50], phone[8], department[30];
    bool admin;
}Employee;

Employee employees[55];

typedef struct{
	int EmpId;
	char password[25];
}EmployeeLogin;

EmployeeLogin employeeLogins[55];

typedef struct{
	int first_n[2] = {4, 10}, last_n[2] = {16, 11}, bday[2] = {29, 10}, sex[2] = {41, 6}, phone[2] = {55, 8} , addr[2] = {65, 48}, LOE[2] = {113, 20}, department[2] = {135, 27}, position[2] = {164, 21}, time_l[2] = {187, 10}, time_a[2] = {199, 10}; 
}Field_information;

Field_information field;
//Declaration of Functions
int idSearch(int id);
void admin(Employee emp);
void user(Employee emp);
int run(int option);
int current_day(int day);
int current_time(int hours, int minutes, int seconds);
void create(int id);
void addToDatabase();
void addToLoginDatabase();
void readDatabase();
void readLoginDatabase();
void createAdmin();
void signUp();
void signIn();
void userMenu();
void edit();
void updateFile();
void display(int id);
void displayAll();
void department_input ();
void delete_record (int id);
void field_sort (int eof);
void select_sort_string (int seek_amount);

EmployeeLogin checkLogin(int id, char pass[25]);
Employee employeeInfo(int id);



int main()
{
   int choice;
    readDatabase();
    readLoginDatabase();
    if(Database==NULL)
	createAdmin();
    printf("Welcome to the Login System.\n");
    printf("1.Sign Up\n");
    printf("2.Sign In\n");
    scanf("%d",&choice);
    while(choice!=1 && choice!=2)
    {
        printf("Incorrect Option! Please Select Another Option\n");
         scanf("%d",&choice);
    }
    if(choice==1)
    signUp();
    else
    signIn();
	return 0;
}

void createAdmin()//creates Admin if database is empty
{
    employees[0].id=00;
    strcpy(employees[0].fname,"Justin");
    strcpy(employees[0].lname,"Walker");
    strcpy(employees[0].address,"GordonTown");
    strcpy(employees[0].gender,"Male");
    strcpy(employees[0].phone,"1234567");
    strcpy(employees[0].department,"IT and Support");
    strcpy(employees[0].position,"Head of Department");
    employees[0].DOB.day=13;
    employees[0].DOB.month=9;
    employees[0].DOB.year=2004;
    employees[0].years_worked=1;
    employees[0].absent=0;
    employees[0].absent=0;
    employees[0].admin=true;
    employeeLogins[0].EmpId=employees[0].id;
    strcpy(employeeLogins[0].password,"password");
	addToDatabase();
    addToLoginDatabase();
	curr++;
}

void signUp()//Sign Up
{
	printf("Please Sign Up\n");
	printf("Your Employee ID# is %02d\n",curr);
	employeeLogins[curr].EmpId=curr;
	printf("Password:");
	gets(employeeLogins[curr].password);
	printf("\nSignUp has been sucessful.");
	addToLoginDatabase();
    signIn();
}

void signIn()//Sign In
{
	int id;
	EmployeeLogin tempLogin;
    Employee tempEmp;
	char pass[25];
    system("pause");
    system("cls");
	printf("Please Sign In\n");
	printf("Employee ID#:");
	scanf("%02d",&id);
    fflush(stdin);
	printf("Password:");
	gets(pass);
	fflush(stdin);
	tempLogin= checkLogin(id,pass);
    if(tempLogin.EmpId!=-1)
    {
        tempEmp=employeeInfo(tempLogin.EmpId);
        if(tempEmp.admin)//checks to see if employee is an admin
        admin(tempEmp);
        else
        user(tempEmp);
    }
    else{
        printf("Username or password is incorrect.\n");
        signIn();
    }
}



EmployeeLogin checkLogin(int id, char pass[25])
{
	EmployeeLogin login;
	login.EmpId=id;
	int found=-1;
	int pos=0;
	while(pos<curr && found==-1)
	{
		if(employeeLogins[pos].EmpId==login.EmpId)
		{ 
			found=1;
            strcpy(employeeLogins[pos].password,pass);
			break;
		}
		else
		pos++;
	}
	if(found==-1)
	{
		login.EmpId=-1;
		return login;
	}
	else
	return login;
}

Employee employeeInfo(int id)//locates the employees information and returns it
{
    for(int i=0; i<=curr;i++)
    {
        if(employees[i].id==id)
        {
            return employees[i];
        }
    }
}

void readDatabase()//reads information from the file on opening the application
{
    char admin[5];
	int linecheck;
	Database=fopen("Employees.txt","r");
	if(Database==NULL)
	{
		printf("\n\tNO EMPOLYEE INFO SAVED...\n\t");
		system("PAUSE");
		system("CLS");
	}
	else
	{
		while(!feof(Database))
		{
           linecheck=fgetc(Database);
           if(linecheck=='\n')
           {
                
           		fscanf(Database,"%02d  %-10s  %-11s  %02d/%02d/%4d  %-6s  (876)-%8s  %-45s  %-20d  %-27s  %-19s  %-10d  %-12d %s\n", &employees[curr].id, &employees[curr].fname, &employees[curr].lname, &employees[curr].DOB.day, &employees[curr].DOB.month, &employees[curr].DOB.year, &employees[curr].gender, &employees[curr].phone,&employees[curr].address, &employees[curr].years_worked, &employees[curr].department, &employees[curr].position, &employees[curr].late, &employees[curr].absent,&admin);
			   	curr++;
                if(strcmp(admin,"true")==0)
                employees[curr].admin=true;
                else
                employees[curr].admin=false;
		   }
		}
	}
	fclose(Database);
}

void readLoginDatabase()//reads information from the file on opening the application
{
	int linecheck, pos=0;
	LoginDatabase=fopen("Employee-Logins.txt","r");
	if(LoginDatabase==NULL)
	{
		printf("\n\tNO EMPOLYEE LOGINS INFO SAVED...\n\t");
		system("PAUSE");
		system("CLS");
	}
	else
	{
		while(!feof(LoginDatabase))
		{
           linecheck=fgetc(LoginDatabase);
           if(linecheck=='\n')
           {
           		fscanf(LoginDatabase,"%02d  %-25s\n", &employeeLogins[pos].EmpId, &employeeLogins[pos].password);
			   	pos++;
		   }
		}
	}
	fclose(LoginDatabase);
}

void addToDatabase()//When an Employee's record is added, it is then added to the database
{
	Database=fopen("Employees.txt","a");
	if(Database==NULL)
	{
		printf("\n\tFILE NOT OPENED");
	}
	else
	{
		fprintf(Database,"%02d\t%s\t%-8s\t%02d/%02d/%4d\t%s\t(876)-%s\t%s\t%d\t%s\t%s\t%d\t%d\t%s\n", employees[curr].id, employees[curr].fname, employees[curr].lname, employees[curr].DOB.day, employees[curr].DOB.month, employees[curr].DOB.year, employees[curr].gender, employees[curr].phone,employees[curr].address, employees[curr].years_worked, employees[curr].department, employees[curr].position, employees[curr].late, employees[curr].absent,"false");
		printf("\n\t NEW EMPLOYEE INFO SAVED...\n\t");
		system("PAUSE");
		system("CLS");
	}
	fclose(Database);
}

void addToLoginDatabase()//When an Employee's record is added, it is then added to the database
{
	LoginDatabase=fopen("Employee-Logins.txt","a");
	if(LoginDatabase==NULL)
	{
		printf("\n\tFILE NOT OPENED");
	}
	else
	{
		fprintf(LoginDatabase,"%02d\t%s\n", employeeLogins[curr].EmpId, employeeLogins[curr].password);
		printf("\n\t NEW EMPLOYEE LOGIN INFO SAVED...\n\t");
		system("PAUSE");
		system("CLS");
	}
	fclose(LoginDatabase);
}

int idSearch(int id)//searches to see if Employee ID# exists
{
	for(int i=0;i<=curr;i++)
	{
		if(employees[i].id==id)
		{
			return i;
		}
	}	
	return -1;
}

int current_day(int day){
    switch (day)
    {
        case 0: printf("Today is Sunday\n"); break;
        case 1: printf("Today is Monday\n"); break;
        case 2: printf("Today is Tuesday\n"); break;
        case 3: printf("Today is Wednesday\n"); break;
        case 4: printf("Today is Thursday\n"); break;
        case 5: printf("Today is Friday\n"); break;
        case 6: printf("Today is Saturday\n"); break;
    }
    if(day==0 || day==6)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int current_time(int hours, int minutes, int seconds){
    printf("You clocked in at %02d:%02d:%02d \n",hours, minutes,
   seconds);
    if(hours>=8)// If worker has reached after 8 AM they are deemed to be late
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void admin(Employee emp)//only  Head of Departments have admin control
{
	int choice;
	char opt;
    system("cls");
    printf("<ENTERING ADMINISTRATOR MODE>\n");
    Sleep(3000);
    printf ("Hello %s %s\n",emp.fname,emp.lname);
    system("pause");
    system("cls");
    fflush(stdin);
    Sleep(3000);
    while (choice != 7)
    {
		printf("What would you like to do today?\n\n");
		printf("1.Create Record(s)\n");
		printf("2.Edit Record(s)\n");
		printf("3.Show a Record\n");
		printf("4.Show all records)\n");
		printf("5.Delete Record\n");
		printf("6.Sort Records\n");
		printf("7.Exit\n");
		printf("Please Select One of the Options Above\n");
		scanf("%d",&choice);
        system("pause");
		system("cls");
        Sleep(3000);
		run(choice);
		system("pause");
		system("cls");
		printf("\nWould You Like to Continue?[Y/N]\n");
		scanf("%c",opt);
		if(opt!='y'||opt!='Y')
		{
			break;
		}
	}
	printf("<EXITING ADMIN MODE>\n"); 
	Sleep(3000);
	system("pause");
	system ("cls");
    
}

void user(Employee emp)
{
    time_t a;
    struct tm *curr_time;
    a = time(0);
    curr_time = localtime(&a);
    int lateness, absent,pos,id,day_check,time_check;
     day_check = current_day(curr_time->tm_wday);
	if (day_check == -1)
	{
	    printf("Sorry, no registration on the weekend. Love the enthusiasm though.:)");
	}
	else
	{
	    time_check = current_time(curr_time->tm_hour, curr_time->tm_min, curr_time->tm_sec);
	    if (time_check ==  -1)
		{
		    emp.late++;
	    	printf("You are late. Try to do better next time.");
	    }
	    else
		{
	    	emp.absent++;
	    	printf(":(");
	    }
	}
    userMenu();
    pos=idSearch(emp.id);
    employees[pos]=emp;
    updateFile();
}

void userMenu()
{
    int choice;
    char opt;
    while (choice != 3)
    {
    	system("cls");
		printf("What would you like to do today?\n\n");
		printf("1.Edit Record\n");
		printf("2.Show Record\n");
		printf("3.Exit\n");
		printf("Please Select One of the Options Above\n");
		scanf("%d",&choice);
		run(choice);
		system("pause");
		system("cls");
		printf("\nWould You Like to Continue?[Y/N]\n");
		scanf("%c",opt);
		if(opt!='y'||opt!='Y')
		{
			break;
		}
	}
}



int run(int option)//runs the Admin Menu
{
	int id;
    switch (option){
        case 1:	employees[curr].id = 11*curr;
				create (employees[curr].id); break; //create a new record.
        case 2: edit(); break; //edit a record.
        case 3: printf("Employee ID:");
				scanf("%d",&id);
				display(id); break; //display one record.
        case 4: displayAll(); break; //display all records.
        case 5:	printf("Employee ID:");
				scanf("%d",&id);
        		delete_record (id); break; // delete a record;
		case 6:	end_of_file+=1;
        		field_sort (end_of_file); break; // sort the records;
        case 7: printf("<EXITING ADMIN MODE>\n");Sleep(3000); system("cls"); break;
        default: printf("Incorrect input.\n"); Sleep (3000); system ("cls");
    }
}

void create(int id)
{
	char choice[4];
	if(curr<max)
	{
        printf("%d\n\n",curr);
        	printf("Please enter employee's information.\n");
			printf("First Name:");
			scanf("%s", &employees[curr].fname);
			fflush (stdin);
			printf("Last Name:");
			scanf("%s", &employees[curr].lname);
			fflush (stdin);
			printf("Date of birth\n");
			printf("Day[dd]:");
			scanf("%d", &employees[curr].DOB.day);
			printf("Month[mm]:");
			scanf("%d", &employees[curr].DOB.month);
			printf("Year[yyyy]:");
			scanf("%d", &employees[curr].DOB.year);
			fflush (stdin);
			printf("Gender [M/F]:");
			scanf("%s", &employees[curr].gender);
			fflush (stdin);
			printf("Phone Number:(876) ");
			gets(employees[curr].phone);
			printf("Address:");
			gets(employees[curr].address);
			printf("Years Worked at the Company:");
			scanf("%d", &employees[curr].years_worked);
			department_input ();
			system("cls");
            fflush(stdin);
            display(employees[curr].id);
			printf("Is this information correct?[Y/N]\n");
            scanf("%s", &choice);
            employees[curr].admin=false;
	}
    printf ("\nEmployee Information added successfully.\n");
    addToDatabase();
    addToLoginDatabase();
	curr++;
}

void edit()//Only Phone Number and Address Can be Edited
{
	int id,pos;
	printf("Employee ID:");
	scanf("%d",&id);
	pos=idSearch(id);
	if(pos!=-1)
	{
		printf("Phone Number: +1876");
		gets(employees[pos].phone);
		printf("Address:");
		gets(employees[pos].address);
		updateFile();
	}
	
}

void updateFile()//When changes are made file should update to represent the changes
{
	int i;
    char admin[5];
	Database=fopen("Employees.txt","w");
	if(Database!=NULL)
	for(i=0;i<curr;i++)
    {   
        if(employees[i].admin)
        strcpy(admin,"true");
        else
        strcpy(admin,"false");
        fflush(stdin);
	    fprintf(Database,"%02d  %-10s  %-11s  %02d/%02d/%4d  %-6s  (876)-%8s  %-45s  %-20d  %-27s  %-19s  %-10d  %-12d %s\n", employees[i].id, employees[i].fname, employees[i].lname, employees[i].DOB.day, employees[i].DOB.month, employees[i].DOB.year, employees[i].gender, employees[i].phone,employees[i].address, employees[i].years_worked, employees[i].department, employees[i].position, employees[i].late, employees[i].absent,admin);
    }
	fclose(Database);
}

void display(int id)//Displays Employee's Record if it exists
{
	int pos;
	pos= idSearch(id);
    printf("%d",pos);
	if(pos!=-1)
	{
		printf("Name:%s %s\n",employees[pos].fname,employees[pos].lname);
		printf("Date of Birth:%d/%d/%d\n",employees[pos].DOB.day,employees[pos].DOB.month,employees[pos].DOB.year);
		printf("Gender:%s\n",employees[pos].gender);
		printf("Phone:+1876%s\n",employees[pos].phone);
		printf("Address:%s\n",employees[pos].address);
		printf("Department:%s\n",employees[pos].department);
		printf("Position:%s\n",employees[pos].position);
		printf("Years Worked:%d\n",employees[pos].years_worked);
		printf("Days Late:%d\n",employees[pos].late);
		printf("Days Absent:%d\n",employees[pos].absent);
	}
}

void displayAll()
{
	for(int i=0;i<curr;i++)
	{
		display(i);
		printf("\n--------------------------------------------------------------------------------------------------------------------------");
	}
}

void department_input()//function to state what deparment the employee works and what position the employee holds
{
	int opt;
	while (opt > 12 || opt < 1)
	{
        system("cls");
		printf ("Which department is this employee in?\n\n");
		printf ("1) Prepared Foods and Pastries\n2) Frozen foods\n3) Produce\n4) Boxed and Canned foods\n5) Electronics\n6) Clothing and Textile\n7) Hardware\n8) Household and Aesthetics\n9) Cleaning items\n10) Everyday Goods\n11) Checkout and Administration\n12) IT and Support");
		printf ("\n\nPick the number corresponding with the department of choice: ");
		scanf ("%d", &opt);
		switch (opt)
		{
			case 1:
				strcpy (employees[curr].department,"Prepared Foods and Pastries");
				break;
			case 2:
				strcpy (employees[curr].department,"Frozen foods");
				break;
			case 3:
				strcpy (employees[curr].department,"Produce");
				break;
			case 4:
				strcpy (employees[curr].department,"Boxed and Canned foods");
				break;
			case 5:
				strcpy (employees[curr].department,"Electronics");
				break;
			case 6:
				strcpy (employees[curr].department,"Clothing and Textile");
				break;
			case 7:
				strcpy (employees[curr].department,"Hardware");
				break;
			case 8:
				strcpy (employees[curr].department,"Household and Aesthetics");
				break;
			case 9:
				strcpy (employees[curr].department,"Cleaning Items");
				break;
			case 10:
				strcpy (employees[curr].department,"Everyday Goods");
				break;
			case 11:
				strcpy (employees[curr].department,"Checkout and Administration");
				break;
			case 12:
				strcpy (employees[curr].department,"IT and Support");
				break;
			default:
				printf ("Incorrect input");
				Sleep (3000);
				system ("cls");
				break;
		}
	}
	opt=0;
	while (opt > 10 || opt < 1 )
	{
        system("cls");
		printf ("What position is this employee?\n\n");
		printf ("1) Head of Department\n2) Department Supervisor\n3) Cashier\n4) Stock Clerk\n5) Packer\n6) Chef\n7) Security\n8) Janitor\n9) Accountant\n10) Front Desk Clerk");
		printf ("\n\nPick the number corresponding with the position of choice: ");
		scanf ("%d", &opt);
		switch (opt)
		{
			case 1:
				strcpy (employees[curr].position,"Head of Department");
				break;
			case 2:
				strcpy (employees[curr].position,"Department Supervisor");
				break;
			case 3:
				strcpy (employees[curr].position,"Cashier");
				break;
			case 4:
				strcpy (employees[curr].position,"Stock Clerk");
				break;
			case 5:
				strcpy (employees[curr].position,"Packer");
				break;
			case 6:
				strcpy (employees[curr].position,"Chef");
				break;
			case 7:
				strcpy (employees[curr].position,"Security");
				break;
			case 8:
				strcpy (employees[curr].position,"Janitor");
				break;
			case 9:
				strcpy (employees[curr].position,"Accountant");
				break;
			case 10:
				strcpy (employees[curr].position,"Front Desk Clerk");
				break;
			default:
				printf ("Incorrect input\n");
				Sleep (3000);
				system ("cls");
				break;
		}
	}
}

void delete_record (int id)
{
    int pos;
	pos= idSearch(id);
	if(pos!=-1)
	{
        employees[pos]=employees[pos+1];
	}
    updateFile();
}

void field_sort (int eof)
{
	int seek;
	while (choice_int_1 < 1 || choice_int_1 > 11)
	{
		system ("cls");
		printf ("\n1) First name\n2) Last name\n3) Birthday\n4) Gender\n5) Phone #\n6) Address\n7) Length of employment\n8) Department\n9) Position\n10) Times Late\n11) Times Absent\n\n\tUnder which field would you like to sort the records? ");
		scanf ("%d", &choice_int_1);
	
		switch (choice_int_1)
		{
				case 1:	seek = 4; select_sort_string (seek); break;
				case 2: seek = 16; select_sort_string (seek); break;
				case 3: seek = 29; select_sort_string (seek); break;
				case 4: seek = 41; select_sort_string (seek); break;
				case 5: seek = 55; select_sort_string (seek); break;
				case 6: seek = 65; select_sort_string (seek); break;
				case 7: seek = 112; select_sort_string (seek); break;
				case 8: seek = 134; select_sort_string (seek); break;
				case 9: seek = 163; select_sort_string (seek); break;
				case 10: seek = 184; select_sort_string (seek); break;
				case 11: seek = 196; select_sort_string (seek); break;
				default: printf ("Incorrect option, please re-enter option"); Sleep (3000); system ("cls"); break;
		}
	
	}

}

void select_sort_string (int seek_amount)//sorts the employee database based on
{
	char c[1000][50], d[1000][50], temp[50];
	int i, j, min;
	
	for (i = 1; i <= end_of_file; i++)
	{
		fseek (Database, record_size*i+seek_amount, SEEK_SET);
		fscanf (Database, "%s", &c[i]);		
	}
	
	
	for (i = 1; i <= end_of_file; i++)
	{
		min = i;
		
		for (j = i; j <= end_of_file; j++)
		{
			
			if (strcmp (c[j], c[min]) < 0)
			{
				min = j;
			}
			
			strcpy (d[i], c[min]);
			
			if (strcmp (c[min], c[i]) != 0)
			{
				strcpy (temp, c[min]);
				strcpy (c[min], c[i]);
				strcpy (c[i], temp);
			}			
			 	
		}	
		
	} 
	
	printf ("ID  FIRSTNAME   LASTNAME     BIRTHDAY  	 GENDER  PHONENUMBER     ADDRESS                                        LENGTH OF EMPLOYMENT  DEPARTMENT                   POSITION             TIMES LATE  TIMES ABSENT\n");

	for (i = 1; i <= end_of_file ; i++)
	{
	
		for (j = 1; j <= end_of_file; j++)
		{
			fseek (Database, record_size*j+seek_amount, SEEK_SET);
			fscanf (Database, "%s", &c[j]);
	
			if (strcmp (d[i], c[j]) == 0)
			{
				fseek (Database, record_size*j, SEEK_SET);
				fgets (c[j], record_size-1, Database);
				printf ("%s\n", c[j]);
			}
	
		}
	
	}

}

//void selectionSort(int arr[], int n)
//{
//   int i, j, min_idx;
  
    // One by one move boundary of unsorted subarray
//    for (i = 0; i < n-1; i++)
//    {
//        // Find the minimum element in unsorted array
//        min_idx = i;
//        for (j = i+1; j < n; j++)
//          if (arr[j] < arr[min_idx])
//            min_idx = j;
  
        // Swap the found minimum element with the first element
//           if(min_idx != i)
//            swap(&arr[min_idx], &arr[i]);
//    }
//}