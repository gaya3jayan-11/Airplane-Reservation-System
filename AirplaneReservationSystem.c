#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// linked list for storing flight details
struct flightNode
{
    int num;
    int avail_seats;
    char from[100], to[100];
    struct flightNode *link;
};
typedef struct flightNode *flight;
flight f_head = NULL;

// linked list for storing user login details
struct loginNode
{
    char user_id[100], password[100];
    struct loginNode *link;
};
typedef struct loginNode *login;
login l_head = NULL;

// linked list for storing booked tickets
struct bookNode
{
    char id[100], from[100], to[100];
    int num, seat_num;
    struct bookNode *link;
};
typedef struct bookNode *book;
book b_head = NULL;

// creating flight node
flight create_flight(int n)
{
    flight temp = (flight)malloc(sizeof(struct flightNode));
    if (temp == NULL)
    {
        printf("\n\x1b[31mMemory not available\x1b[0m");
        exit(0);
    }
    printf("\nEnter From, To, Total Number of Seats: ");
    scanf("%s%s%d", temp->from, temp->to, &temp->avail_seats);
    temp->num = n;
    temp->link = NULL;
    return temp;
}

// printing flight details
void print_flight_details()
{
    if (f_head == NULL)
    {
        printf("There are no flights");
        return;
    }
    printf("\nFlight_No.\tFrom\t\tTo\t\tAvailable_seats");
    flight cur = f_head;
    while (cur != NULL)
    {
        printf("\n%-12d\t%-15s\t%-15s\t%d", cur->num, cur->from, cur->to, cur->avail_seats);
        cur = cur->link;
    }
}

// adding flight details
void add_flight_details()
{
    int n, flag = 0;
    printf("\nEnter Flight Num: ");
    scanf("%d", &n);

    if (f_head == NULL)
    {
        f_head = create_flight(n);
        return;
    }

    flight cur = f_head,prev=NULL;
    while (cur!= NULL)
    {
        if (cur->num == n)
        {
            flag = 1;
            break;
        }
        prev=cur;
        cur = cur->link;
    }

    if (flag == 1)
    {
        printf("\x1b[31mFlight with num %d already exists\x1b[0m", n);
        return;
    }

    // Create a new flight
    flight new_flight = create_flight(n);

    // Add the new flight to the end of the list
    prev->link = new_flight;
}

// removing flight details
void remove_flight()
{
    int n;
    printf("\nEnter Flight Num to be deleted: ");
    scanf("%d", &n);

    flight cur = f_head;
    flight prev = NULL;

    while (cur != NULL)
    {
        if (cur->num == n)
        {
            if (prev == NULL)
            {
                // If the node to be removed is the head of the list
                f_head = cur->link;
            }
            else
            {
                // Update the link of the previous node to skip the current node
                prev->link = cur->link;
            }

            printf("\x1b[32mFlight with num %d successfully removed \x1b[0m", cur->num);
            free(cur);
            return;
        }

        prev = cur;
        cur = cur->link;
    }

    // If the flight with the given number is not found
    printf("\x1b[31mFlight with num %d does not exist\x1b[0m", n);
}

// updating flight details
void update_flight_details()
{
    int ch;
    while (1)
    {
        printf("\n\n1.Add Flight\t2.Remove Flight\t 3.Display Available flights\t4.Return back");
        printf("\nEnter Choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            add_flight_details();
            break;
        case 2:
            if (f_head == NULL)
                printf("\nThere are no flights");
            else
            {
                print_flight_details();
                remove_flight();
            }
            break;

        case 3: print_flight_details();
            break;

        case 4:
            return;
        default:
            printf("\x1b[31mINVALID CHOICE\x1b[0m");
        }
    }
}

// admin function
void admin()
{
    int pswd;
    printf("\n\n~~~~~\n\nEnter admin password: ");
    scanf("%d", &pswd);
    if (pswd == 29110209)
    {
        update_flight_details();
    }
    else
        printf("\x1b[31mWRONG PASSWORD\x1b[0m");
}

