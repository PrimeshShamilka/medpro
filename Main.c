'''
   Copyright (c) 2020, Primesh Shamilka,
   email: primeshs.17@cse.mrt.ac.lk
   All rights reserved. https://github.com/PrimeshShamilka/
   
   Revision history:
	  May 3rd, 2020: version1.0.
'''

#include <stdio.h>
#include "authenticator.c"
#include "data_rw_module.c"
#include <stdlib.h>
#include <string.h>

int main(){
    char login;
    char add_new_user;
    char read_or_write;
    char* username;
    char* user_type;
    char read_category;
    char write_category;


    printf("WELCOME TO MEDPRO HOMEPAGE!..\n");

    /*
    *
    * USER AUTHENTICATION
    * 
    */

    printf("Press L to login\n");
    scanf("%c", &login);
    getchar();

    struct user current_user = authenticate();
    username = current_user.username;
    user_type = current_user.user_type;


    /*
    *
    * ADD NEW USER FOR ADMINS
    * 
    */

    if(strcmp(user_type,"admin") == 0)
    {
        printf("Press A to add new user\n");
        scanf("%c", &add_new_user);
        getchar();

        if(add_new_user == 'A'){
            add_user();
        }else{
            printf("do something");
        }
    }

    /*
    *
    * READ OR WRITE DATA
    * 
    */


    else
    {
        printf("\nPress R to READ data or W to WRITE data\n");
        scanf("%c", &read_or_write);
        getchar();

        if(read_or_write == 'R')
        {
            printf("\nSELECT THE DATA CATEGORY\n"
                    "========================\n");
            printf("A:ADMIN DETAILS\n"
                "B:DOCTORS DETAILS\n"
                "C:NURSES DETAILS\n"
                "D:PATIENT DETAILS\n"
                "E:SICKNESS DETAILS\n"
                "F:DRUG PRESCRIPTIONS\n"
                "G:LAB TEST PRESCRIPTIONS\n");
            scanf("%c", &read_category);
            getchar();
            
            // read_data(username, user_type, read_category);
        }
        else if(read_or_write == 'W')
        {
            printf("A:ADMIN DETAILS\n"
                "B:DOCTORS DETAILS\n"
                "C:NURSES DETAILS\n"
                "D:PATIENT DETAILS\n"
                "E:SICKNESS DETAILS\n"
                "F:DRUG PRESCRIPTIONS\n"
                "G:LAB TEST PRESCRIPTIONS\n");
            scanf("%c", &write_category);
            getchar();

            // write_data(username, user_type, read_category);

        }
    }


return 0;
}

