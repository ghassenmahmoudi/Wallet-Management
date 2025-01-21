#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*We put in this structure the name of the categories*/
typedef struct node2
{
    char arr[50];
    struct node2 *p_next;
}node2;
typedef node2 *categories;

typedef struct Date
{
    int year,month,day;
}Date;

typedef struct Data
{
    int income_exp;
    Date date;
    char category[50];
    double amount;

}Data;

/*The structure of the node of a double linked list*/
typedef struct node
{
    Data data_t;
    struct node *p_next;
    struct node *p_prev;
}node;

/*The structure of the the double linked list (Wallet) and make we use the head and tail to make it easier to navigate*/
typedef struct
{
    node* head;
    node* tail;
}wallet;

/*Initialise the Wallet Database*/
void initial_wallet(wallet *Wallet)
{
    Wallet->head=NULL;
    Wallet->tail=NULL;
}

/* This function Adds a new income/expense data_t to the Wallet database*/
void add_data(wallet* Wallet, Data new_data)
{
    node* new_node=(node*)malloc(sizeof(node));
    new_node->data_t=new_data;
    new_node->p_next=NULL;
    new_node->p_prev=NULL;
    if (Wallet->head == NULL)
    {
        Wallet->head=new_node;
        Wallet->tail=new_node;
    }
    else{
        Wallet->tail->p_next = new_node;
        new_node->p_prev=Wallet->tail;
        Wallet->tail=new_node;
    }
}



/*This function adds a new category to the categories list. It creates a new node,
 copies the category name to the node, and appends it to the end of the list.*/
void add_data_cat(categories *all_cat, char* cat)
{
    categories new_node,move;
    new_node = (categories)malloc(sizeof(node2));
    strcpy(new_node->arr, cat);
    new_node->p_next = NULL;

    if (*all_cat == NULL)
    {
        *all_cat=new_node;
    }
    else
    {
        move = *all_cat;
        while (move->p_next != NULL)
        {
            move = move->p_next;
        }

        move->p_next = new_node;

    }

}


/*This function displays the summary of expenses and incomes. It calculates the total income, total expense,
the most expensive category,
the money spent on that category, and the overall balance.
It iterates through the linked list of transactions, updating the respective variables.
This means evaluating all entries of the database,*/
void display_summary(wallet Wallet){
    double total_income = 0;
    double total_expense = 0;
    double max_expense = 0;
    char most_expensive_cat[50] = "";
    double category_expense = 0;

    node* move = Wallet.head;
    while (move != NULL) {
        if (move->data_t.income_exp == 0) {                                               //0 for income, 1 for expense
            total_income += move->data_t.amount;
        } else {
            total_expense += move->data_t.amount;
            if (move->data_t.amount > max_expense) {
                max_expense = move->data_t.amount;
                strcpy(most_expensive_cat, move->data_t.category);
            }
            if (strcmp(move->data_t.category, most_expensive_cat) == 0) {
                category_expense += move->data_t.amount;
            }
        }
        move = move->p_next;
    }

    double balance = total_income - total_expense;

    printf("Total Incomes: %.2f\n", total_income);
    printf("Total Expenses: %.2f\n", total_expense);
    printf("Most Expensive Category: %s\n", most_expensive_cat);
    printf("Money Spent for %s: %.2f\n", most_expensive_cat, category_expense);
    printf("Overall Balance: %.2f\n", balance);

}

/*save the Database*/
void save_database(wallet Wallet) {
    // Delete the existing database file
    remove("database_wallet.txt");
    /*Save the new database*/
    FILE* fp = fopen("database_wallet.txt", "w");
    if (fp == NULL) {
        exit(0);
    }
    node* move = Wallet.head;
    while (move != NULL) {
        fprintf(fp, "%s %.2f %d %d %d %d\n", move->data_t.category, move->data_t.amount, move->data_t.income_exp, move->data_t.date.year, move->data_t.date.month, move->data_t.date.day);
        move = move->p_next;
    }

    fclose(fp);
}

