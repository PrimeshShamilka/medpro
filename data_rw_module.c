#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
*
* GET SINGLE FIELD
* 
*/

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}



/*
*
* STRING CONCAT
* 
*/

char *concatenate(char *a, char *b, char *c)
{
  int size = strlen(a) + strlen(b) + strlen(c) + 1;
  char *str = malloc(size);
  strcpy (str, a);
  strcat (str, b);
  strcat (str, c); 

  return str;
}



/*
*
* READ TABLE FIELDS
* 
*/

char* read_table_fields(char *data_table){
    
    char *file_path = concatenate("/home/primesh/medpro/", data_table, ".csv");
    FILE* stream;
    if ((stream = fopen(file_path,"r")) == NULL)
    {
    printf("Error! opening file");
    }

    int count = 0;
    char line[1024];
    char * table_fields;

    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        table_fields = tmp;
        count++;
        if(count == 1){
            break;
        }
    }
    fclose(stream);
    return table_fields;
}



/*
*
* READ OWN RECORD IN THE TABLE
* 
*/


void read_table_own(char *data_table, char *user_name){

    char *file_path = concatenate("/home/primesh/medpro/", data_table, ".csv");
    FILE* stream;
    if ((stream = fopen(file_path,"r")) == NULL)
    {
    printf("Error! opening file");
    }

    char line[1024];
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        char *current_user = getfield(tmp, 1);
        if(strcmp(user_name, current_user  == 0)){
            printf("%s",tmp);
        }else{
            printf("YOUR DATA RECORD IS NOT IN THE DATABASE.");
        }

        free(tmp);
    }
    fclose(stream);
}



/*
*
* READ ALL TABLE RECORDS
* 
*/

void read_table(char *data_table){
    
    char *file_path = concatenate("/home/primesh/medpro/", data_table, ".csv");
    FILE* stream;
    if ((stream = fopen(file_path,"r")) == NULL)
    {
    printf("Error! opening file");
    }

    char line[1024];
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        printf("%s",tmp);
        free(tmp);
    }
    fclose(stream);
}



/*
*
* READ DATA 
* 
*/


void read_data(char *username, char *user_type, char read_category){

    char *db_tables[] = {"admins_details_table",
                    "doctor_details_table",
                    "nurse_details_table",
                    "patient_details_table",
                    "sickness_details_table",
                    "drug_prescriptions_table",
                    "lab_test_prescriptions_table"};
    
    char *data_table;

    switch (read_category)
    {
        case 'A':
            data_table = db_tables[0];
            break;
        case 'B':
            data_table = db_tables[1];
            break;
        case 'C':
            data_table = db_tables[2];
            break;
        case 'D':
            data_table = db_tables[3];
            break;
        case 'E':
            data_table = db_tables[4];
            break;
        case 'F':
            data_table = db_tables[5];
            break;
        case 'G':
            data_table = db_tables[6];
            break;
    }

    char *db_location = concatenate(data_table, ".", user_type);

    struct privileges usr_privilege = get_user_privileges(db_location);
    char *r_privilege = usr_privilege.r_privilege;

    if(strcmp(r_privilege, "READ_ANY") == 0){
        read_table(data_table);
    }else if(strcmp(r_privilege, "READ_OWN") == 0){
        read_table_own(data_table, username);
    }else{
        printf("YOU DON'T HAVE ACCESS TO THIS DATA.");
    }

}




/*
*
* WRITE TABLE RECORDS
* 
*/


void write_table(char *user_inputs[10], char *data_table, int count){

    char *file_path = concatenate("/home/primesh/medpro/", data_table, ".csv");
    FILE* stream;
    if ((stream = fopen(file_path,"a")) == NULL)
    {
    printf("Error! opening file");
    }
    printf("%s",user_inputs[2]);
    char *data_record = "";
    data_record = concatenate(data_record, ",", user_inputs[0]);

    for (int x = 0; x <5; x++){
        data_record = concatenate(data_record, ",", user_inputs[x]);
    }

    fprintf(stream, "%s,\n", "data_record");

    fclose(stream);
}




/*
*
* WRITE TABLE 
* 
*/


void write_data(char *username, char *user_type, char write_category){

    char *db_tables[] = {"admins_details_table",
                    "doctor_details_table",
                    "nurse_details_table",
                    "patient_details_table",
                    "sickness_details_table",
                    "drug_prescriptions_table",
                    "lab_test_prescriptions_table"};
    
    char *data_table;

    switch (write_category)
    {
        case 'A':
            data_table = db_tables[0];
            break;
        case 'B':
            data_table = db_tables[1];
            break;
        case 'C':
            data_table = db_tables[2];
            break;
        case 'D':
            data_table = db_tables[3];
            break;
        case 'E':
            data_table = db_tables[4];
            break;
        case 'F':
            data_table = db_tables[5];
            break;
        case 'G':
            data_table = db_tables[6];
            break;
    }

    char *db_location = concatenate(data_table, ".", user_type);
    struct privileges usr_privilege = get_user_privileges(db_location);
    char *w_privilege = usr_privilege.w_privilege;

    if(strcmp(w_privilege, "WRITE") == 0){
        char* table_fields = read_table_fields(data_table);
        char* user_inputs[10];
        char* token; 
        char* rest = table_fields; 

        strtok(table_fields, "\n");

        printf("ENTER THE FOLLOWING FIELDS\n");
        int i = 0;
        while ((token = strtok_r(rest, ",", &rest))){ 
            printf("%s:\n", token); 
            char *temp;
            fgets(user_inputs[i], 10, stdin);
            i++;
        }
        char *temp;
        write_table(user_inputs, data_table, i);
    }else{
        printf("YOU DON'T HAVE ACCESS TO THIS DATA.");
    }
}