// creating user login node
login create_user_login(char uid[])
{
    login temp = (login)malloc(sizeof(login));
    if (temp == NULL)
    {
        printf("\n\x1b[31mMemory not available\x1b[0m");
        exit(0);
    }
    printf("\nEnter Password: ");
    scanf("%s", temp->password);
    strcpy(temp->user_id, uid);
    temp->link = NULL;
    return temp;
}

// Function to book a ticket
void book_ticket(char uid[])
{
    // Print available flights
    printf("\nAvailable Flights:");
    print_flight_details();

    // Get flight details from the user
    int flight_num;
    printf("\n\nEnter Flight Num to book a ticket: ");
    scanf("%d", &flight_num);

    // Check if the flight exists
    flight selected_flight = f_head;
    int flight_found = 0;
    while (selected_flight != NULL)
    {
        if (selected_flight->num == flight_num)
        {
            flight_found = 1;
            int ticket_num,ticket_found=0;
            printf("\n\nEnter Number of tickets: ");
            scanf("%d",&ticket_num);
            // Check if there are available seats
            if (selected_flight->avail_seats < ticket_num)
            {
                printf("\x1b[31mNo available seats for Flight %d\x1b[0m", flight_num);
                return;
            }

            book cur_ticket=b_head;
            while(cur_ticket!=NULL)
            {
                if(cur_ticket->num==flight_num && cur_ticket->id==uid)
                {
                    cur_ticket->seat_num+=ticket_num;
                    ticket_found=1;
                    break;
                }
                cur_ticket=cur_ticket->link;
            }

            if(!ticket_found)
            {
                // Create a new booked ticket node
                book new_ticket = (book)malloc(sizeof(struct bookNode));
                if (new_ticket == NULL)
                {
                    printf("\n\x1b[31mMemory not available\x1b[0m");
                    exit(0);
                }

                strcpy(new_ticket->id, uid);
                strcpy(new_ticket->from, selected_flight->from);
                strcpy(new_ticket->to, selected_flight->to);
                new_ticket->num = flight_num;
                new_ticket->seat_num = ticket_num;

                // Add the new ticket to the booked tickets list
                new_ticket->link = b_head;
                b_head = new_ticket;
            }
            // Decrement available seats and book the ticket
            selected_flight->avail_seats-=ticket_num;
            printf("\x1b[32mTickets booked successfully for Flight %d\x1b[0m", flight_num);
            break;
        }
        selected_flight = selected_flight->link;
    }

    if (!flight_found)
    {
        printf("\x1b[31mFlight with num %d not found\x1b[0m", flight_num);
        return;
    }


}

// Function to cancel a ticket
void cancel_ticket(char uid[])
{
    // Print booked tickets for the user
    printf("\nBooked Tickets for User %s:", uid);
    book cur_ticket = b_head;
    int ticket_found = 0;
    while (cur_ticket != NULL)
    {
        if (strcmp(cur_ticket->id, uid) == 0)
        {
            ticket_found++;
            printf("\nFlight: %d \t\tFrom: %s \t\tTo: %s\t\t No. of Tickets: %d", cur_ticket->num, cur_ticket->from, cur_ticket->to, cur_ticket->seat_num);
        }
        cur_ticket = cur_ticket->link;
    }

    if (!ticket_found)
    {
        printf("\n\x1b[31mNo booked tickets found for User %s\x1b[0m", uid);
        return;
    }

    // Get flight number to cancel the ticket
    int flight_num,ticket_num;
    printf("\nEnter Flight Num to cancel the ticket: ");
    scanf("%d", &flight_num);
    printf("Enter number of tickets to be cancelled: ");
    scanf("%d", &ticket_num);

    // Find the booked ticket to cancel
    book prev_ticket = NULL;
    cur_ticket = b_head;
    while (cur_ticket != NULL)
    {
        if (strcmp(cur_ticket->id, uid) == 0 && cur_ticket->num == flight_num)
        {
            cur_ticket->seat_num-=ticket_num;
            if(cur_ticket->seat_num==0)
            {
                if (prev_ticket == NULL)
                {
                    // If the ticket to cancel is the head of the list
                    b_head = cur_ticket->link;
                }
                else
                {
                    // Update the link of the previous ticket to skip the current ticket
                prev_ticket->link = cur_ticket->link;
                }
                free(cur_ticket);
            }
            // Increment available seats for the canceled flight
            flight canceled_flight = f_head;
            while (canceled_flight != NULL)
            {
                if (canceled_flight->num == flight_num)
                {
                    canceled_flight->avail_seats+=ticket_num;
                    break;
                }
                canceled_flight = canceled_flight->link;
            }

            printf("\x1b[32mTicket canceled successfully for Flight %d\x1b[0m", flight_num);
            return;
        }

        prev_ticket = cur_ticket;
        cur_ticket = cur_ticket->link;
    }

    printf("\x1b[31mTicket not found for Flight %d\x1b[0m", flight_num);
}