int check_cat_incat(categories cat, char* t) {
    categories move = cat;
    int Flag = 0;
    while (move!= NULL) {
        if (strcmp((move->arr), t) == 0) {      //The category is already existing in the categories list
            Flag = 1;
            break;
        } else {
            move = move->p_next;
        }
    }
    return Flag;
}
/*In the delete_wallet function, it iterates through the Wallet list,
freeing each node's memory and updating the head and tail pointers to NULL after deleting all nodes.*/
void delete_wallet(wallet* Wallet) {
    node* current = Wallet->head;
    while (current != NULL) {
        node* temp = current;
        current = current->p_next;
        free(temp);
    }
    Wallet->head = NULL;
    Wallet->tail = NULL;
}
/*In the delete_categories function, it iterates through the cat category list,
 freeing each node's memory and updating the cat pointer to NULL after deleting all nodes.*/
void delete_categories(categories* cat) {
    node2* current = *cat;
    while (current != NULL) {
        node2* temp = current;
        current = current->p_next;
        free(temp);
    }
    *cat = NULL;
}
/*Loading database*/
void load_database(wallet* Wallet, categories* cat) {
    // Delete the existing Wallet list
    delete_wallet(Wallet);

    // Delete the existing category list
    delete_categories(cat);

    FILE* fp = fopen("database_wallet.txt", "r");
    if (fp == NULL) {
        exit(1);
    }

    char category[50];
    float amount;
    int income_exp, year, month, day;

    while (fscanf(fp, "%s %f %d %d %d %d", category, &amount, &income_exp, &year, &month, &day) == 6) {
        Data new_data;
        new_data.income_exp = income_exp;
        new_data.amount = amount;
        new_data.date.year = year;
        new_data.date.month = month;
        new_data.date.day = day;
        strcpy(new_data.category, category);
        add_data_cat(cat, new_data.category);
        add_data(Wallet, new_data);
    }

    fclose(fp);
}


/*This functionThis function check_date is used to compare two dates and determine if the first date is within the given timeline (date2).
 It compares the year, month, and day of the two dates to make the comparison and
 returns 0 if the first date is earlier or equal to the second date, and 1 if it is later.*/
int check_date(Date date1, Date date2)
{
    if (date1.year < date2.year)
    {
        return 0;
    }
    else if (date1.year > date2.year)
    {
        return 1;
    }
        else{
        if (date1.month < date2.month)
        {
            return 0;
        }
        else if (date1.month > date2.month)
        {
            return 1;
        }
        else {
            if (date1.day < date2.day)
                {
                return 0;
                }
            else
            {
                return 1;
            }
        }
    }
}

/*Display statistics for each category*/
void displayCategoryStatistics(wallet *walletList, categories categoryList, Date start_Date, Date end_Date)
{
    categories currentCategory = categoryList;
    while (currentCategory != NULL)
    {
        char categoryName[50];
        strcpy(categoryName, currentCategory->arr);
        double totalAmount = 0.0;
        double largestExpense = 0.0;  // Variable to track the largest expense

        node *currentTransaction = walletList->head;
        while (currentTransaction != NULL)
        {
            // Check if the transaction falls within the specified date range and category
            if ((check_date(currentTransaction->data_t.date, start_Date)) && (check_date(end_Date, currentTransaction->data_t.date)) && (strcmp(currentTransaction->data_t.category, categoryName) == 0))
            {
                totalAmount += currentTransaction->data_t.amount;

                // Update the largest expense if the current transaction amount is greater
                if (currentTransaction->data_t.amount > largestExpense)
                {
                    largestExpense = currentTransaction->data_t.amount;
                }
            }

            currentTransaction = currentTransaction->p_next;
        }

        printf("Category: %s\n", categoryName);
        printf("Total amount: %.2f\n", totalAmount);
        printf("Largest expense: %.2f\n", largestExpense);

        printf("------------------------------\n");

        currentCategory = currentCategory->p_next;
    }
}