// logging in valid user
void user_login()
{
    printf("\n\n~~LOGIN PAGE~~");
    char uid[100];
    printf("\n\nUSER ID: ");
    scanf("%s", uid);
    login cur = l_head;
    int flag = 0;
    while (cur != NULL)
    {
        if (strcmp(cur->user_id, uid) == 0)
        {
            flag = 1;
            break;
        }
        cur = cur->link;
    }
    if (flag == 0)
    {
        printf("\n\x1b[31mINVALID USER ID\x1b[0m");
        printf("\nPlease enter Valid User ID\n(or register if you do not have an account)");
        return;
    }
    char pw[100];
    printf("\nPASSWORD: ");
    scanf("%s", pw);
    if (strcmp(cur->password, pw) == 0)
    {
        printf("\n\x1b[32mSuccessfully Logged in \x1b[0m");
        while (1)
        {
            printf("\n\n1.Book Ticket\t2.Cancel Ticket\t 3.Return to Registration");
            int ch;
            printf("\n\nEnter choice: ");
            scanf("%d", &ch);
            switch (ch)
            {
            case 1:
                book_ticket(uid);
                break;
            case 2:
                cancel_ticket(uid);
                break;
            case 3:
                return;
            default:
                printf("Invalid input");
            }
        }
        return;
    }
    printf("\nINVALID PASSWORD");
}

// registering user
void register_user()
{
    printf("\n\n~~REGISTRATION PAGE~~");
    char uid[100];
    printf("\n\nEnter User ID: ");
    scanf("%s", uid);
    if (l_head == NULL)
    {
        l_head = create_user_login(uid);
        printf("\n\x1b[32mSuccesfully Registered \x1b[0m");
        return;
    }
    login cur = l_head,prev=NULL;
    int flag = 0;
    while (cur != NULL)
    {
        if (strcmp(cur->user_id, uid) == 0)
        {
            flag = 1;
            break;
        }
        prev=cur;
        cur = cur->link;
    }
    if (flag == 1)
    {
        printf("\n\x1b[31mUser ID already exists\x1b[0m");
        return;
    }
    prev->link = create_user_login(uid);
    printf("\n\x1b[32mSuccesfully Registered \x1b[0m");
}

// user function
void user()
{
    int ch;
    while (1)
    {
        printf("\n\n1.Login\t2.Register\t3.Return back to Main Menu");
        printf("\n\nEnter your choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            user_login();
            break;
        case 2:
            register_user();
            break;
        case 3:
            return;
        }
    }
}

int main()
{
    int ch;
    while (1)
    {
        printf("\n\n\t\t\t\x1b[33m~~~WELCOME TO LINKED LIST AIRLINES~~~\x1b[0m\n");
        printf("\t\t\t1.Admin\t\t2.User\t\t3.Exit");
        printf("\n\nEnter your choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            admin();
            break;
        case 2:
            user();
            break;
        case 3:
            printf("\n\x1b[36mTHANK YOU \x1b[0m");
            exit(0);
        default:
            printf("\n\x1b[31mINVALID CHOICE \x1b[0m");
        }
    }
}