int is_valid_date(int year, int month, int day) {
    // Validate year
    if (year < 0 || year > 9999) {
        return 0;
    }

    // Validate month
    if (month < 1 || month > 12) {
        return 0;
    }

    // Validate day
    if (day < 1 || day > 31) {
        return 0;
    }

    // Additional validation rules can be added here, such as checking for leap years

    return 1;
}

int is_valid_choice(int choice) {
    return (choice >= 0 && choice <= 7);
}




int main()
{

    wallet Wallet;
    initial_wallet(&Wallet);
    categories all_category;
    all_category=NULL;
    // The Menu
    int choice;
    do {
        printf("\n--- Menu ---\n");
        printf("0. Load database\n");
        printf("1. Add new income/expense\n");
        printf("2. Save database\n");
        printf("3. Create new category\n");
        printf("4. Display summary\n");
        printf("5. Display category statistics within a period of time\n");
        printf("6. Display category statistics\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice entered. Please try again.\n");
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }

        if (!is_valid_choice(choice)) {
            printf("Invalid choice entered. Please try again.\n");
            continue;
        }

        switch (choice) {
        case 0:{
            /*Loading DATABASE*/
            load_database(&Wallet,&all_category);
            break;
        }

        case 1: {
            Data data_t;
            do{
            printf("Enter the date (year month day): ");
            scanf("%4d %2d %2d", &data_t.date.year, &data_t.date.month, &data_t.date.day);

            if (!is_valid_date(data_t.date.year, data_t.date.month,data_t.date.day)) {
                printf("Invalid date entered.\n");
            }
            }while(is_valid_date==0);
            do{
            printf("Enter the type (0 for income, 1 for expense): ");
            scanf("%d", &data_t.income_exp);
            }while(data_t.income_exp!=1 && data_t.income_exp!=0);
            printf("Enter the category: ");
            scanf("%s", data_t.category);
            do{
            printf("Enter the amount: ");
            scanf("%lf", &data_t.amount);
            }while(data_t.amount<0 || getchar() != '\n');
            add_data(&Wallet, data_t);
            if (check_cat_incat(all_category,data_t.category)==0)                                      /*We check if the category exists or Not */
                add_data_cat(&all_category,data_t.category );                                         /*Add the new category*/
            break;
        }

        case 2: {
            /*Saving Database*/
            save_database(Wallet);
            break;
        }
        case 3: {
            char new_cat[50];
            printf("Enter the category: ");
            scanf("%s", new_cat);
            if (check_cat_incat(all_category,new_cat)==0){              //check if the category is already registered in the list
                add_data_cat(&all_category,new_cat);}
            break;
            }

        case 4: {
            /*This function helps display the stats and including all entries of the database*/
            display_summary(Wallet);
            break;
        }
        case 5: {
            Date start_Date, end_Date;
            do{
            printf("Enter the start date (year month day): ");
            scanf("%d %d %d", &start_Date.year, &start_Date.month, &start_Date.day);
            if (!is_valid_date(start_Date.year, start_Date.month,start_Date.day)) {
                printf("Invalid date entered.\n");
            }
            }while(is_valid_date(start_Date.year, start_Date.month,start_Date.day)==0);
            do{
            printf("Enter the end date (year month day): ");
            scanf("%d %d %d", &end_Date.year, &end_Date.month, &end_Date.day);
            if (!is_valid_date(end_Date.year, end_Date.month,end_Date.day)) {
                printf("Invalid date entered.\n");
            }
            }while(is_valid_date(end_Date.year, end_Date.month,end_Date.day)==0);
            displayCategoryStatistics(&Wallet,all_category, start_Date,end_Date);
            break;
        }
        case 6:{
            Date d1={0,0,0};Date d2={9998,0,0};                 //This 2 dates to include all the transaction
            displayCategoryStatistics(&Wallet,all_category,d1,d2);
            break;
        }

       case 7: {
           printf("Exiting the program...\n");
            break;
        }
        default: {
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }

}while (choice != 7);


    return 0;
}
